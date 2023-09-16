# WebRadio_Jcbasimul / WebRadio_Radiko / WebRadio_ListenRadio / WebRadio_FmPlapla

### 機能
- [WebRadio_with_ESP8266Audio](https://github.com/m5stack/M5Unified/tree/master/examples/Advanced/WebRadio_with_ESP8266Audio) をベースにした、グラフィカルなインターフェースのコード例です。
- 選局と音量は不揮発性メモリに保存され、次回の起動時に初期採用されます。
- 旧来からの ESP32 に加えて ESP32-S3 にも対応しています。

![image1](/docs/screenshot.jpg)

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
- フラッシュ 4MB の場合は Partition Schema を No OTA もしくは Minimal SPIFFS にしてください。
- Jcbasimul / FM++ は常時 SSL でのストリーミングとなり、内蔵 SRAM だけでは足りず PSRAM を必要とします。
- arduino-esp32 v2.0.3 では PSRAM 有効にするとビルドが失敗します。
- arduino-esp32 v2.0.4 と Fire の組み合わせでは PSRAM を有効に出来ません。

#### arduino-esp32 の推奨バージョン
|ライブラリ|PSRAM無効|PSRAM有効(Fire)|PSRAM有効(Fire以外)|
|:--------:|:--------------------:|:--------------------:|:--------------------:|
|Jcbasimul|v2.0.2/v2.0.3|～v2.0.13(v2.0.3を除く)|～v2.0.13|
|Radiko|～v2.0.13|～v2.0.13(v2.0.3を除く)|～v2.0.13|
|ListenRadio|～v2.0.13|～v2.0.13(v2.0.3を除く)|～v2.0.13|
|FM++|v2.0.2/v2.0.3|～v2.0.13(v2.0.3を除く)|～v2.0.13|

## 使用方法
### WiFi設定
SDカードを用いて WiFi の設定が行えます。  
いったん接続に成功すれば、以降はSDカードは必要ありません。  
具体的な記述方法は [説明](sdcard/) をご覧ください。

![image1](/docs/wifi.png)

### Radiko premium（プレミアム会員）
WiFi設定と同じ要領で SDカードを用いて Radiko premium の認証情報を登録すると、全国の放送局が選局可能になります。  
ファイル名は radiko.txt で、1行目にユーザーID（メールアドレス）、2行目にパスワードを指定してください。  
いったん登録して起動させれば、認証情報は不揮発性メモリーに保存され、以降はSDカードは必要ありません。  

### 設定の初期化
Aボタン・Bボタン・Cボタンの3つを押しながら電源ON（もしくはリセット）すると以下の情報が初期化されます。
- 選局
- 音量
- Radiko premiun の認証情報

### ボタン動作
|ボタン|動作|
|:-------------:|:----:|
|Aボタン1回クリック|選局・次|
|Aボタン2回クリック|選局・前|
|Bボタン|音量・減|
|Cボタン|音量・増|

### タッチパネル
|操作|動作|
|:-------------:|:----:|
|下にフリック|選局・次|
|上にフリック|選局・前|
|左にスワイプ|音量・減|
|右にスワイプ|音量・増|

## 謝辞
かっこいいビジュアルは [lovyan03](https://github.com/lovyan03/) さんの制作です。  
ずっと見てても飽きないです！
