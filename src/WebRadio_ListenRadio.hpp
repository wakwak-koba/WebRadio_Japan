/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_
#define _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_

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
/*00*/	{30058, "FM ABASHIRI"},
/*01*/	{30074, "FMりべーる"},
/*02*/	{30045, "FMねむろ"},
/*03*/	{30029, "FMくしろ"},
/*04*/	{30016, "FM JAGA"},
/*05*/	{30038, "FM WING"},
/*06*/	{30032, "さっぽろ村ラジオ"},
/*07*/	{30044, "RADIOワンダーストレージ FMドラマシティ"},
/*08*/	{30005, "三角山放送局（札幌市西区）"},
/*09*/	{30034, "ラジオカロスサッポロ"},
/*10*/	{30090, "FMアップル"},
/*11*/	{30025, "FMおたる"},
/*12*/	{30015, "FMメイプル"},
/*13*/	{30087, "wi-radio"},
/*14*/	{30079, "BeFM"},
/*15*/	{30050, "カシオペアFM"},
/*16*/	{30017, "ラヂオもりおか"},
/*17*/	{30097, "みやこハーバーラジオ"},
/*18*/	{30094, "ラヂオ気仙沼"},
/*19*/	{30037, "ラジオ石巻"},
/*20*/	{30056, "BAY WAVE"},
/*21*/	{30007, "RADIO3"},
/*22*/	{30018, "fmいずみ"},
/*23*/	{30092, "なとらじ801"},
/*24*/	{30089, "鹿角きりたんぽFM"},
/*25*/	{30014, "エフエム椿台"},
/*26*/	{30076, "横手かまくらエフエム"},
/*27*/	{30030, "FMゆーとぴあ"},
/*28*/	{30019, "FM Mot.com"},
/*29*/	{30020, "KOCOラジ"},
/*30*/	{30009, "FMいわき"},
/*31*/	{30023, "FMひたち"},
/*32*/	{30075, "たかはぎFM"},
/*33*/	{30022, "FMぱるるん"},
/*34*/	{30021, "FMうしくうれしく放送"},
/*35*/	{30043, "まえばしCITYエフエム"},
/*36*/	{30047, "FMふっかちゃん"},
/*37*/	{30048, "FMわたらせ"},
/*38*/	{30002, "フラワーラジオ"},
/*39*/	{30096, " ハローハッピー・こしがやエフエム"},
/*40*/	{30008, "REDS WAVE"},
/*41*/	{30035, "FM Kawaguchi"},
/*42*/	{30026, "775ライブリーFM"},
/*43*/	{30042, "RadioCity 中央エフエム"},
/*44*/	{30036, "レインボータウンFM"},
/*45*/	{30027, "エフエム世田谷"},
/*46*/	{30051, "コマラジ"},
/*47*/	{30039, "調布FM"},
/*48*/	{30033, "FMたちかわ"},
/*49*/	{30081, "Tokyo Star Radio(八王子FM)"},
/*50*/	{30046, "かわさきＦＭ"},
/*51*/	{30065, "マリンFM"},
/*52*/	{30064, "エフエム戸塚"},
/*53*/	{30057, "ＦＭカオン"},
/*54*/	{30040, "エフエムEGAO"},
/*55*/	{30001, "FM N1"},
/*56*/	{30006, "ラジオ・ミュー"},
/*57*/	{30012, "敦賀FM"},
/*58*/	{30061, "ラジオスイート"},
/*59*/	{30071, "FM87.0 RADIO MIX KYOTO"},
/*60*/	{30082, "京都三条ラジオカフェ"},
/*61*/	{30063, "FMおとくに"},
/*62*/	{30073, "FMたんご"},
/*63*/	{30078, "BAN-BANラジオ"},
/*64*/	{30013, "FM ジャングル"},
/*65*/	{30053, "DARAZ　FM"},
/*66*/	{30010, "FMびざん"},
/*67*/	{30024, "FM815（高松）"},
/*68*/	{30070, "FM SUN"},
/*69*/	{30052, "AIR STATION HIBIKI"},
/*70*/	{30077, "COMI&#215;TEN"},
/*71*/	{30085, "チョクラジ"},
/*72*/	{30088, "FMのべおか"},
/*73*/	{30072, "エフエムたつごう"},
/*74*/	{30054, "あまみエフエム"},
/*75*/	{30080, "せとラジ"},
/*76*/	{30095, "FMやんばる"},
/*77*/	{30011, "FMうるま"},
/*78*/	{30003, "FMニライ"},
/*79*/	{30066, "オキラジ"},
/*80*/	{30093, "FMぎのわん"},
/*81*/	{30098, "ぎのわんシティFM"},
/*82*/	{30068, "fm那覇"},
/*83*/	{30055, "FMよなばる"},
/*84*/	{30083, "FMとよみ"},
/*85*/	{30069, "FMいしがきサンサンラジオ"},
/*86*/	{30120, "Heart FM"},
/*87*/	{30041, "練馬放送"},
/*88*/	{30119, "たこみんFM"},
/*89*/	{30099, "試験放送A"},
};
static byte initial_station = 8;   // 三角山放送局（札幌市西区）

static const char * headerKeys[] = {"Content-Encoding"};

class ListenRadio : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint16_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2048, 3) {
#else
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint16_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2048, 3, 1 - cpuDecode, 2560) {
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
          clearPlaylists();
        }
        
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
        bufferSize = std::max(6 * 1024, (int)std::min( (uint32_t)UINT16_MAX, heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));
      
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
    uint16_t bufferSize;
    void * decode_buffer = nullptr;
    size_t decode_buffer_size;
    
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
          onError("select_station->getPlaylists(): false");
          current_playlist = nullptr;
        }
        current_station = select_station;
        if(onPlay)
          onPlay(current_station->getName(), getIndex(current_station));
        select_station = nullptr;
      }
      
      if(select_playlist) {
        current_station = select_playlist->getStation();
        current_playlist = select_playlist;
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
      
      if(!decoder)
        return;
        
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
