# minimum_Jcbasimul / minimum_Radiko / minimum_ListenRadio / minimum_FmPlapla

### 機能
- Jcbasimu / Radiko / ListenRadio / FM++ を M5Stack シリーズ（Gray/Fire/Core2/CoreS3等）の内蔵スピーカーで聴くための最小コード例です。

### ビルドに必要なライブラリ
#### Jcbasimul / Radiko / ListenRadio / FM++ 共通
- [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- [m5stack/M5GFX](https://github.com/m5stack/M5GFX)
- [m5stack/M5Unified](https://github.com/m5stack/M5Unified)
- [wakwak-koba/ESP8266Audio](https://github.com/wakwak-koba/ESP8266Audio) forked from [earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
#### Jcbasimul / FM++ のみ
- [Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)
- [arduino-libraries/Arduino_JSON] は不要になりました

### ビルド時の注意
- フラッシュ 4MB で Jcbasimal をビルドする場合は Partition Schema を No OTA もしくは Minimal SPIFFS にしてください。
- Jcbasimul / FM++ は常時 SSL でのストリーミングとなり、内蔵 SRAM だけでは足りず PSRAM を必要とします。
- arduino-esp32 v2.0.3 では PSRAM 有効にするとビルドが失敗します。
- arduino-esp32 v2.0.4 と Fire の組み合わせでは PSRAM を有効に出来ません。

#### arduino-esp32 の推奨バージョン
|ライブラリ|PSRAM無効|PSRAM有効(Fire)|PSRAM有効(Fire以外)|
|:--------:|:--------------------:|:--------------------:|:--------------------:|
|Jcbasimul|v2.0.2/v2.0.3|～v3.0.4(v2.0.3を除く)|～v3.0.4|
|Radiko|～v3.0.4|～v3.0.4(v2.0.3を除く)|～v3.0.4|
|ListenRadio|～v3.0.4|～v3.0.4(v2.0.3を除く)|～v3.0.4|
|FM++|v2.0.2/v2.0.3|～v3.0.4(v2.0.3を除く)|～v3.0.4|
