# minimum_Jcbasimul / minimum_Radiko

### 機能
- Jcbasimu / Radiko を M5Stack シリーズ（Gray/Fire/Core2等）の内蔵スピーカーで聴くための最小コード例です。

### ビルドに必要なライブラリ
#### Jcbasimul / Radiko 共通
- [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- [m5stack/M5GFX](https://github.com/m5stack/M5GFX)
- [m5stack/M5Unified](https://github.com/m5stack/M5Unified)
- [wakwak-koba/ESP8266Audio](https://github.com/wakwak-koba/ESP8266Audio) forked from [earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
#### Jcbasimul のみ
- [Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)
- [arduino-libraries/Arduino_JSON](https://github.com/arduino-libraries/Arduino_JSON)

### ビルド時の注意
- フラッシュ 4MB で Jcbasimal をビルドする場合は Partition Schema を No OTA もしくは Minimal SPIFFS にしてください。
- arduino-esp32 v2.0.3 では PSRAM 有効にするとビルドが失敗します。
- arduino-esp32 v2.0.4 と Fire の組み合わせでは PSRAM を有効に出来ません。

#### arduino-esp32 の推奨バージョン
|ライブラリ|PSRAM無効|PSRAM有効(Fire)|PSRAM有効(Fire以外)|
|:--------:|:--------------------:|:--------------------:|:--------------------:|
|Jcbasimul|v2.0.2/v2.0.3|v2.0.2/v2.0.5|v2.0.2/v2.0.4/v2.0.5|
|Radiko|v2.0.2/v2.0.3/v2.0.5|v2.0.2/v2.0.5|v2.0.2/v2.0.4/v2.0.5|
