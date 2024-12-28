/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_
#define _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <AudioGeneratorAAC.h>
#include <AudioFileSourceHTTPStream.h>
#include "AudioFileSourceTS.hpp"
#include "WebRadio.h"
#include "common.h"

struct station_t {
    const uint16_t id;
    const char * name;
};

static const station_t station_list[] = {
/*00*/	{30058,"FM ABASHIRI / 北海道"},
/*01*/	{30074,"FMりべーる / 北海道"},
/*02*/	{30045,"FMねむろ / 北海道"},
/*03*/	{30029,"FMくしろ / 北海道"},
/*04*/	{30004,"FMはまなす / 北海道"},
/*05*/	{30016,"FM JAGA / 北海道"},
/*06*/	{30038,"FM WING / 北海道"},
/*07*/	{30032,"さっぽろ村ラジオ / 北海道"},
/*08*/	{30044,"RADIOワンダーストレージ FMドラマシティ / 北海道"},
/*09*/	{30005,"三角山放送局（札幌市西区） / 北海道"},
/*10*/	{30034,"ラジオカロスサッポロ / 北海道"},
/*11*/	{30090,"FMアップル / 北海道"},
/*12*/	{30025,"FMおたる / 北海道"},
/*13*/	{30015,"FMメイプル / 北海道"},
/*14*/	{30087,"wi-radio / 北海道"},
/*15*/	{30079,"BeFM / 東北"},
/*16*/	{30050,"カシオペアFM / 東北"},
/*17*/	{30017,"ラヂオもりおか / 東北"},
/*18*/	{30097,"みやこハーバーラジオ / 東北"},
/*19*/	{30094,"ラヂオ気仙沼 / 東北"},
/*20*/	{30037,"ラジオ石巻 / 東北"},
/*21*/	{30056,"BAY WAVE / 東北"},
/*22*/	{30007,"RADIO3 / 東北"},
/*23*/	{30018,"fmいずみ / 東北"},
/*24*/	{30092,"なとらじ801 / 東北"},
/*25*/	{30089,"鹿角きりたんぽFM / 東北"},
/*26*/	{30014,"エフエム椿台 / 東北"},
/*27*/	{30076,"横手かまくらエフエム / 東北"},
/*28*/	{30030,"FMゆーとぴあ / 東北"},
/*29*/	{30019,"FM Mot.com / 東北"},
/*30*/	{30020,"KOCOラジ / 東北"},
/*31*/	{30009,"FMいわき / 東北"},
/*32*/	{30023,"FMひたち / 関東"},
/*33*/	{30075,"たかはぎFM / 関東"},
/*34*/	{30022,"FMぱるるん / 関東"},
/*35*/	{30021,"FMうしくうれしく放送 / 関東"},
/*36*/	{30043,"まえばしラジオ / 関東"},
/*37*/	{30047,"FMふっかちゃん / 関東"},
/*38*/	{30048,"FMわたらせ / 関東"},
/*39*/	{30002,"フラワーラジオ / 関東"},
/*40*/	{30096," ハローハッピー・こしがやエフエム / 関東"},
/*41*/	{30008,"REDS WAVE / 関東"},
/*42*/	{30035,"FM Kawaguchi / 関東"},
/*43*/	{30026,"775ライブリーFM / 関東"},
/*44*/	{30036,"レインボータウンFM / 関東"},
/*45*/	{30027,"エフエム世田谷 / 関東"},
/*46*/	{30039,"調布FM / 関東"},
/*47*/	{30033,"FMたちかわ / 関東"},
/*48*/	{30081,"Tokyo Star Radio(八王子FM) / 関東"},
/*49*/	{30046,"かわさきＦＭ / 関東"},
/*50*/	{30057,"ＦＭカオン / 関東"},
/*51*/	{30040,"エフエムEGAO / 東海"},
/*52*/	{30001,"FM N1 / 北信越"},
/*53*/	{30006,"ラジオ・ミュー / 北信越"},
/*54*/	{30012,"敦賀FM / 北信越"},
/*55*/	{30061,"ラジオスイート / 近畿"},
/*56*/	{30067,"エフエム花 / 近畿"},
/*57*/	{30071,"FM87.0 RADIO MIX KYOTO / 近畿"},
/*58*/	{30082,"京都三条ラジオカフェ / 近畿"},
/*59*/	{30063,"FMおとくに / 近畿"},
/*60*/	{30073,"FMたんご / 近畿"},
/*61*/	{30078,"BAN-BANラジオ / 近畿"},
/*62*/	{30013,"FM ジャングル / 近畿"},
/*63*/	{30053,"DARAZ　FM / 中国・四国"},
/*64*/	{30010,"FMびざん / 中国・四国"},
/*65*/	{30024,"FM815（高松） / 中国・四国"},
/*66*/	{30052,"AIR STATION HIBIKI / 九州・沖縄"},
/*67*/	{30077,"COMI&#215;TEN / 九州・沖縄"},
/*68*/	{30085,"チョクラジ / 九州・沖縄"},
/*69*/	{30088,"FMのべおか / 九州・沖縄"},
/*70*/	{30072,"エフエムたつごう / 九州・沖縄"},
/*71*/	{30054,"あまみエフエム / 九州・沖縄"},
/*72*/	{30095,"FMやんばる / 九州・沖縄"},
/*73*/	{30066,"オキラジ / 九州・沖縄"},
/*74*/	{30093,"FMぎのわん / 九州・沖縄"},
/*75*/	{30098,"ぎのわんシティFM / 九州・沖縄"},
/*76*/	{30068,"fm那覇 / 九州・沖縄"},
/*77*/	{30083,"FMとよみ / 九州・沖縄"},
/*78*/	{30069,"FMいしがきサンサンラジオ / 九州・沖縄"},
/*79*/	{30118,"かしみんFM / ネット放送局"},
/*80*/	{30119,"たこみんFM / ネット放送局"},
/*81*/	{30099,"試験放送A / ネット放送局"},
};
static byte initial_station = 9;   // 三角山放送局（札幌市西区）

static const char * headerKeys[] = {"Content-Encoding"};

class ListenRadio : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3) {
#else
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3, 1 - cpuDecode, 2560) {
#endif
      for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
        stations.push_back(new station_t(this, station_list[i].id, station_list[i].name));
      defaultStationIdx = initial_station;
    }
    
    ~ListenRadio() {
      if(decoder)
        delete decoder;
      if(decode_buffer)
        free(decode_buffer);
    }
    
    class station_t : public WebRadio::Station {
      private:
        const uint16_t id;
        const char* name;

      public:
        station_t(ListenRadio* _radio, const uint16_t _id, const char * _name) : Station(_radio), id(_id), name(_name)  {}
        ~station_t() {
          dispose();
        }
        
        virtual void dispose() override { clearPlaylists(); }
        virtual const char * getName() override { return name; }
        virtual bool play() override {
          getListenRadio()->play(this);
          return true;
        }
        
        ListenRadio * getListenRadio() {
          return (ListenRadio *)radio;
        }
        
        String getUrl() {
          return "http://mtist.as.smartstream.ne.jp/" + String(id) + "/livestream/";
        }
        
        struct playlist_t {
          private:
            station_t* station;
            String url;
            AudioGeneratorAAC * decoder = nullptr;
          public:
            playlist_t(station_t* _station, const String & _url) : station(_station), url(_url) {;}
            ~playlist_t() {
              dispose();
            }
            
            virtual void dispose() {
              clearChunks();
            }
            
            bool play() {
              getListenRadio()->select_playlist = this;
              return true;
            }
            
            station_t * getStation() {
              return station;
            }
            
            ListenRadio * getListenRadio() {
              return getStation()->getListenRadio();
            }
            
            AudioGeneratorAAC * getDecoder() {
              auto ListenRadio = getListenRadio();
              AudioGeneratorAAC * decoder;
              if(ListenRadio->decode_buffer != nullptr)
                decoder = new AudioGeneratorAAC(ListenRadio->decode_buffer, ListenRadio->decode_buffer_size, ListenRadio->enableSBR);
              else
                decoder = new AudioGeneratorAAC(ListenRadio->enableSBR);
              
              decoder->RegisterMetadataCB(ListenRadio->fnCbMetadata, ListenRadio->fnCbMetadata_data);
              decoder->RegisterStatusCB  (ListenRadio->fnCbStatus  , ListenRadio->fnCbStatus_data  );
              return decoder;
            }
            
            struct chunk_t {
              private:
                playlist_t* playlist;
                String url;
              public:
                chunk_t(playlist_t* _playlist, const String & _url) : playlist(_playlist), url(_url) {;}
                
                AudioFileSourceHTTPStream * getStream() {
                  auto ListenRadio = playlist->getListenRadio();
                  auto stream = new AudioFileSourceHTTPStream();
                  stream->RegisterMetadataCB(ListenRadio->fnCbMetadata, ListenRadio->fnCbMetadata_data);
                  stream->RegisterStatusCB  (ListenRadio->fnCbStatus  , ListenRadio->fnCbStatus_data  );
                  if(!stream->open(url.c_str())) {
                    delete stream;
                    stream = nullptr;
                  }
                  return stream;
                }
                
                String toString() {
                  return url;
                }
              private:
            };
            
            std::vector<chunk_t *> * getChunks() {
              WiFiClient client;
              HTTPClient http;
              bool success = false;
              
              clearChunks();
              
              if (http.begin(client, url)) {
                http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
                auto httpCode = http.GET();
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                  auto payload = http.getString();
                  
                  if(http.header(headerKeys[0]).equalsIgnoreCase("gzip"))
                    payload = uncompress((uint8_t *)(payload.c_str()), payload.length());
                  getInner(payload, "media", "\n", [this](const String & value) {
                    auto url = getStation()->getUrl() + value;
                    chunks.push_back(new chunk_t(this, url));
                  });
                  success = true;
                } else {
                  char bufs[url.length() + 10];
                  sprintf(bufs, "%s %d", url.c_str(), httpCode);
                  getListenRadio()->sendLog(bufs, true);
                }
                http.end();
              }
              if(success)
                return &chunks;
              else
                return nullptr;
            }
            
          private:
            void clearChunks() {
              for (auto itr : chunks)
                delete itr;
              chunks.clear();
            }
          private:
            std::vector<chunk_t *> chunks;
        };
        
        std::vector<playlist_t *> * getPlaylists() {
          WiFiClient client;
          HTTPClient http;
          bool success = false;
          
          clearPlaylists();
          
          auto url = getUrl() + "playlist.m3u8";
          if (http.begin(client, url)) {
            http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
            auto httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              auto payload = http.getString();
              if(http.header(headerKeys[0]).equalsIgnoreCase("gzip"))
                payload = uncompress((uint8_t *)(payload.c_str()), payload.length());
              
              getInner(payload, "chunklist", "\n", [this](const String & value) {
                auto url = getUrl() + value;
                playlists.push_back(new playlist_t(this, url));
              });
              success = true;
            } else {
              char bufs[url.length() + 10];
              sprintf(bufs, "%s %d", url.c_str(), httpCode);
              getListenRadio()->sendLog(bufs, true);
            }
            http.end();
          }
          if(success)
            return &playlists;
          else
            return nullptr;
        }
        
        String toString() {
          return String(id) + "/" + name;
        }
      private:
          void clearPlaylists() {
            for (auto itr : playlists)
              delete itr;
            playlists.clear();
          }
      private:
        std::vector<playlist_t *> playlists;
    };
    
    void setEnableSBR(bool sbr = false) {
      if(!decode_buffer)
        enableSBR = sbr;
    }
    
    virtual bool begin() override {
      if(!decode_buffer) {
        decode_buffer_size = enableSBR ? 89444: 26368;
        decode_buffer = heap_caps_malloc(decode_buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
        if(!decode_buffer)
          return false;
      }
      deInit();
      
      if(!bufferSize)
        bufferSize = std::max((uint32_t)(6 * 1024), std::min((uint32_t)(256 * 1024), (uint32_t)heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));
      
      buffer = new AudioFileSourceTS(bufferSize);
      buffer->identifyPid(0x101);
      
      startTask();
      
      return true;
    }
    
    virtual void stop() override {
      current_playlist = nullptr;
      chunks = nullptr;
      
      if(decoder) {
        stopDecode = 2;
        while(stopDecode == 2) {delay(100);}
        buffer->init();      
      }
      
      if(stream) {
        buffer->setSource(nullptr);
        delete stream;
        stream = nullptr;
      }
      stopDecode = 0;
    }
    
    virtual void handle() override {
#ifndef SEPARATE_DOWNLOAD_TASK
      downloadTaskCore();
#endif
    }
    
    virtual bool RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) override {
      return WebRadio::RegisterMetadataCB(fn, data) && buffer->RegisterMetadataCB(fn, data);
    }
    virtual bool RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) override {
      return WebRadio::RegisterStatusCB(fn, data) && buffer->RegisterStatusCB(fn, data);
    }
    
    String getInfoBuffer() {
      return buffer->getInfoBuffer();
    }
    
  private:
    AudioGeneratorAAC * decoder = nullptr;
    AudioFileSource * stream = nullptr;
    AudioFileSourceTS * buffer = nullptr;
    uint32_t bufferSize;
    void * decode_buffer = nullptr;
    uint32_t decode_buffer_size;
    
    station_t::playlist_t * select_playlist = nullptr;
    station_t::playlist_t * current_playlist = nullptr;
    int chunk_index;
    std::vector<station_t::playlist_t::chunk_t *> * chunks = nullptr;
    
    bool nextChunk = false;
    byte stopDecode = 0;    // 2:request stop 1:accept request
    
    AudioStatus::metadataCBFn fnCbMetadata = nullptr;
    void *fnCbMetadata_data = nullptr;
    AudioStatus::statusCBFn fnCbStatus = nullptr;
    void *fnCbStatus_data = nullptr;
    bool enableSBR = false;
    
  private:
    void deInit() {
      if(buffer) {
        delete buffer;
        buffer = nullptr;
      }
    }
    
    virtual void downloadTaskCore() override {
      if(select_station || select_playlist) {
        stop();
        chunks = nullptr;
        saveSettings = millis() + 10000;
      }
      
      if(select_station) {
        auto playlists = ((station_t *)select_station)->getPlaylists();
        if(playlists && playlists->size() > 0) {
          select_playlist = (*playlists)[0];
        } else {
          current_playlist = nullptr;
          onSerious("select_station->getPlaylists(): false");
        }
        current_station = select_station;
        if(onPlay)
          onPlay(current_station->getName(), getIndex(current_station));
        select_station = nullptr;
      }
      
      if(select_playlist) {
        auto last_station = current_station;
        current_station = select_playlist->getStation();
        current_playlist = select_playlist;
        if(last_station && last_station != current_station)
          last_station->dispose();
        if(onPlay)
          onPlay(current_station->getName(), getIndex(current_station));
        select_playlist = nullptr;
      }
      
      if(current_playlist && !chunks) {
        chunks = current_playlist->getChunks();
        if(chunks == nullptr) {
          sendLog("current_playlist->getChunks(): false", true);
          delay(5000);
          select_station = current_station;
        }
//      else if(onProgram)
//        onProgram(((station_t *)current_station)->getProgram().c_str());
        chunk_index = 0;
      }
      
      if(chunks && !stream) {
        if(chunk_index >= chunks->size())
          chunks = nullptr;
        else {
          auto chunk = (*chunks)[chunk_index];
          chunk_index++;
          
          {
            auto chunkText = chunk->toString();
            char text[7 + chunkText.length()];
            sprintf(text, "%2d/%2d %s", chunk_index, chunks->size(), chunkText.c_str());
            sendLog(text);
          }
          
          stream = chunk->getStream();
          if(stream)
            buffer->setSource(stream);
        }
      }
      
      if(current_playlist && !decoder && buffer->isFilled()) {
        decoder = current_playlist->getDecoder();
        if(!decoder->begin(buffer, out)) {
          delete decoder;
          decoder = nullptr;
          onSerious("failed: decoder->begin()");
        }
      }
      
      if(stream && stream->getPos() >= stream->getSize())
        nextChunk = true;
      
      if(stream)
        if(!buffer->fill())
          nextChunk = true;
      
      if(nextChunk) {
        buffer->setSource(nullptr);
        if(stream) {
          delete stream;
          stream = nullptr;
        }
        nextChunk = false;
      }           
      
      if (saveSettings > 0 && millis() > saveSettings) {
        saveStation(current_station);
        saveSettings = 0;
      }
    }
    
    virtual void decodeTaskCore() override {
      static auto last_loop = millis();
      auto now_millis = millis();
      
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
      
      if(!decoder) {
        if (current_playlist && now_millis - last_loop > 30000)
          onSerious("Streaming reception aborted");
        return;
      }
      
      if (decoder->isRunning()) {
        if(buffer->getSize() >= 2*1024) {
          if(decoder->loop())
            last_loop = now_millis;
          else {
            decoder->stop();
            decoder = nullptr;
            nextChunk = true;
          }
        } else if (now_millis - last_loop > 5000) {
          sendLog("Streaming reception time out", true);
          decoder->stop();
          decoder = nullptr;
          nextChunk = true;
        }
      }
    }
    
    virtual void saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) override {
      nvs_set_u8(nvs_handle, "ListenRadio", getIndex(station));
    }
    
    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override {
      uint8_t idx_8;
      if(nvs_get_u8(nvs_handle, "ListenRadio", &idx_8))
        return nullptr;
      
      if(idx_8 > getNumOfStations() - 1)
        idx_8 = getNumOfStations() - 1;
      return getStation(idx_8);
    }
};

#endif
