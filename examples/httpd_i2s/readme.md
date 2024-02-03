# httpd_Jcbasimul_i2s / httpd_Radiko_i2s / httpd_ListenRadio_i2s / httpd_FmPlapla

### 機能
- Jcbasimul / Radiko / ListenRadio / FM++ を受信し、I2S モジュールで出力します。
- 起動すると HTTPサーバー になり、ブラウザで選局できます。
- PSRAM:Enabled に最適化してあります。なくても動きますが、音の途切れが発生する場合があります。

### ビルドに必要なライブラリ
#### Jcbasimul / Radiko / ListenRadio / FM++ 共通
- [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- [wakwak-koba/ESP8266Audio](https://github.com/wakwak-koba/ESP8266Audio) forked from [earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
#### Jcbasimul / FM++ のみ
- [Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)
- [arduino-libraries/Arduino_JSON] は不要になりました

### 配線

|PCM5102Aモジュール|ESP32|ESP32S3|
|:----:|:----:|:----:|
|SCK|GND|GND|
|BCK|G26|G15|
|DIN|G22|G7|
|LCK|G25|G6|
|GND|GND|GND|
|VIN|3V3|3V3|

- 他のモジュールでも可能と思われます。
- 上記以外のピンを使用する場合は、スケッチ内 out.SetPinout() でピン番号を指定して下さい。
- ESP-WROOM32(PSRAMなし)、ESP32-WROVER-B(PSRAMあり) でテストしています。

![image1](/docs/i2s.JPG)
