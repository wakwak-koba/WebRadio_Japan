// https://twitter.com/wakwak-koba/

#include <WiFi.h>
#include <WebRadio_ListenRadio.h>
#include <AudioOutputI2S.h>
#include <ArduinoOTA.h>
#include <WebServer.h>

static AudioOutputI2S out(0, AudioOutputI2S::EXTERNAL_I2S, 10);
static ListenRadio radio(&out, APP_CPU_NUM);
static WebServer httpd(80);
static String stationName;

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

  WiFi.begin();
  for(int count = 0; WiFi.status() != WL_CONNECTED; count++) {
    if(count > 300)
      ESP.restart();
    Serial.print(".");
    delay(100);
  }
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());  

  ArduinoOTA.onStart([]() {
    radio.stop();
    out.stop();
  });
  ArduinoOTA.begin();

  radio.onPlay = [](const char * station_name, const size_t station_idx) {
    Serial.printf("onPlay:%d %s\n", station_idx, station_name);
    stationName = String(station_name);
  };
  radio.onInfo = [](const char * text) {
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
      "<title>WebRadio_ListenRadio</title>"
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
      "<title>WebRadio_ListenRadio</title>"
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
