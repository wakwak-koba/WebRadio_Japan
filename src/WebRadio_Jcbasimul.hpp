/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_
#define _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceJcbasimul.hpp"
#include "WebRadio.h"

static constexpr const size_t decode_stack_size = 16*1024;
static constexpr const char* station_list[][2] =
{
  {"FMはな / 北海道","fmhana"},
  {"エフエムもえる / 北海道","moeru"},
  {"Airてっし / 北海道","airtesshi"},
  {"ラジオニセコ / 北海道","radioniseko"},
  {"FMいるか / 北海道","iruka"},
  {"FM AZUR / 青森県","fmazur"},
  {"アップルウェーブ / 青森県","applewave"},
  {"ＦＭごしょがわら / 青森県","fmgoshogawara"},
  {"FMONE / 岩手県","fmone"},
  {"エフエムいわぬま / 宮城県","fmiwanuma"},
  {"H＠！FM / 宮城県","hatfm"},
  {"ラジオ モンスター / 山形県","radiomonster"},
  {"エフエムNCV / 山形県","yonezawancvfm"},
  {"えふえむい～じゃんおらんだらじお / 山形県","orandaradio"},
  {"ハーバーラジオ / 山形県","harborradio"},
  {"ウルトラFM / 福島県","ultrafm"},
  {"FMポコ / 福島県","fmpoco"},
  {"エフエムきたかた / 福島県","fmkitakata"},
  {"FMだいご / 茨城県","fmdaigo"},
  {"FMかしま / 茨城県","fmkashima"},
  {"ラジオ高崎 / 群馬県","radiotakasaki"},
  {"エフエム太郎 / 群馬県","fmtaro"},
  {"FM OZE / 群馬県","fmoze"},
  {"ラヂオななみ / 群馬県","radionanami"},
  {"FMチャッピー / 埼玉県","fmchappy"},
  {"発するＦＭ / 埼玉県","miyoshifm"},
  {"ラジオ川越 / 埼玉県","radiokawagoe"},
  {"市川うららＦＭ(I&#038;U-LaLaFM) / 千葉県","ulalafm"},
  {"かずさエフエム / 千葉県","kazusafm"},
  {"ラジオ成田 / 千葉県","radionarita"},
  {"ふくろうFM / 千葉県","fmfukuro"},
  {"SKYWAVE FM / 千葉県","skywavefm"},
  {"FMえどがわ / 東京都","fmedogawa"},
  {"むさしのFM / 東京都","musashinofm"},
  {"FMしながわ / 東京都","fmshinagawa"},
  {"かつしかFM / 東京都","fmkatsushika"},
  {"渋谷のラジオ / 東京都","shibuyanoradio"},
  {"FMブルー湘南 / 神奈川県","fmblueshonan"},
  {"鎌倉FM / 神奈川県","kamakurafm"},
  {"FM湘南ナパサ / 神奈川県","fmshonan"},
  {"FMおだわら / 神奈川県","fmodawara"},
  {"湘南マジックウェイブ / 神奈川県","magicwave"},
  {"FMやまと / 神奈川県","fmyamato"},
  {"レディオ湘南 / 神奈川県","radioshonan"},
  {"FMサルース / 神奈川県","fmsalus"},
  {"エフエム甲府 / 山梨県","fmkofu"},
  {"FMふじやま / 山梨県","fmfujiyama"},
  {"エフエム　ふじごこ / 山梨県","fmfujigoko"},
  {"ＦＭ八ヶ岳 / 山梨県","fmyatsugatake"},
  {"ラジオチャット・ＦＭにいつ / 新潟県","radiochat"},
  {"FMうおぬま / 新潟県","fmuonuma"},
  {"エフエムながおか / 新潟県","fmnagaoka"},
  {"エフエムしばた / 新潟県","fmshibata"},
  {"FM KENTO / 新潟県","fmkento"},
  {"ＦＭゆきぐに / 新潟県","fmyukiguni"},
  {"FM-J エフエム上越 / 新潟県","fmjyoetsu"},
  {"ＦＭピッカラ / 新潟県","fmpikkara"},
  {"LCV FM / 長野県","lovefm"},
  {"FM軽井沢 / 長野県","fmkaruizawa"},
  {"エフエムあづみの / 長野県","azuminofm"},
  {"高ボッチ高原FM / 長野県","shiojirifm"},
  {"FMPiPi / 岐阜県","fmpipi"},
  {"FMわっち / 岐阜県","fmwatch"},
  {"Hits FM / 岐阜県","hitsfm"},
  {"FM Haro！ / 静岡県","haro"},
  {"FM IS / 静岡県","fmis"},
  {"g-sky76.5 / 静岡県","fmshimada"},
  {"富士山GOGOFM / 静岡県","fujiyamagogofm"},
  {"ボイスキュー / 静岡県","voicecue"},
  {"マリンパル / 静岡県","fmshimizu"},
  {"FM-Hi！ / 静岡県","fmhi"},
  {"Radio-f / 静岡県","radiof"},
  {"COAST－FM76.7MHｚ / 静岡県","coastfm"},
  {"エフエムなぎさステーション / 静岡県","nagisastation"},
  {"Ciao！ / 静岡県","ciao"},
  {"FMいずのくに / 静岡県","fmizunokuni"},
  {"エフエム　ななみ / 愛知県","fmnanami"},
  {"United North / 愛知県","unitednorth"},
  {"RADIO SANQ / 愛知県","radiosanq"},
  {"i-wave / 愛知県","fmichinomiya"},
  {"いなべエフエム / 三重県","inabefm"},
  {"Suzuka Voice FM 78.3MHz / 三重県","suzuka"},
  {"富山シティエフエム株式会社 / 富山県","toyamacityfm"},
  {"エフエムとなみ / 富山県","fmtonami"},
  {"ラジオたかおか / 富山県","radiotakaoka"},
  {"ラジオこまつ / 石川県","radiokomatsu"},
  {"ラジオななお / 石川県","radionanao"},
  {"ラジオかなざわ / 石川県","radiokanazawa"},
  {"えふえむ草津 / 滋賀県","fmkusatsu"},
  {"FMいかる / 京都府","fmikaru"},
  {"FMうじ / 京都府","fmuji"},
  {"FMまいづる / 京都府","fmmaizuru"},
  {"FM845 / 京都府","kyotoribingufm"},
  {"FM千里 / 大阪府","fmsenri"},
  {"FMちゃお / 大阪府","fmyao"},
  {"ウメダFM Be Happy!789 / 大阪府","umedafm"},
  {"FM-HANAKO 82.4MHz / 大阪府","fmhanako"},
  {"タッキー816みのおエフエム / 大阪府","minofm"},
  {"エフエムいたみ / 兵庫県","fmitami"},
  {"ハミングFM宝塚 / 兵庫県","fmtakarazuka"},
  {"さくらFM / 兵庫県","sakurafm"},
  {"エフエムみっきぃ / 兵庫県","fmmiki"},
  {"805たんば / 兵庫県","tanba"},
  {"FM GENKI / 兵庫県","fmgenki"},
  {"なら どっと FM / 奈良県","narafm"},
  {"エフエムハイホー / 奈良県","fmnishiyamato"},
  {"FM五條 / 奈良県","fmgojo"},
  {"バナナエフエム / 和歌山県","bananafm"},
  {"FM TANABE / 和歌山県","fmtanabe"},
  {"FMはしもと / 和歌山県","fmhashimoto"},
  {"FMビーチステーション / 和歌山県","beachstation"},
  {"レディオ モモ / 岡山県","radiomomo"},
  {"エフエムくらしき / 岡山県","fmkurashiki"},
  {"FMふくやま / 広島県","bingo"},
  {"FMおのみち / 広島県","fmonomichi"},
  {"ＦＭちゅーピー / 広島県","fmchupea"},
  {"FMはつかいち / 広島県","fmhatsukaichi"},
  {"FM東広島 / 広島県","fmhigashihiroshima"},
  {"FOR　LIFE　RADIO / 広島県","fmmihara"},
  {"COME ON ! FM / 山口県","comeonfm"},
  {"しゅうなんFM / 山口県","shunanfm"},
  {"RADIO BIRD / 鳥取県","radiobird"},
  {"FMラヂオバリバリ / 愛媛県","fmradiobaribari"},
  {"FMがいや / 愛媛県","fmgaiya"},
  {"Hello! NEW 新居浜 FM / 愛媛県","niihamafm"},
  {"DreamsFM / 福岡県","dreamsfm"},
  {"FM八女 / 福岡県","fmyame"},
  {"FMからつ / 佐賀県","fmkaratsu"},
  {"Kappa　FM / 熊本県","fmyatsushiro"},
  {"FM791 / 熊本県","kumamotocityfm"},
  {"ゆふいんラヂオ局 / 大分県","yufuin"},
  {"NOASFM / 大分県","noasfm"},
};
static byte initial_station = 95;   // ウメダFM Be Happy!789

class Jcbasimul : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint16_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode) {
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint16_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode, 1 - cpuDecode, 4096) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
      defaultStationIdx = initial_station;
    }
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint16_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3) {
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint16_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3, 1 - cpuDecode, 4096) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
      defaultStationIdx = initial_station;
    }

    ~Jcbasimul() {
      if(decoder)
        delete decoder;
      if(source)
        delete source;
    }

    class station_t : public WebRadio::Station {
      public:
        station_t(Jcbasimul* _radio, const char * _name, const char * _id) : Station(_radio), name(_name), id(_id)  {
        }
        
        Jcbasimul * getJcbasimul() {
          return (Jcbasimul *)radio;
        }
        
        virtual const char * getName()  { return name; }
        virtual bool play() override {
          getJcbasimul()->select_station = this;
          return true;
        }
        
        String getUrl() {
          return "https://api.radimo.smen.biz/api/v1/select_stream?channel=0&quality=high&burst=5&station=" + String(id);
        }
        
        AudioGeneratorOpus * getDecoder() {
          auto jcba = (Jcbasimul *)radio;
          auto decoder = new AudioGeneratorOpus();
          decoder->RegisterMetadataCB(jcba->fnCbMetadata, jcba->fnCbMetadata_data);
          decoder->RegisterStatusCB  (jcba->fnCbStatus  , jcba->fnCbStatus_data  );
          return decoder;
        }

        AudioFileSourceJcbasimul * getSource() {
          auto jcba = (Jcbasimul *)radio;
          AudioFileSourceJcbasimul * source;
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
            delay(5000);
          } else
            getJcbasimul()->sendLog(url.c_str());
          
          return source;
        }
        
      private:
        const char* name;
        const char* id;
    };

    virtual bool begin() override {
      if(!bufferSize)
        bufferSize = std::max(5 * 1024, (int)std::min( (uint32_t)UINT16_MAX, heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));
      
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
        stop();
        current_station = select_station;

        if(onPlay)
          onPlay(current_station->getName(), getIndex(current_station));
        saveSettings = millis() + 10000;

        select_station = nullptr;
      } else if(current_station && !source) {
        source = ((station_t *)current_station)->getSource();
      } else if(source && !decoder) {
        decoder = ((station_t *)current_station)->getDecoder();
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
      
      if (saveSettings > 0 && now_millis > saveSettings) {
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
        ;
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
          Serial.println(source->getSize());
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
      nvs_set_u8(nvs_handle, "jcbasimul", getIndex(station));
    }

    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override {
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
      
  private:
    AudioFileSourceJcbasimul * source = nullptr;
    AudioGeneratorOpus * decoder = nullptr;
    uint8_t * buffer = nullptr;
    uint16_t bufferSize;
    byte stopDecode = 0;    // 2:request stop 1:accept request

};

#endif
