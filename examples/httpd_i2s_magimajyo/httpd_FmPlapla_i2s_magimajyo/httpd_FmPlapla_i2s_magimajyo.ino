// https://twitter.com/wakwak-koba/

#include <WebRadio_FmPlapla.h>
#include <AudioOutputI2S.h>
#include <ArduinoOTA.h>
#include <WebServer.h>

#include "LCD_MagimajyoPures.hpp"
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
#include "lgfx_ja3_ayug.h"
#endif

static AudioOutputI2S out(0, AudioOutputI2S::EXTERNAL_I2S, 10);
static FmPlapla radio(&out, APP_CPU_NUM);
static WebServer httpd(80);
static String stationName;
static String programName;

#ifdef _LCD_MAGIMAJYO_PURES_HPP_
#ifdef CONFIG_IDF_TARGET_ESP32S3
static LCD_MagimajoPures lcd(48, 47, 45, 3, 46, 9, 10, 11, 12, 13, 14, 21, 8);
#else
static LCD_MagimajoPures lcd(13, 12, 27, 23, 21, 19, 18, 5, 4, 2, 15, 14, 33);
#endif
static lgfx::LGFX_Sprite sprite; // [640 * 48] Buffer
static const lgfx::U8g2font font_L = { lgfx_ja3_ayug_20 } ;
#endif

void setup() {
  Serial.begin(115200);
#ifdef CONFIG_IDF_TARGET_ESP32S3
  out.SetPinout(15, 6, 7);    // bck, lrc, dout
  pinMode( 4, OUTPUT); digitalWrite( 4, LOW);   // XSMT
  pinMode( 5, OUTPUT); digitalWrite( 5, LOW);   // FMT
  pinMode(16, OUTPUT); digitalWrite(16, LOW);   // SCK
  pinMode(17, OUTPUT); digitalWrite(17, LOW);   // FLT
  pinMode(18, OUTPUT); digitalWrite(18, LOW);   // DEMP
#else
//out.SetPinout(26, 25, 22);  // bck, lrc, dout
#endif
  
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
  pinMode( 0, INPUT_PULLUP);                    // BackLight
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
  for(int count = 0; WiFi.status() != WL_CONNECTED; count++) {
    if(count > 300)
      ESP.restart();
    Serial.print(".");
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
    sprite.print(".");
    lcd.writeBuffer(sprite);
#endif
    delay(100);
  }
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());  
#ifdef _LCD_MAGIMAJYO_PURES_HPP_
  sprite.clear();
  sprite.setCursor(0, 0);
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
      "<title>WebRadio_FmPlaPla</title>"
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
      "<title>WebRadio_FmPlaPla</title>"
      "<meta charset=\"UTF-8\">"
      "<script type=\"text/javascript\">"
      "  function  sel(index) {"
      "    form.station.value = index;"
      "    form.submit();"
      "  }"
      "</script>"
      "</head>"
      "<body>"
      "<h2>WebRadio_FmPlaPla</h2>"
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
      "<title>WebRadio_FmPlaPla</title>"
      "<meta charset=\"UTF-8\">"
      "</head>"
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
#ifdef CONFIG_IDF_TARGET_ESP32S3
  digitalWrite( 4, HIGH);  // XSMT
#endif
}

void loop() {
  ArduinoOTA.handle();
  httpd.handleClient();
  radio.handle();
}