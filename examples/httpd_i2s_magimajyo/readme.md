# httpd_Jcbasimul_i2s_magimajyo / httpd_Radiko_i2s_magimajyo / httpd_ListenRadio_i2s_magimajyo / httpd_FmPlapla_i2s_magimajyo

### 機能
- httpd_Jcbasimul_i2s / httpd_Radiko_i2s / httpd_ListenRadio_i2s / httpd_FmPlapla_i2s に加えて、マジョカアイリス を接続する前提で局名などが表示されます
- Radiko では AAC-SBR を有効にし高音質で再生します
- 液晶+SBR+Radikoプレミアム を全て有効にする場合には PSRAM を必要とします

### ビルドに必要なライブラリ
#### Jcbasimul / Radiko / ListenRadio / FM++ 共通
- [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- [Lovyan03/LovyanGFX](https://github.com/lovyan03/LovyanGFX)
- [wakwak-koba/ESP8266Audio](https://github.com/wakwak-koba/ESP8266Audio) forked from [earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
#### Jcbasimul / FM++ のみ
- [Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)
- [arduino-libraries/Arduino_JSON] は不要になりました

### 配線
|PCM5102Aモジュール|ESP32|ESP32S3|
|:----:|:----:|
|SCK|GND|GND|
|BCK|G26|G15|
|DIN|G22|G7|
|LCK|G25|G6|
|GND|GND|GND|
|VIN|3V3|3V3|
|XSMT|3V3|G4
|FMT|GND|G5|
|SCK|GND|G16|
|FLT|GND|G17|
|DEMP|GND|G18|
- ESP32S3 時の場合は拙作の WebRadioModule を前提にした配線を記載しましたが、XSMT/FMT/SCK/FLT/DEMP は ESP32 と同様の結線にしても問題ありません

|マジョカアイリス|信号名|ESP32|ESP32S3|
|:----:|:--:|:----:|
|1|GND|GND|GND|
|2|RST|G33|G8|
|3|GND|GND|GND|
|4|D0|G23|G3|
|5|D1|G21|G46|
|6|D2|G19|G9|
|7|D3|G18|G10|
|8|D4|G5|G11|
|9|D5|G4|G12|
|10|D6|G2|G13|
|11|D7|G15|G14|
|12|CS1|G14|G21|
|13|RD|G12|G47|
|14|WR|G13|G48|
|15|CS2|G14|G21|
|16|DC|G27|G45|
|18|VDD|3V3|3V3|
|19|VDDIO|3V3|3V3|
|20|LED+|||
|21|LED-|||
|22|GND|GND|GND|
- LED+／LED- はバックライト用で 5.5V～ が必要です。
- ESP32-WROVER-B / ESP32-S3-WROOM-1-N16R8 (ともにPSRAMあり)でテストしています。

![image1](/docs/magimajyo.JPG)
