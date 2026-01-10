/*
 * https://twitter.com/wakwak_koba/
 */

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceFmPlapla.hpp"
#include "WebRadio_FmPlapla.h"

// https://fmplapla.com/api/select_stream?station=radioodate&burst=5
static constexpr const char* station_list[][2] =
{
/*00*/	{"FMラジオおおだて / 秋田県大館市","radioodate"},
/*01*/	{"FMはなび / 秋田県大仙市","fmhanabi"},
/*02*/	{"きたかみE&amp;Beエフエム / 岩手県北上市","iianbefm"},
/*03*/	{"FMねまらいん / 岩手県大船渡市","fmnemaline"},
/*04*/	{"FMあすも / 岩手県一関市","fmasmo"},
/*05*/	{"ＯＣＲFM835 / 宮城県大崎市","bikkifm"},
/*06*/	{"ラヂオは〜と / 新潟県燕市","radioheart"},
/*07*/	{"FMとおかまち / 新潟県十日町市","fmtokamachi"},
/*08*/	{"FMみょうこう / 新潟県妙高市","fmmyoko"},
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
/*20*/	{"ほんじょうFM / 埼玉県本庄市","honjofm"},
/*21*/	{"FMクマガヤ / 埼玉県熊谷市","fmkumagaya"},
/*22*/	{"ちちぶFM / 埼玉県秩父市","chichibufm"},
/*23*/	{"TOKYO854 くるめラ / 東京都東久留米市","fmhigashikurume"},
/*24*/	{"エフエム西東京 / 東京都西東京市","fmnishitokyo"},
/*25*/	{"ラジオフチューズ / 東京都府中市","radiofuchues"},
/*26*/	{"FM大師 / 神奈川県川崎市川崎区","fmdaishi"},
/*27*/	{"金沢シーサイドFM / 神奈川県横浜市金沢区","kanazawaseasidefm"},
/*28*/	{"78.2エフエムひこね / 滋賀県彦根市","fmhikone"},
/*29*/	{"FMおおつ / 滋賀県大津市","fmotsu"},
/*30*/	{"みんなのあま咲き放送局 / 兵庫県尼崎市","amasakifm"},
/*31*/	{"YES-fm / 大阪府大阪市中央区","yesfm"},
/*32*/	{"FMいずみおおつ / 大阪府泉大津市","fmizumiotsu"},
/*33*/	{"FMヤマト / 奈良県大和高田市","fmyamato"},
/*34*/	{"FMらら76.8 / 岐阜県可児市","fmrara768"},
/*35*/	{"名古屋市防災(MID-FM) / 愛知県名古屋市中区","nagoyabousai"},
/*36*/	{"ラジオ・ラブィート / 愛知県豊田市","radioloveat"},
/*37*/	{"メディアスFM / 愛知県東海市","mediasfm"},
/*38*/	{"KATCH&amp;Pitch 地域情報 / 愛知県刈谷市","pitchfm"},
/*39*/	{"TEES-843FM / 愛知県豊橋市","tees-843fm"},
/*40*/	{"CTY-FM / 三重県四日市市","fmyokkaichi"},
/*41*/	{"ゆめウェーブ / 岡山県笠岡市","fmyumewave"},
/*42*/	{"FMわっしょい / 山口県防府市","fmwassyoi"},
/*43*/	{"FMスマイルウェ～ブ / 山口県山陽小野田市","fmsunsunkirara"},
/*44*/	{"FMはたらんど / 高知県四万十市","fmhataland"},
/*45*/	{"FM KITAQ / 福岡県北九州市","fmkitaq"},
/*46*/	{"FMたんと / 福岡県大牟田市","fmtanto"},
/*47*/	{"えびすFM / 佐賀県佐賀市","ebisufm"},
/*48*/	{"みつばちラジオ / 熊本県天草市","mitsubachiradio"},
/*49*/	{"エフエム対馬 / 長崎県対馬市","fmtsushima"},
/*50*/	{"FMさせぼ / 長崎県佐世保市","happyfm"},
/*51*/	{"エフエム諫早 / 長崎県諫早市","fmisahaya"},
/*52*/	{"FMしまばら / 長崎県島原市","fmshimabara"},
/*53*/	{"FMひまわり / 長崎県南島原市","fmhimawari"},
/*54*/	{"サンシャインFM / 宮崎県宮崎市","sunshinefm"},
/*55*/	{"FMさつませんだい / 鹿児島県薩摩川内市","fmsatsumasendai"},
/*56*/	{"FMきりしま / 鹿児島県霧島市","fmkirishima"},
/*57*/	{"フレンズFM / 鹿児島県鹿児島市","friendsfm"},
/*58*/	{"FMぎんが / 鹿児島県鹿児島市","fmginga"},
/*59*/	{"FMかのや / 鹿児島県鹿屋市","fmkanoya"},
/*60*/	{"FMもとぶ / 沖縄県本部町","fmmotobu"},
/*61*/	{"FMうるま / 沖縄県うるま市","fmuruma"},
/*62*/	{"FMくめじま / 沖縄県久米島町","fmkumejima"},
/*63*/	{"FMみやこ / 沖縄県宮古島市","fmmiyako"},
};
static byte initial_station = 63;   // FMみやこ

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
