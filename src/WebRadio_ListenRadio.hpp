/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_
#define _WAKWAK_KOBA_WEBRADIO_LISTENRADIO_HPP_

#include <AudioGenerator.h>
#include "AudioFileSourceTS.hpp"
#include "WebRadio.h"
#include "common.h"

class ListenRadio : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3)
#else
    ListenRadio(AudioOutput * _out, int cpuDecode, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 2560, 3, 1 - cpuDecode, 2560)
#endif
    {}
    
    ~ListenRadio() {
      if(decode_buffer)
        free(decode_buffer);
    }
    
    class station_t : public WebRadio::Station {
      private:
        const uint16_t id;
        const char* name;

      public:
        station_t(ListenRadio* _radio, const uint16_t _id, const char * _name) : Station(_radio), id(_id), name(_name)  {}
        ~station_t() { dispose(); }
        
        virtual void dispose() override { clearPlaylists(); }
        virtual const char * getName() override { return name; }
        virtual bool play() override { getListenRadio()->play(this); return true; }
        ListenRadio * getListenRadio() { return (ListenRadio *)radio; }
        String getUrl() { return "http://mtist.as.smartstream.ne.jp/" + String(id) + "/livestream/"; }
        
        struct playlist_t {
          private:
            station_t* station;
            String url;
            AudioGenerator * decoder = nullptr;
          public:
            playlist_t(station_t* _station, const String & _url) : station(_station), url(_url) {;}
            ~playlist_t() { dispose(); }
            
            virtual void dispose() { clearChunks(); }
            bool play() { getListenRadio()->select_playlist = this; return true; }
            station_t * getStation() { return station; }
            ListenRadio * getListenRadio() { return getStation()->getListenRadio(); }
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
        
        String toString() {
          return String(id) + "/" + name;
        }
      private:
          void clearPlaylists();
      private:
        std::vector<playlist_t *> playlists;
    };
    
    void setEnableSBR(bool sbr = false);
    virtual bool begin() override;
    virtual void stop() override;
    virtual void handle() override {
#ifndef SEPARATE_DOWNLOAD_TASK
      downloadTaskCore();
#endif
    }
    
    virtual bool RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) override;
    virtual bool RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) override;
    String getInfoBuffer();
    
  private:
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
    void deInit();
    virtual void downloadTaskCore() override;
    virtual void decodeTaskCore() override;
    virtual void saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) override;
    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override;
};

#endif
