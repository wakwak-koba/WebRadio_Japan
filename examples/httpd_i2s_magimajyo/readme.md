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
|PCM5102Aモジュール|ESP32|
|:----:|:----:|
|SCK|GND|
|BCK|G26|
|DIN|G22|
|LCK|G25|
|GND|GND|
|VIN|3V3|


|マジョカアイリス|信号名|ESP32|
|:----:|:--:|:----:|
|1|GND|GND|
|2|RST|G33|
|3|GND|GND|
|4|D0|G23|
|5|D1|G21|
|6|D2|G19|
|7|D3|G18|
|8|D4|G5|
|9|D5|G4|
|10|D6|G2|
|11|D7|G15|
|12|CS1|G14|
|13|RD|G12|
|14|WR|G13|
|15|CS2|G14|
|16|DC|G27|
|18|VDD|3V3|
|19|VDDIO|3V3|
|20|LED+||
|21|LED-||
|22|GND|GND|
- LED+／LED- はバックライト用で 5.5V～ が必要です。
- ESP32-WROVER-B(PSRAMあり) でテストしています。

![image1](/docs/magimajyo.JPG)
