// https://twitter.com/wakwak-koba/

//#define RADIKO_USER "SET YOUR MAIL-ADDRESS"
//#define RADIKO_PASS "SET YOUR PREMIUM PASS"

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include <WebRadio_Radiko.h>
#include <AudioOutputI2S.h>
#include <ArduinoOTA.h>
#include <WebServer.h>

#include "LCD_MagimajyoPures.hpp"
#include "lgfx_ja3_ayug.h"

static AudioOutputI2S out(0, AudioOutputI2S::EXTERNAL_I2S, 10);
static Radiko radio(&out, APP_CPU_NUM);
static WebServer httpd(80);
static String stationName;
static String programName;

#ifdef _LCD_MAGIMAJYO_PURES_HPP_
static LCD_MagimajoPures lcd;
static lgfx::LGFX_Sprite sprite; // [640 * 48] Buffer
static const lgfx::U8g2font font_L = { lgfx_ja3_ayug_20 } ;
#endif

void setup() {
  Serial.begin(115200);
//out.SetPinout(26, 25, 22);  // bck, lrc, dout
  
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
  lcd.init();
  lcd.setColorDepth(16);
  lcd.setRotation(2); // 0 or 2
  sprite.setPsram(true);
  sprite.setColorDepth(16);
  sprite.createSprite(640, 48);
  sprite.setSwapBytes(true);
  sprite.setFont(&font_L);
#endif

  WiFi.begin();
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());  
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
  sprite.print("IP address:");
  sprite.println(WiFi.localIP());  
  lcd.writeBuffer(sprite);
#endif

  ArduinoOTA.onStart([]() {
    Serial.println("ArduinoOTA.onStart");
    radio.stop();
    out.stop();
  });
  ArduinoOTA.begin();

#if defined( RADIKO_USER ) && defined( RADIKO_PASS )
  radio.setAuthorization(RADIKO_USER, RADIKO_PASS);
#endif
  radio.setEnableSBR(true);
  radio.onPlay = [&](const char * station_name, const size_t station_idx) {
    Serial.printf("onPlay:%d %s\n", station_idx, station_name);
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
    sprite.setFont(&font_L);
    sprite.clear();
    sprite.drawString(station_name, 0, 0);
    sprite.setFont(&fonts::Font0);
    sprite.drawString(WiFi.localIP().toString().c_str(), 550, 40);
    lcd.writeBuffer(sprite);
#endif
    stationName = String(station_name);
  };
  radio.onProgram = [&](const char * text) {
    if(!programName.equals(text)) {
      Serial.printf("onProgram:%s\n", text);
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
      sprite.setFont(&font_L);
      sprite.clear();
      sprite.drawString(stationName.c_str(), 0, 0);
      sprite.drawString(text, 0, 24);
      sprite.setFont(&fonts::Font0);
      sprite.drawString(WiFi.localIP().toString().c_str(), 550, 40);
      lcd.writeBuffer(sprite);
#endif
      programName = String(text);
    }
  };
  radio.onInfo = [&](const char * text) {
    Serial.println(text);
  };
  radio.setSyslog("255.255.255.255");
  if(!radio.begin()) {
    Serial.println("failed: radio.begin()");
    for(;;);
  }

  for(int i = 0; i < radio.getNumOfStations(); i++)
    Serial.printf("%d %s\n", i, radio.getStation(i)->getName());

  httpd.on("/top", [&]() {
    if(httpd.method() == HTTP_POST) {
      auto select_station = httpd.arg("station");
      if(select_station.length()) {
        auto station = radio.getStation(select_station.toInt());
        station->play();
        stationName = String(station->getName()) + " に切り替え中";
      }
    }

    httpd.sendHeader("Connection", "close");
    String html =
      "<!DOCTYPE html><html>"
      "<head>"
      "<title>WebRadio_Radiko</title>"
      "<meta charset=\"UTF-8\">"
      "<meta http-equiv=\"refresh\" content=\"5\">"
      "</head>"
      "<body>"
      "<h1>" + stationName + "<h1>"
      "</body>"
      "</html>";
    httpd.send(200, "text/html", html);
  });
  
  httpd.on("/list", HTTP_GET, [&]() {
    httpd.sendHeader("Connection", "close");
    String htmlHeader = 
      "<!DOCTYPE html><html>"
      "<head>"
      "<title>WebRadio_Radiko</title>"
      "<meta charset=\"UTF-8\">"
      "<script type=\"text/javascript\">"
      "  function  sel(index) {"
      "    form.station.value = index;"
      "    form.submit();"
      "  }"
      "</script>"
      "</head>"
      "<body>"
      "<h2>WebRadio_Radiko</h2>"
      "<form method=\"post\" action=\"top\" target=\"top\" id=\"form\">"
      "  <input type=\"hidden\" name=\"station\" />"
      "</form>";
    String htmlFooter = "</body></html>";
    String htmlbody;
    for(int i = 0; i < radio.getNumOfStations(); i++)
      htmlbody += "<a href=\"javascript:sel(" + String(i) + ")\">" + String(radio.getStation(i)->getName()) + "</a><br />";
    httpd.send(200, "text/html", htmlHeader + htmlbody + htmlFooter);
  });

  httpd.on("/", HTTP_GET, [&]() {
    httpd.sendHeader("Connection", "close");
    String html = 
      "<!DOCTYPE html><html>"
      "<head>"
      "<title>WebRadio_Radiko</title>"
      "<meta charset=\"UTF-8\">"
      "<frameset rows=\"96,*\" cols=\"*\" frameborder=\"no\">"
      "  <frame src=\"top\" name=\"top\" frameborder=\"no\" scrolling=\"no\" />"
      "  <frame src=\"list\" name=\"list\" frameborder=\"no\" scrolling=\"auto\" />"
      "</frameset>"
      "<noframes>"
      "  <body>"
      "  フレームを使っています"
      "  </body>"
      "</noframes>"
      "</html>";
    httpd.send(200, "text/html", html);
  });

  httpd.begin();
  radio.play();
}

void loop() {
  ArduinoOTA.handle();
  httpd.handleClient();
  radio.handle();
}