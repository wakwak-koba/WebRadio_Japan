/*
 * https://twitter.com/wakwak_koba/
 */

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceFmPlapla.hpp"
#include "WebRadio_FmPlapla.h"

// https://fmplapla.com/api/select_stream?station=radioodate&burst=5
static constexpr const char* station_list[][2] =
{
/*00*/	{"FMはなび / 秋田県大仙市","fmhanabi"},
/*01*/	{"きたかみE&Beエフエム / 岩手県北上市","iianbefm"},
/*02*/	{"FMあすも / 岩手県一関市","fmasmo"},
/*03*/	{"ＯＣＲFM835 / 宮城県大崎市","bikkifm"},
/*04*/	{"ラヂオは〜と / 新潟県燕市","radioheart"},
/*05*/	{"FMとおかまち / 新潟県十日町市","fmtokamachi"},
/*06*/	{"FMみょうこう / 新潟県妙高市","fmmyoko"},
/*07*/	{"FMぜんこうじ / 長野県長野市","fmzenkouji"},
/*08*/	{"あづみ野エフエム / 長野県安曇野市","azuminofm"},
/*09*/	{"はれラジ / 長野県東御市","fmtomi"},
/*10*/	{"fmさくだいら / 長野県佐久市","fmsakudaira"},
/*11*/	{"FMまつもと / 長野県松本市","fmmatsumoto"},
/*12*/	{"いいだFM / 長野県飯田市","iidafm"},
/*13*/	{"ミヤラジ / 栃木県宇都宮市","miyaradi"},
/*14*/	{"FMもおか / 栃木県真岡市","fmmoka"},
/*15*/	{"FMゆうがお / 栃木県下野市","fmyugao"},
/*16*/	{"FMくらら857 / 栃木県栃木市","fmkulala857"},
/*17*/	{"FM DAMONO / 栃木県足利市","fmdamono"},
/*18*/	{"おーラジ / 栃木県小山市","oradi"},
/*19*/	{"FM桐生 / 群馬県桐生市","fmkiryu"},
/*20*/	{"FMクマガヤ / 埼玉県熊谷市","fmkumagaya"},
/*21*/	{"ちちぶFM / 埼玉県秩父市","chichibufm"},
/*22*/	{"あいラジ / 埼玉県上尾市","airadi"},
/*23*/	{"TOKYO854 くるめラ / 東京都東久留米市","fmhigashikurume"},
/*24*/	{"Hello FM / 東京都福生市","hellofm"},
/*25*/	{"エフエム西東京 / 東京都西東京市","fmnishitokyo"},
/*26*/	{"ラジオTAMAリバー / 東京都府中市","radiofuchues"},
/*27*/	{"かわさきエフエム / 神奈川県川崎市","kawasakifm"},
/*28*/	{"FM大師 / 神奈川県川崎市川崎区","fmdaishi"},
/*29*/	{"金沢シーサイドFM / 神奈川県横浜市金沢区","kanazawaseasidefm"},
/*30*/	{"びわこの東のFM / 滋賀県彦根市","fmhikone"},
/*31*/	{"FMおおつ / 滋賀県大津市","fmotsu"},
/*32*/	{"FM BLOOM(あま咲き放送局) / 兵庫県尼崎市","amasakifm"},
/*33*/	{"YES-fm / 大阪府大阪市中央区","yesfm"},
/*34*/	{"やおFM / 大阪府八尾市","yaofm"},
/*35*/	{"FMいずみおおつ / 大阪府泉大津市","fmizumiotsu"},
/*36*/	{"奥河内ラジオ / 大阪府河内長野市","okukawachi"},
/*37*/	{"FMハイホー / 奈良県王寺町","fmhaiho"},
/*38*/	{"FMヤマト / 奈良県大和高田市","fmyamato"},
/*39*/	{"FMらら76.8 / 岐阜県可児市","fmrara768"},
/*40*/	{"名古屋市防災(MID-FM) / 愛知県名古屋市中区","nagoyabousai"},
/*41*/	{"ラジオ・ラブィート / 愛知県豊田市","radioloveat"},
/*42*/	{"メディアスFM / 愛知県東海市","mediasfm"},
/*43*/	{"KATCH&Pitch 地域情報 / 愛知県刈谷市","pitchfm"},
/*44*/	{"TEES-843FM / 愛知県豊橋市","tees-843fm"},
/*45*/	{"CTY-FM / 三重県四日市市","fmyokkaichi"},
/*46*/	{"ゆめウェーブ / 岡山県笠岡市","fmyumewave"},
/*47*/	{"FMわっしょい / 山口県防府市","fmwassyoi"},
/*48*/	{"FMスマイルウェ～ブ / 山口県山陽小野田市","fmsunsunkirara"},
/*49*/	{"FMはたらんど / 高知県四万十市","fmhataland"},
/*50*/	{"FM KITAQ / 福岡県北九州市","fmkitaq"},
/*51*/	{"COMIxTEN / 福岡県福岡市","comiten"},
/*52*/	{"FMたんと / 福岡県大牟田市","fmtanto"},
/*53*/	{"えびすFM / 佐賀県佐賀市","ebisufm"},
/*54*/	{"エフエムきつき802 / 大分県杵築市","fmkitsuki"},
/*55*/	{"みつばちラジオ / 熊本県天草市","mitsubachiradio"},
/*56*/	{"エフエム対馬 / 長崎県対馬市","fmtsushima"},
/*57*/	{"FMさせぼ / 長崎県佐世保市","happyfm"},
/*58*/	{"Silk FM / 長崎県大村市","fmomura"},
/*59*/	{"エフエム諫早 / 長崎県諫早市","fmisahaya"},
/*60*/	{"FMしまばら / 長崎県島原市","fmshimabara"},
/*61*/	{"FMひまわり / 長崎県南島原市","fmhimawari"},
/*62*/	{"サンシャインFM / 宮崎県宮崎市","sunshinefm"},
/*63*/	{"FMさつませんだい / 鹿児島県薩摩川内市","fmsatsumasendai"},
/*64*/	{"FMきりしま / 鹿児島県霧島市","fmkirishima"},
/*65*/	{"フレンズFM / 鹿児島県鹿児島市","friendsfm"},
/*66*/	{"FMかのや / 鹿児島県鹿屋市","fmkanoya"},
/*67*/	{"FMもとぶ / 沖縄県本部町","fmmotobu"},
/*68*/	{"FMうるま / 沖縄県うるま市","fmuruma"},
/*69*/	{"FMくめじま / 沖縄県久米島町","fmkumejima"},
/*70*/	{"FMみやこ / 沖縄県宮古島市","fmmiyako"},
};
static byte initial_station = 70;   // FMみやこ

AudioGenerator * FmPlapla :: station_t :: getDecoder() {
  auto jcba = (FmPlapla *)radio;
  auto decoder = new AudioGeneratorOpus();
  decoder->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
  decoder->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
  return decoder;
}

AudioFileSourceWebSockets * FmPlapla :: station_t :: getSource() {
  auto jcba = (FmPlapla *)radio;
  AudioFileSourceWebSockets * source;
  if(jcba->buffer)
    source = new AudioFileSourceFmPlapla(jcba->buffer, jcba->bufferSize);
  else
    source = new AudioFileSourceFmPlapla(jcba->bufferSize);
  source->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
  source->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
  
  auto url = getUrl();
  if(!source->open(url.c_str())) {
    delete source;
    source = nullptr;
    url = "error: " + url;
    getFmPlapla()->sendLog(url.c_str(), true);
//          delay(5000);
  } else
    getFmPlapla()->sendLog(url.c_str());
  
  return source;
}

bool FmPlapla :: begin() {
  for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
    stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
  defaultStationIdx = initial_station;

  auto psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  if(!psram) {
    onError("Requires PSRAM");
    return false;
  }

  if(!bufferSize)
    bufferSize = std::max((uint32_t)(5 * 1024), std::min((uint32_t)(64 * 1024), (uint32_t)psram));
  
  startTask();
  return true; 
}

void FmPlapla :: stop() {
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

void FmPlapla :: downloadTaskCore() {
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

void FmPlapla :: decodeTaskCore() {
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
      if(decoder->begin(source, out)) {
        sendLog("success: decoder->begin(source, out)", true);
        last_loop = now_millis;
      } else {
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

void FmPlapla :: saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) {
  nvs_set_u8(nvs_handle, "FmPlapla", getIndex(station));
}

WebRadio::Station * FmPlapla :: restoreStationCore(uint32_t nvs_handle) {
  uint8_t idx_8;
  uint32_t idx_32;
  if(nvs_get_u8(nvs_handle, "FmPlapla", &idx_8))
    if(!nvs_get_u32(nvs_handle, "station", &idx_32))
      idx_8 = idx_32;
    else
      return nullptr;

  if(idx_8 > getNumOfStations())
    idx_8 = getNumOfStations() - 1;
  return getStation(idx_8);
}
