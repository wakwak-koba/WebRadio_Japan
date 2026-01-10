/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_RADIKO_HPP_
#define _WAKWAK_KOBA_WEBRADIO_RADIKO_HPP_

#include <AudioGenerator.h>
#include "AudioFileSourceHLS.hpp"
#include "WebRadio.h"
#include "common.h"

class Radiko : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    Radiko(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3) {
#else
    Radiko(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3, 1 - cpuDecode, 2560) {
#endif
//    decode_buffer = heap_caps_malloc(decode_buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    }
    
    ~Radiko() {
      setAuthorization(nullptr, nullptr);
      if(decode_buffer)
        free(decode_buffer);
    }
    
    class station_t : public WebRadio::Station {
      public:
        String id;
        String name;
        String area;
        
        station_t(Radiko* _radiko) : Station(_radiko) {;}
        ~station_t() { dispose(); }
        
        virtual void dispose() override { clearPlaylists(); }
        virtual const char * getName() override { return name.c_str(); }
        virtual bool play() override { getRadiko()->play(this); return true; }
        Radiko * getRadiko() { return (Radiko *)radio; }
        
        struct playlist_t {
          private:
            station_t* station;
            String url;
            AudioGenerator * decoder = nullptr;
          public:
            playlist_t(station_t* _station, const String & _url) : station(_station), url(_url) {;}
            ~playlist_t() { dispose(); }
            
            virtual void dispose() { clearChunks(); }
            bool play() { getRadiko()->select_playlist = this; return true; }
            station_t * getStation() { return station; }
            Radiko * getRadiko() { return getStation()->getRadiko(); }
            AudioGenerator * getDecoder();
            
            struct chunk_t {
              private:
                playlist_t* playlist;
                String url;
              public:
                chunk_t(playlist_t* _playlist, const String & _url) : playlist(_playlist), url(_url) {;}
                
                AudioFileSource * getStream();
                
                String toString() {
                  return url;
                }
              private:
            };
            
            std::vector<chunk_t *> * getChunks();
          private:
            void clearChunks();
          private:
            std::vector<chunk_t *> chunks;
        };
        
        std::vector<playlist_t *> * getPlaylists();
        String getProgram();
        
        String toString() {
          return area + "/" + id + "/" + name;
        }
      private:
          void clearPlaylists();
      private:
        std::vector<playlist_t *> playlists;
    };
    
    void setAuthorization(const bool clear = false);
    void setAuthorization(const char * user, const char *pass, const bool save = false);
    void setLocation() { setLocation(0.0F, 0.0F); }
    void setLocation(uint8_t pref);
    void setLocation(float lat, float lon);
    void setEnableSBR(bool sbr = false);
    
    virtual bool begin() override { return begin("bcd151073c03b352e1ef2fd66c32209da9ca0afa"); }
    bool begin(const char *secret_key);
    virtual void stop() override;
    
    virtual void handle() override {
      if(!token.length()) {
#ifdef SEPARATE_DOWNLOAD_TASK
        vTaskSuspend(download_handle);
#endif
        sendLog("authentication process");
        auto idx = getIndex(current_station);
        
        if(!begin()) {
          delay(5000);
          onSerious("failed: radio.begin()");
          for(;;);
        }
        
        play(stations[idx]);
#ifdef SEPARATE_DOWNLOAD_TASK
        vTaskResume(download_handle);
#endif
      }
#ifndef SEPARATE_DOWNLOAD_TASK
      else
        downloadTaskCore();
#endif
    }
    
    virtual bool RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) override;
    virtual bool RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) override;
    String getInfoBuffer();
    
  private:
    AudioFileSource * stream = nullptr;
    AudioFileSourceHLS * buffer = nullptr;
    uint32_t bufferSize;
    void * decode_buffer = nullptr;
    uint32_t decode_buffer_size;
    
    char * user = nullptr;;
    char * pass = nullptr;;
    float lat = 0.0F;
    float lon = 0.0F;
    bool areaFree = false;
    
    String token;
    String area;
    
    uint8_t                 select_pref = 0;
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
    void deInit();
    virtual void downloadTaskCore() override;   
    virtual void decodeTaskCore() override;
    virtual void saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) override;
    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override;
};

#endif
