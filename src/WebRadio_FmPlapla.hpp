/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_FmPlapla_HPP_
#define _WAKWAK_KOBA_WEBRADIO_FmPlapla_HPP_

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceFmPlapla.hpp"
#include "WebRadio.h"

// https://fmplapla.com/api/select_stream?station=radioodate&burst=5
static constexpr const size_t decode_stack_size = 16*1024;
static constexpr const char* station_list[][2] =
{
/*00*/	{"FMラジオおおだて / 秋田県大館市","radioodate"},
/*01*/	{"FMはなび / 秋田県大仙市","fmhanabi"},
/*02*/	{"きたかみE&Beエフエム / 岩手県北上市","iianbefm"},
/*03*/	{"FMねまらいん / 岩手県大船渡市","fmnemaline"},
/*04*/	{"FMあすも / 岩手県一関市","fmasmo"},
/*05*/	{"ＯＣＲFM835 / 宮城県大崎市","bikkifm"},
/*06*/	{"FMあおぞら / 宮城県亘理町","fmaozora"},
/*07*/	{"ラヂオは〜と / 新潟県燕市","radioheart"},
/*08*/	{"FMとおかまち / 新潟県十日町市","fmtokamachi"},
/*09*/	{"FMみょうこう / 新潟県妙高市","fmmyoko"},
/*10*/	{"はれラジ / 長野県東御市","fmtomi"},
/*11*/	{"fmさくだいら / 長野県佐久市","fmsakudaira"},
/*12*/	{"FMまつもと / 長野県松本市","fmmatsumoto"},
/*13*/	{"いいだFM / 長野県飯田市","iidafm"},
/*14*/	{"ミヤラジ / 栃木県宇都宮市","miyaradi"},
/*15*/	{"FMもおか / 栃木県真岡市","fmmoka"},
/*16*/	{"FMゆうがお / 栃木県下野市","fmyugao"},
/*17*/	{"FMくらら857 / 栃木県栃木市","fmkulala857"},
/*18*/	{"おーラジ / 栃木県小山市","oradi"},
/*19*/	{"FM桐生 / 群馬県桐生市","fmkiryu"},
/*20*/	{"ほんじょうFM / 埼玉県本庄市","honjofm"},
/*21*/	{"FMクマガヤ / 埼玉県熊谷市","fmkumagaya"},
/*22*/	{"ちちぶFM / 埼玉県秩父市","chichibufm"},
/*23*/	{"TOKYO854 くるめラ / 東京都東久留米市","fmhigashikurume"},
/*24*/	{"エフエム西東京 / 東京都西東京市","fmnishitokyo"},
/*25*/	{"ラジオフチューズ / 東京都府中市","radiofuchues"},
/*26*/	{"金沢シーサイドFM / 神奈川県横浜市金沢区","kanazawaseasidefm"},
/*27*/	{"78.2エフエムひこね / 滋賀県彦根市","fmhikone"},
/*28*/	{"FMおおつ / 滋賀県大津市","fmotsu"},
/*29*/	{"YES-fm / 大阪府大阪市中央区","yesfm"},
/*30*/	{"FMいずみおおつ / 大阪府泉大津市","fmizumiotsu"},
/*31*/	{"FMヤマト / 奈良県大和高田市","fmyamato"},
/*32*/	{"FMらら76.8 / 岐阜県可児市","fmrara768"},
/*33*/	{"名古屋市防災(MID-FM) / 愛知県名古屋市中区","nagoyabousai"},
/*34*/	{"ラジオ・ラブィート / 愛知県豊田市","radioloveat"},
/*35*/	{"メディアスFM / 愛知県東海市","mediasfm"},
/*36*/	{"KATCH&Pitch 地域情報 / 愛知県刈谷市","pitchfm"},
/*37*/	{"TEES-843FM / 愛知県豊橋市","tees-843fm"},
/*38*/	{"CTY-FM / 三重県四日市市","fmyokkaichi"},
/*39*/	{"ゆめウェーブ / 岡山県笠岡市","fmyumewave"},
/*40*/	{"FMわっしょい / 山口県防府市","fmwassyoi"},
/*41*/	{"FMスマイルウェ～ブ / 山口県山陽小野田市","fmsunsunkirara"},
/*42*/	{"FM KITAQ / 福岡県北九州市","fmkitaq"},
/*43*/	{"FMたんと / 福岡県大牟田市","fmtanto"},
/*44*/	{"えびすFM / 佐賀県佐賀市","ebisufm"},
/*45*/	{"みつばちラジオ / 熊本県天草市","mitsubachiradio"},
/*46*/	{"FMさせぼ / 長崎県佐世保市","happyfm"},
/*47*/	{"エフエム諫早 / 長崎県諫早市","fmisahaya"},
/*48*/	{"FMしまばら / 長崎県島原市","fmshimabara"},
/*49*/	{"FMひまわり / 長崎県南島原市","fmhimawari"},
/*50*/	{"サンシャインFM / 宮崎県宮崎市","sunshinefm"},
/*51*/	{"シティエフエム都城 / 宮崎県都城市","cityfmmiyakonojyo"},
/*52*/	{"FMさつませんだい / 鹿児島県薩摩川内市","fmsatsumasendai"},
/*53*/	{"FMきりしま / 鹿児島県霧島市","fmkirishima"},
/*54*/	{"フレンズFM / 鹿児島県鹿児島市","friendsfm"},
/*55*/	{"FMぎんが / 鹿児島県鹿児島市","fmginga"},
/*56*/	{"FMかのや / 鹿児島県鹿屋市","fmkanoya"},
/*57*/	{"FMうるま / 沖縄県うるま市","fmuruma"},
/*58*/	{"FMみやこ / 沖縄県宮古島市","fmmiyako"},
};
static byte initial_station = 58;   // FMみやこ

class FmPlapla : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    FmPlapla(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const size_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode) {
#else
    FmPlapla(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const size_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode, 1 - cpuDecode, 4096) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
      defaultStationIdx = initial_station;
    }
#ifndef SEPARATE_DOWNLOAD_TASK
    FmPlapla(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const size_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3) {
#else
    FmPlapla(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const size_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3, 1 - cpuDecode, 4096) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
      defaultStationIdx = initial_station;
    }

    ~FmPlapla() {
      if(decoder)
        delete decoder;
      if(source)
        delete source;
    }

    class station_t : public WebRadio::Station {
      public:
        station_t(FmPlapla* _radio, const char * _name, const char * _id) : Station(_radio), name(_name), id(_id)  {
        }
        
        FmPlapla * getFmPlapla() {
          return (FmPlapla *)radio;
        }
        
        virtual const char * getName()  { return name; }
        virtual bool play() override {
          getFmPlapla()->select_station = this;
          return true;
        }
        
        String getUrl() {
//        return "https://fmplapla.com/api/select_stream?channel=0&quality=high&burst=5&station=" + String(id);
          return "https://fmplapla.com/api/select_stream?station=" + String(id) + "&burst=5";
        }
        
        AudioGeneratorOpus * getDecoder() {
          auto jcba = (FmPlapla *)radio;
          auto decoder = new AudioGeneratorOpus();
          decoder->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
          decoder->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
          return decoder;
        }

        AudioFileSourceFmPlapla * getSource() {
          auto jcba = (FmPlapla *)radio;
          AudioFileSourceFmPlapla * source;
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
        
      private:
        const char* name;
        const char* id;
    };

    virtual bool begin() override {
      auto psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
      if(!psram) {
        onError("Requires PSRAM");
        return false;
      }

      if(!bufferSize)
        bufferSize = std::max(5 * 1024, (int)std::min( (size_t)(128 * 1024), psram));
      
      startTask();
      return true; 
    }

    void stop() {
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
    
    virtual void handle() override {
#ifndef SEPARATE_DOWNLOAD_TASK
      downloadTaskCore();
#endif
    }
    
    String getInfoBuffer() {
      if(source)
        return source->getInfoBuffer();
      return "";
    }
    
  private:
    virtual void downloadTaskCore() override {
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
    
    virtual void decodeTaskCore() override {
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
    
    virtual void saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) override {
      nvs_set_u8(nvs_handle, "FmPlapla", getIndex(station));
    }

    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override {
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
      
  private:
    AudioFileSourceFmPlapla * source = nullptr;
    AudioGeneratorOpus * decoder = nullptr;
    uint8_t * buffer = nullptr;
    size_t bufferSize;
    byte stopDecode = 0;    // 2:request stop 1:accept request

};

#endif
