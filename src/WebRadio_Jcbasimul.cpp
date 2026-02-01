/*
 * https://twitter.com/wakwak_koba/
 */

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceJcbasimul.hpp"
#include "WebRadio_Jcbasimul.h"

static constexpr const char* station_list[][2] =
{
/*00*/	{"ＦＭはな / 北海道","fmhana"},
/*01*/	{"エフエムもえる / 北海道","moeru"},
/*02*/	{"Ａｉｒてっし / 北海道","airtesshi"},
/*03*/	{"ラジオニセコ / 北海道","radioniseko"},
/*04*/	{"FMいるか / 北海道","iruka"},
/*05*/	{"FMびゅー / 北海道","muroran"},
/*06*/	{"FMとまこまい / 北海道","fmtomakomai"},
/*07*/	{"FMくりやま / 北海道","fmkuriyama"},
/*08*/	{"FM AZUR / 青森県","fmazur"},
/*09*/	{"アップルウェーブ / 青森県","applewave"},
/*10*/	{"FMごしょがわら / 青森県","fmgoshogawara"},
/*11*/	{"FMONE / 岩手県","fmone"},
/*12*/	{"エフエムいわぬま / 宮城県","fmiwanuma"},
/*13*/	{"Ｈ＠！ＦＭ  / 宮城県","hatfm"},
/*14*/	{"ラジオ モンスター / 山形県","radiomonster"},
/*15*/	{"エフエムNCV / 山形県","yonezawancvfm"},
/*16*/	{"えふえむい～じゃんおらんだらじお / 山形県","orandaradio"},
/*17*/	{"ハーバーラジオ / 山形県","harborradio"},
/*18*/	{"ウルトラＦＭ / 福島県","ultrafm"},
/*19*/	{"ＦＭポコ / 福島県","fmpoco"},
/*20*/	{"エフエムきたかた / 福島県","fmkitakata"},
/*21*/	{"FM愛'S / 福島県","fmaizu"},
/*22*/	{"ＦＭだいご / 茨城県","fmdaigo"},
/*23*/	{"FMかしま / 茨城県","fmkashima"},
/*24*/	{"ラジオ高崎 / 群馬県","radiotakasaki"},
/*25*/	{"エフエム太郎 / 群馬県","fmtaro"},
/*26*/	{"ＦＭ ＯＺＥ / 群馬県","fmoze"},
/*27*/	{"ラヂオななみ / 群馬県","radionanami"},
/*28*/	{"ＦＭチャッピー / 埼玉県","fmchappy"},
/*29*/	{"発するFM / 埼玉県","miyoshifm"},
/*30*/	{"ラジオ川越 / 埼玉県","radiokawagoe"},
/*31*/	{"市川うららFM(I&U-LaLaFM) / 千葉県","ulalafm"},
/*32*/	{"かずさFM / 千葉県","kazusafm"},
/*33*/	{"ラジオ成田 / 千葉県","radionarita"},
/*34*/	{"FMふくろう / 千葉県","fmfukuro"},
/*35*/	{"SKYWAVE FM / 千葉県","skywavefm"},
/*36*/	{"ＦＭえどがわ / 東京都","fmedogawa"},
/*37*/	{"むさしのＦＭ / 東京都","musashinofm"},
/*38*/	{"FMしながわ / 東京都","fmshinagawa"},
/*39*/	{"かつしかFM / 東京都","fmkatsushika"},
/*40*/	{"渋谷のラジオ / 東京都","shibuyanoradio"},
/*41*/	{"中央エフエム・RADIO CITY / 東京都","radiocity"},
/*42*/	{"コマラジ / 東京都","komaraji"},
/*43*/	{"関東臨時災害放送局訓練 / 東京都","rinsaikanto"},
/*44*/	{"ＦＭブルー湘南 / 神奈川県","fmblueshonan"},
/*45*/	{"鎌倉FM / 神奈川県","kamakurafm"},
/*46*/	{"FM湘南ナパサ / 神奈川県","fmshonan"},
/*47*/	{"FMおだわら / 神奈川県","fmodawara"},
/*48*/	{"FM湘南マジックウェイブ / 神奈川県","magicwave"},
/*49*/	{"FMやまと / 神奈川県","fmyamato"},
/*50*/	{"レディオ湘南 / 神奈川県","radioshonan"},
/*51*/	{"FMサルース / 神奈川県","fmsalus"},
/*52*/	{"マリンFM / 神奈川県","marinefm"},
/*53*/	{"エフエム戸塚 / 神奈川県","fmtotsuka"},
/*54*/	{"エボラジ / 神奈川県","chigasakifm"},
/*55*/	{"エフエム甲府 / 山梨県","fmkofu"},
/*56*/	{"FMふじやま / 山梨県","fmfujiyama"},
/*57*/	{"エフエム　ふじごこ / 山梨県","fmfujigoko"},
/*58*/	{"FM八ヶ岳 / 山梨県","fmyatsugatake"},
/*59*/	{"ラジオチャット・FMにいつ / 新潟県","radiochat"},
/*60*/	{"FMうおぬま / 新潟県","fmuonuma"},
/*61*/	{"エフエムながおか / 新潟県","fmnagaoka"},
/*62*/	{"エフエムしばた / 新潟県","fmshibata"},
/*63*/	{"FM KENTO / 新潟県","fmkento"},
/*64*/	{"ＦＭゆきぐに / 新潟県","fmyukiguni"},
/*65*/	{"FMじょうえつ / 新潟県","fmjyoetsu"},
/*66*/	{"FMピッカラ / 新潟県","fmpikkara"},
/*67*/	{"LCV FM / 長野県","lovefm"},
/*68*/	{"FM軽井沢 / 長野県","fmkaruizawa"},
/*69*/	{"高ボッチ高原FM / 長野県","shiojirifm"},
/*70*/	{"伊那谷FM / 長野県","inadanifm"},
/*71*/	{"ＦＭＰｉＰｉ / 岐阜県","fmpipi"},
/*72*/	{"ＦＭわっち / 岐阜県","fmwatch"},
/*73*/	{"Ｈｉｔｓ ＦＭ  / 岐阜県","hitsfm"},
/*74*/	{"FM Haro! / 静岡県","haro"},
/*75*/	{"FM ISみらいずステーション / 静岡県","fmis"},
/*76*/	{"g-sky76.5 / 静岡県","fmshimada"},
/*77*/	{"富士山ＧＯＧＯＦＭ / 静岡県","fujiyamagogofm"},
/*78*/	{"ボイスキュー / 静岡県","voicecue"},
/*79*/	{"Radio-f / 静岡県","radiof"},
/*80*/	{"COAST-FM76.7MH z / 静岡県","coastfm"},
/*81*/	{"エフエムなぎさステーション / 静岡県","nagisastation"},
/*82*/	{"Ciao! / 静岡県","ciao"},
/*83*/	{"FMいずのくに / 静岡県","fmizunokuni"},
/*84*/	{"RADIO LUSH / 静岡県","fmyaizu"},
/*85*/	{"S-Wave / 静岡県","swave"},
/*86*/	{"エフエム　ななみ / 愛知県","fmnanami"},
/*87*/	{"United North / 愛知県","unitednorth"},
/*88*/	{"RADIO SANQ  / 愛知県","radiosanq"},
/*89*/	{"i-wave / 愛知県","fmichinomiya"},
/*90*/	{"HeartFM / 愛知県","heartfm"},
/*91*/	{"いなべエフエム / 三重県","inabefm"},
/*92*/	{"Suzuka Voice FM 78.3MHz / 三重県","suzuka"},
/*93*/	{"富山シティエフエム株式会社 / 富山県","toyamacityfm"},
/*94*/	{"エフエムとなみ / 富山県","fmtonami"},
/*95*/	{"ラジオたかおか / 富山県","radiotakaoka"},
/*96*/	{"ラジオこまつ / 石川県","radiokomatsu"},
/*97*/	{"ラジオななお / 石川県","radionanao"},
/*98*/	{"ラジオかなざわ / 石川県","radiokanazawa"},
/*99*/	{"えふえむ草津 / 滋賀県","fmkusatsu"},
/*100*/	{"ＦＭいかる / 京都府","fmikaru"},
/*101*/	{"FMうじ / 京都府","fmuji"},
/*102*/	{"FMまいづる / 京都府","fmmaizuru"},
/*103*/	{"FM845 / 京都府","kyotoribingufm"},
/*104*/	{"FM千里 / 大阪府","fmsenri"},
/*105*/	{"ウメダFM Be Happy!789 / 大阪府","umedafm"},
/*106*/	{"タッキー816みのおエフエム / 大阪府","minofm"},
/*107*/	{"エフエムいたみ / 兵庫県","fmitami"},
/*108*/	{"ハミングFM宝塚 / 兵庫県","fmtakarazuka"},
/*109*/	{"さくらFM / 兵庫県","sakurafm"},
/*110*/	{"エフエムみっきぃ / 兵庫県","fmmiki"},
/*111*/	{"805たんば / 兵庫県","tanba"},
/*112*/	{"FM GENKI / 兵庫県","fmgenki"},
/*113*/	{"なら どっと ＦＭ / 奈良県","narafm"},
/*114*/	{"ＦＭ五條 / 奈良県","fmgojo"},
/*115*/	{"FMまほろば / 奈良県","fmmahoroba"},
/*116*/	{"バナナエフエム / 和歌山県","bananafm"},
/*117*/	{"FM TANABE / 和歌山県","fmtanabe"},
/*118*/	{"FMはしもと / 和歌山県","fmhashimoto"},
/*119*/	{"FMビーチステーション / 和歌山県","beachstation"},
/*120*/	{"FM新宮 / 和歌山県","fmshingu"},
/*121*/	{"レディオ モモ / 岡山県","radiomomo"},
/*122*/	{"FMくらしき / 岡山県","fmkurashiki"},
/*123*/	{"FMふくやま / 広島県","bingo"},
/*124*/	{"FMおのみち / 広島県","fmonomichi"},
/*125*/	{"FMちゅーピー / 広島県","fmchupea"},
/*126*/	{"FMはつかいち / 広島県","fmhatsukaichi"},
/*127*/	{"FM東広島 / 広島県","fmhigashihiroshima"},
/*128*/	{"FOR LIFE RADIO / 広島県","fmmihara"},
/*129*/	{"ＣＯＭＥ ＯＮ ! ＦＭ / 山口県","comeonfm"},
/*130*/	{"しゅうなんＦＭ / 山口県","shunanfm"},
/*131*/	{"RADIO BIRD / 鳥取県","radiobird"},
/*132*/	{"エフエム・サン / 香川県","fmsun"},
/*133*/	{"FMラヂオバリバリ / 愛媛県","fmradiobaribari"},
/*134*/	{"FMがいや  / 愛媛県","fmgaiya"},
/*135*/	{"Hello! NEW 新居浜 FM / 愛媛県","niihamafm"},
/*136*/	{"Dreams FM / 福岡県","dreamsfm"},
/*137*/	{"FM八女 / 福岡県","fmyame"},
/*138*/	{"ＦＭからつ / 佐賀県","fmkaratsu"},
/*139*/	{"Kappa FM / 熊本県","fmyatsushiro"},
/*140*/	{"FM791 / 熊本県","kumamotocityfm"},
/*141*/	{"ゆふいんラヂオ局 / 大分県","yufuin"},
/*142*/	{"NOASFM / 大分県","noasfm"},
};
static byte initial_station = 105;   // ウメダFM Be Happy!789

AudioGenerator * Jcbasimul :: station_t :: getDecoder() {
  auto jcba = (Jcbasimul *)radio;
  auto decoder = new AudioGeneratorOpus();
  decoder->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
  decoder->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
  return decoder;
}

AudioFileSourceWebSockets * Jcbasimul :: station_t :: getSource() {
  auto jcba = (Jcbasimul *)radio;
  AudioFileSourceWebSockets * source;
  if(jcba->buffer)
    source = new AudioFileSourceJcbasimul(jcba->buffer, jcba->bufferSize);
  else
    source = new AudioFileSourceJcbasimul(jcba->bufferSize);
  source->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
  source->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
  
  auto url = getUrl();
  if(!source->open(url.c_str())) {
    delete source;
    source = nullptr;
    url = "error: " + url;
    getJcbasimul()->sendLog(url.c_str(), true);
//          delay(5000);
  } else
    getJcbasimul()->sendLog(url.c_str());
  
  return source;
}

bool Jcbasimul :: begin() {
  for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
    stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
  defaultStationIdx = initial_station;

  auto psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  if(!psram) {
    onError("Requires PSRAM");
    return false;
  }
  
  if(!bufferSize)
    bufferSize = std::max((uint32_t)(5 * 1024), std::min((uint32_t)(128 * 1024), (uint32_t)psram));
  
  startTask();
  return true; 
}

void Jcbasimul :: stop() {
  current_station = nullptr;
  
  if(decoder) {
    stopDecode = 2;
    while(stopDecode == 2) {delay(100);}
  }
  
  if(source) {
    delete source;
    source = nullptr;
  }
  stopDecode = 0;
}

String Jcbasimul :: getInfoBuffer() {
  if(source)
    return source->getInfoBuffer();
  return "";
}

void Jcbasimul :: downloadTaskCore() {
  auto now_millis = millis();
  
  if (select_station) {
    if(select_station != current_station)
      saveSettings = millis() + 10000;
    stop();

    current_station = select_station;
    source = ((station_t *)current_station)->getSource();
    if(source) {
      decoder = ((station_t *)current_station)->getDecoder();
      restart(false);
    }
    else
      onSerious("getSource(): false");

    if(onPlay)
      onPlay(current_station->getName(), getIndex(current_station));

    select_station = nullptr;
  } else if (source && !source->isOpen()) {
    sendLog("source->isOpen(): false", true);
    delay(5000);
    select_station = current_station;
  } else if (source && source->isOpen()) {
    static auto last_report = now_millis;
    if(source->fill() && now_millis - last_report >= 5000) {
      auto writeSum = source->getTotalTransferred();
      auto text = ((station_t *)current_station)->getUrl() + " " + String((uint32_t)(writeSum / 1024)) + " KiB";
      sendLog(text.c_str());
      last_report = now_millis;
    }
  }
  
  if (saveSettings > 0 && now_millis > saveSettings && current_station) {
    saveStation(current_station);
    saveSettings = 0;
  }
}
    
void Jcbasimul :: decodeTaskCore() {
  auto now_millis = millis();
  static auto last_loop = now_millis;
  
  if(stopDecode > 0) {
    if(decoder) {
      if(decoder->isRunning())
        decoder->stop();
      delete decoder;
      decoder = nullptr;
    }
    stopDecode--;
    while(stopDecode) {delay(100);}
  }
  else if(!decoder) {
    if (current_station && now_millis - last_loop > 10000)
      onSerious("Streaming reception aborted");
  } else if(!decoder->isRunning()) {
    if(source->getSize() >= (bufferSize >> 1) ) {
      if(decoder->begin(source, out))
        last_loop = now_millis;
      else {
        sendLog("failed: decoder->begin(source, out)", true);
        select_station = current_station;
        delay(1000);
      }
    }
  } else if(source->getSize() > 0) {
    if (decoder->loop())
      last_loop = now_millis;
    else {
      sendLog("failed: decoder->loop()", true);
      delay(5000);
      select_station = current_station;
    }
  } else if (now_millis - last_loop > 10000) {
    sendLog("Streaming reception time out", true);
    decoder->stop();
    last_loop = now_millis;
    select_station = current_station;
  }
}

void Jcbasimul :: saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) {
  nvs_set_u8(nvs_handle, "jcbasimul", getIndex(station));
}

WebRadio::Station * Jcbasimul :: restoreStationCore(uint32_t nvs_handle) {
  uint8_t idx_8;
  uint32_t idx_32;
  if(nvs_get_u8(nvs_handle, "jcbasimul", &idx_8))
    if(!nvs_get_u32(nvs_handle, "station", &idx_32))
      idx_8 = idx_32;
    else
      return nullptr;

  if(idx_8 > getNumOfStations())
    idx_8 = getNumOfStations() - 1;
  return getStation(idx_8);
}
