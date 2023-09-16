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
/*00*/	{"ＦＭはな / 北海道","fmhana"},
/*01*/	{"エフエムもえる / 北海道","moeru"},
/*02*/	{"Ａｉｒてっし / 北海道","airtesshi"},
/*03*/	{"ラジオニセコ / 北海道","radioniseko"},
/*04*/	{"FMいるか / 北海道","iruka"},
/*05*/	{"FMびゅー / 北海道","muroran"},
/*06*/	{"FMとまこまい / 北海道","fmtomakomai"},
/*07*/	{"FM AZUR / 青森県","fmazur"},
/*08*/	{"アップルウェーブ / 青森県","applewave"},
/*09*/	{"FMごしょがわら / 青森県","fmgoshogawara"},
/*10*/	{"FMONE / 岩手県","fmone"},
/*11*/	{"エフエムいわぬま / 宮城県","fmiwanuma"},
/*12*/	{"Ｈ＠！ＦＭ  / 宮城県","hatfm"},
/*13*/	{"ラジオ モンスター / 山形県","radiomonster"},
/*14*/	{"エフエムNCV / 山形県","yonezawancvfm"},
/*15*/	{"えふえむい～じゃんおらんだらじお / 山形県","orandaradio"},
/*16*/	{"ハーバーラジオ / 山形県","harborradio"},
/*17*/	{"ウルトラＦＭ / 福島県","ultrafm"},
/*18*/	{"ＦＭポコ / 福島県","fmpoco"},
/*19*/	{"エフエムきたかた / 福島県","fmkitakata"},
/*20*/	{"FM愛'S / 福島県","fmaizu"},
/*21*/	{"ＦＭだいご / 茨城県","fmdaigo"},
/*22*/	{"FMかしま / 茨城県","fmkashima"},
/*23*/	{"ラジオ高崎 / 群馬県","radiotakasaki"},
/*24*/	{"エフエム太郎 / 群馬県","fmtaro"},
/*25*/	{"ＦＭ ＯＺＥ / 群馬県","fmoze"},
/*26*/	{"ラヂオななみ / 群馬県","radionanami"},
/*27*/	{"ＦＭチャッピー / 埼玉県","fmchappy"},
/*28*/	{"発するFM / 埼玉県","miyoshifm"},
/*29*/	{"ラジオ川越 / 埼玉県","radiokawagoe"},
/*30*/	{"市川うららFM(I&U-LaLaFM) / 千葉県","ulalafm"},
/*31*/	{"かずさFM / 千葉県","kazusafm"},
/*32*/	{"ラジオ成田 / 千葉県","radionarita"},
/*33*/	{"ふくろうFM / 千葉県","fmfukuro"},
/*34*/	{"SKYWAVE FM / 千葉県","skywavefm"},
/*35*/	{"ＦＭえどがわ / 東京都","fmedogawa"},
/*36*/	{"むさしのＦＭ / 東京都","musashinofm"},
/*37*/	{"FMしながわ / 東京都","fmshinagawa"},
/*38*/	{"かつしかFM / 東京都","fmkatsushika"},
/*39*/	{"渋谷のラジオ / 東京都","shibuyanoradio"},
/*40*/	{"ＦＭブルー湘南 / 神奈川県","fmblueshonan"},
/*41*/	{"鎌倉FM / 神奈川県","kamakurafm"},
/*42*/	{"FM湘南ナパサ / 神奈川県","fmshonan"},
/*43*/	{"FMおだわら / 神奈川県","fmodawara"},
/*44*/	{"FM湘南マジックウェイブ / 神奈川県","magicwave"},
/*45*/	{"FMやまと / 神奈川県","fmyamato"},
/*46*/	{"レディオ湘南 / 神奈川県","radioshonan"},
/*47*/	{"FMサルース / 神奈川県","fmsalus"},
/*48*/	{"エフエム甲府 / 山梨県","fmkofu"},
/*49*/	{"FMふじやま / 山梨県","fmfujiyama"},
/*50*/	{"エフエム　ふじごこ / 山梨県","fmfujigoko"},
/*51*/	{"FM八ヶ岳 / 山梨県","fmyatsugatake"},
/*52*/	{"ラジオチャット・FMにいつ / 新潟県","radiochat"},
/*53*/	{"FMうおぬま / 新潟県","fmuonuma"},
/*54*/	{"エフエムながおか / 新潟県","fmnagaoka"},
/*55*/	{"エフエムしばた / 新潟県","fmshibata"},
/*56*/	{"FM KENTO / 新潟県","fmkento"},
/*57*/	{"ＦＭゆきぐに / 新潟県","fmyukiguni"},
/*58*/	{"FMじょうえつ / 新潟県","fmjyoetsu"},
/*59*/	{"FMピッカラ / 新潟県","fmpikkara"},
/*60*/	{"LCV FM / 長野県","lovefm"},
/*61*/	{"FM軽井沢 / 長野県","fmkaruizawa"},
/*62*/	{"エフエムあづみの / 長野県","azuminofm"},
/*63*/	{"高ボッチ高原FM / 長野県","shiojirifm"},
/*64*/	{"ＦＭＰｉＰｉ / 岐阜県","fmpipi"},
/*65*/	{"ＦＭわっち / 岐阜県","fmwatch"},
/*66*/	{"Ｈｉｔｓ ＦＭ  / 岐阜県","hitsfm"},
/*67*/	{"FM Haro! / 静岡県","haro"},
/*68*/	{"FM ISみらいずステーション / 静岡県","fmis"},
/*69*/	{"g-sky76.5 / 静岡県","fmshimada"},
/*70*/	{"富士山ＧＯＧＯＦＭ / 静岡県","fujiyamagogofm"},
/*71*/	{"ボイスキュー / 静岡県","voicecue"},
/*72*/	{"マリンパル / 静岡県","fmshimizu"},
/*73*/	{"FM-Hi！ / 静岡県","fmhi"},
/*74*/	{"Radio-f / 静岡県","radiof"},
/*75*/	{"COAST-FM76.7MH z / 静岡県","coastfm"},
/*76*/	{"エフエムなぎさステーション / 静岡県","nagisastation"},
/*77*/	{"Ciao! / 静岡県","ciao"},
/*78*/	{"FMいずのくに / 静岡県","fmizunokuni"},
/*79*/	{"エフエム　ななみ / 愛知県","fmnanami"},
/*80*/	{"United North / 愛知県","unitednorth"},
/*81*/	{"RADIO SANQ  / 愛知県","radiosanq"},
/*82*/	{"i-wave / 愛知県","fmichinomiya"},
/*83*/	{"いなべエフエム / 三重県","inabefm"},
/*84*/	{"Suzuka Voice FM 78.3MHz / 三重県","suzuka"},
/*85*/	{"富山シティエフエム株式会社 / 富山県","toyamacityfm"},
/*86*/	{"エフエムとなみ / 富山県","fmtonami"},
/*87*/	{"ラジオたかおか / 富山県","radiotakaoka"},
/*88*/	{"ラジオこまつ / 石川県","radiokomatsu"},
/*89*/	{"ラジオななお / 石川県","radionanao"},
/*90*/	{"ラジオかなざわ / 石川県","radiokanazawa"},
/*91*/	{"えふえむ草津 / 滋賀県","fmkusatsu"},
/*92*/	{"ＦＭいかる / 京都府","fmikaru"},
/*93*/	{"FMうじ / 京都府","fmuji"},
/*94*/	{"FMまいづる / 京都府","fmmaizuru"},
/*95*/	{"FM845 / 京都府","kyotoribingufm"},
/*96*/	{"FM千里 / 大阪府","fmsenri"},
/*97*/	{"ＦＭちゃお / 大阪府","fmyao"},
/*98*/	{"ウメダFM Be Happy!789 / 大阪府","umedafm"},
/*99*/	{"タッキー816みのおエフエム / 大阪府","minofm"},
/*100*/	{"エフエムいたみ / 兵庫県","fmitami"},
/*101*/	{"ハミングFM宝塚 / 兵庫県","fmtakarazuka"},
/*102*/	{"さくらFM / 兵庫県","sakurafm"},
/*103*/	{"エフエムみっきぃ / 兵庫県","fmmiki"},
/*104*/	{"805たんば / 兵庫県","tanba"},
/*105*/	{"FM GENKI / 兵庫県","fmgenki"},
/*106*/	{"なら どっと ＦＭ / 奈良県","narafm"},
/*107*/	{"エフエムハイホー / 奈良県","fmnishiyamato"},
/*108*/	{"ＦＭ五條 / 奈良県","fmgojo"},
/*109*/	{"バナナエフエム / 和歌山県","bananafm"},
/*110*/	{"FM TANABE / 和歌山県","fmtanabe"},
/*111*/	{"FMはしもと / 和歌山県","fmhashimoto"},
/*112*/	{"FMビーチステーション / 和歌山県","beachstation"},
/*113*/	{"レディオ モモ / 岡山県","radiomomo"},
/*114*/	{"FMくらしき / 岡山県","fmkurashiki"},
/*115*/	{"FMふくやま / 広島県","bingo"},
/*116*/	{"FMおのみち / 広島県","fmonomichi"},
/*117*/	{"FMちゅーピー / 広島県","fmchupea"},
/*118*/	{"FMはつかいち / 広島県","fmhatsukaichi"},
/*119*/	{"FM東広島 / 広島県","fmhigashihiroshima"},
/*120*/	{"FOR LIFE RADIO / 広島県","fmmihara"},
/*121*/	{"ＣＯＭＥ ＯＮ ! ＦＭ / 山口県","comeonfm"},
/*122*/	{"しゅうなんＦＭ / 山口県","shunanfm"},
/*123*/	{"RADIO BIRD / 鳥取県","radiobird"},
/*124*/	{"FMラヂオバリバリ / 愛媛県","fmradiobaribari"},
/*125*/	{"FMがいや  / 愛媛県","fmgaiya"},
/*126*/	{"Hello! NEW 新居浜 FM / 愛媛県","niihamafm"},
/*127*/	{"Dreams FM / 福岡県","dreamsfm"},
/*128*/	{"FM八女 / 福岡県","fmyame"},
/*129*/	{"ＦＭからつ / 佐賀県","fmkaratsu"},
/*130*/	{"Kappa FM / 熊本県","fmyatsushiro"},
/*131*/	{"FM791 / 熊本県","kumamotocityfm"},
/*132*/	{"ゆふいんラヂオ局 / 大分県","yufuin"},
/*133*/	{"NOASFM / 大分県","noasfm"},
};
static byte initial_station = 96;   // ウメダFM Be Happy!789

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
//          delay(5000);
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
        source = ((station_t *)current_station)->getSource();
        if(source)
          decoder = ((station_t *)current_station)->getDecoder();

        if(onPlay)
          onPlay(current_station->getName(), getIndex(current_station));
        saveSettings = millis() + 10000;

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
