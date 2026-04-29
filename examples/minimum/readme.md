# minimum_Jcbasimul / minimum_Radiko / minimum_ListenRadio / minimum_FmPlapla

### 機能
- Jcbasimu / Radiko / ListenRadio / FM++ を M5Stack シリーズ（Gray/Fire/Core2/CoreS3等）の内蔵スピーカーで聴くための最小コード例です。

### ビルドに必要なライブラリ
#### Jcbasimul / Radiko / ListenRadio / FM++ 共通
- [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- [m5stack/M5GFX](https://github.com/m5stack/M5GFX)
- [m5stack/M5Unified](https://github.com/m5stack/M5Unified)
- [wakwak-koba/ESP8266Audio](https://github.com/wakwak-koba/ESP8266Audio) forked from earlephilhower/ESP8266Audio または [earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
#### Jcbasimul / FM++ のみ
- [Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)

### ビルド時の注意
- フラッシュ 4MB で Jcbasimal をビルドする場合は Partition Schema を No OTA もしくは Minimal SPIFFS にしてください。
- Jcbasimul / FM++ は常時 SSL でのストリーミングとなり、内蔵 SRAM だけでは足りず PSRAM を必要とします。
- 本家の(私のFork版でない)[earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) を使う場合は AAC-SBR が常時 ON になり、Radiko / ListenRadio でも PSRAM を必要とします。
