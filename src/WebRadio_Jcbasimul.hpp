/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_
#define _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_

#include <AudioGeneratorOpus.h>
#include "AudioFileSourceJcbasimul.hpp"
#include "WebRadio.h"

static constexpr const uint32_t decode_stack_size = 16*1024;
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
/*69*/	{"エフエムあづみの / 長野県","azuminofm"},
/*70*/	{"高ボッチ高原FM / 長野県","shiojirifm"},
/*71*/	{"伊那谷FM / 長野県","inadanifm"},
/*72*/	{"ＦＭＰｉＰｉ / 岐阜県","fmpipi"},
/*73*/	{"ＦＭわっち / 岐阜県","fmwatch"},
/*74*/	{"Ｈｉｔｓ ＦＭ  / 岐阜県","hitsfm"},
/*75*/	{"FM Haro! / 静岡県","haro"},
/*76*/	{"FM ISみらいずステーション / 静岡県","fmis"},
/*77*/	{"g-sky76.5 / 静岡県","fmshimada"},
/*78*/	{"富士山ＧＯＧＯＦＭ / 静岡県","fujiyamagogofm"},
/*79*/	{"ボイスキュー / 静岡県","voicecue"},
/*80*/	{"マリンパル / 静岡県","fmshimizu"},
/*81*/	{"FM-Hi！ / 静岡県","fmhi"},
/*82*/	{"Radio-f / 静岡県","radiof"},
/*83*/	{"COAST-FM76.7MH z / 静岡県","coastfm"},
/*84*/	{"エフエムなぎさステーション / 静岡県","nagisastation"},
/*85*/	{"Ciao! / 静岡県","ciao"},
/*86*/	{"FMいずのくに / 静岡県","fmizunokuni"},
/*87*/	{"RADIO LUSH / 静岡県","fmyaizu"},
/*88*/	{"エフエム　ななみ / 愛知県","fmnanami"},
/*89*/	{"United North / 愛知県","unitednorth"},
/*90*/	{"RADIO SANQ  / 愛知県","radiosanq"},
/*91*/	{"i-wave / 愛知県","fmichinomiya"},
/*92*/	{"HeartFM / 愛知県","heartfm"},
/*93*/	{"いなべエフエム / 三重県","inabefm"},
/*94*/	{"Suzuka Voice FM 78.3MHz / 三重県","suzuka"},
/*95*/	{"富山シティエフエム株式会社 / 富山県","toyamacityfm"},
/*96*/	{"エフエムとなみ / 富山県","fmtonami"},
/*97*/	{"ラジオたかおか / 富山県","radiotakaoka"},
/*98*/	{"ラジオこまつ / 石川県","radiokomatsu"},
/*99*/	{"ラジオななお / 石川県","radionanao"},
/*100*/	{"ラジオかなざわ / 石川県","radiokanazawa"},
/*101*/	{"えふえむ草津 / 滋賀県","fmkusatsu"},
/*102*/	{"ＦＭいかる / 京都府","fmikaru"},
/*103*/	{"FMうじ / 京都府","fmuji"},
/*104*/	{"FMまいづる / 京都府","fmmaizuru"},
/*105*/	{"FM845 / 京都府","kyotoribingufm"},
/*106*/	{"FM千里 / 大阪府","fmsenri"},
/*107*/	{"ウメダFM Be Happy!789 / 大阪府","umedafm"},
/*108*/	{"タッキー816みのおエフエム / 大阪府","minofm"},
/*109*/	{"エフエムいたみ / 兵庫県","fmitami"},
/*110*/	{"ハミングFM宝塚 / 兵庫県","fmtakarazuka"},
/*111*/	{"さくらFM / 兵庫県","sakurafm"},
/*112*/	{"エフエムみっきぃ / 兵庫県","fmmiki"},
/*113*/	{"805たんば / 兵庫県","tanba"},
/*114*/	{"FM GENKI / 兵庫県","fmgenki"},
/*115*/	{"なら どっと ＦＭ / 奈良県","narafm"},
/*116*/	{"エフエムハイホー / 奈良県","fmnishiyamato"},
/*117*/	{"ＦＭ五條 / 奈良県","fmgojo"},
/*118*/	{"FMまほろば / 奈良県","fmmahoroba"},
/*119*/	{"バナナエフエム / 和歌山県","bananafm"},
/*120*/	{"FM TANABE / 和歌山県","fmtanabe"},
/*121*/	{"FMはしもと / 和歌山県","fmhashimoto"},
/*122*/	{"FMビーチステーション / 和歌山県","beachstation"},
/*123*/	{"レディオ モモ / 岡山県","radiomomo"},
/*124*/	{"FMくらしき / 岡山県","fmkurashiki"},
/*125*/	{"FMふくやま / 広島県","bingo"},
/*126*/	{"FMおのみち / 広島県","fmonomichi"},
/*127*/	{"FMちゅーピー / 広島県","fmchupea"},
/*128*/	{"FMはつかいち / 広島県","fmhatsukaichi"},
/*129*/	{"FM東広島 / 広島県","fmhigashihiroshima"},
/*130*/	{"FOR LIFE RADIO / 広島県","fmmihara"},
/*131*/	{"ＣＯＭＥ ＯＮ ! ＦＭ / 山口県","comeonfm"},
/*132*/	{"しゅうなんＦＭ / 山口県","shunanfm"},
/*133*/	{"RADIO BIRD / 鳥取県","radiobird"},
/*134*/	{"エフエム・サン / 香川県","fmsun"},
/*135*/	{"FMラヂオバリバリ / 愛媛県","fmradiobaribari"},
/*136*/	{"FMがいや  / 愛媛県","fmgaiya"},
/*137*/	{"Hello! NEW 新居浜 FM / 愛媛県","niihamafm"},
/*138*/	{"Dreams FM / 福岡県","dreamsfm"},
/*139*/	{"FM八女 / 福岡県","fmyame"},
/*140*/	{"ＦＭからつ / 佐賀県","fmkaratsu"},
/*141*/	{"Kappa FM / 熊本県","fmyatsushiro"},
/*142*/	{"FM791 / 熊本県","kumamotocityfm"},
/*143*/	{"ゆふいんラヂオ局 / 大分県","yufuin"},
/*144*/	{"NOASFM / 大分県","noasfm"},
};
static byte initial_station = 107;   // ウメダFM Be Happy!789

class Jcbasimul : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode) {
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, priorityDecode, 1 - cpuDecode, 4096) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i][0], station_list[i][1]));
      defaultStationIdx = initial_station;
    }
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3) {
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, decode_stack_size, 3, 1 - cpuDecode, 4096) {
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
    uint32_t bufferSize;
    byte stopDecode = 0;    // 2:request stop 1:accept request

};

#endif
