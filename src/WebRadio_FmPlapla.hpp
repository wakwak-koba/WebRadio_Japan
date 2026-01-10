/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_FmPlapla_HPP_
#define _WAKWAK_KOBA_WEBRADIO_FmPlapla_HPP_

#include <AudioGenerator.h>
#include "AudioFileSourceWebSocket.hpp"
#include "WebRadio.h"

class FmPlapla : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    FmPlapla(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, priorityDecode)
#else
    FmPlapla(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, priorityDecode, 1 - cpuDecode, 4096)
#endif
    {}
#ifndef SEPARATE_DOWNLOAD_TASK
    FmPlapla(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, 3)
#else
    FmPlapla(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, 3, 1 - cpuDecode, 4096)
#endif
    {}

    ~FmPlapla() {
      if(source)
        delete source;
    }

    class station_t : public WebRadio::Station {
      public:
        station_t(FmPlapla* _radio, const char * _name, const char * _id) : Station(_radio), name(_name), id(_id)  {}
        
        FmPlapla * getFmPlapla() { return (FmPlapla *)radio; }
        virtual const char * getName()  { return name; }
        virtual bool play() override { getFmPlapla()->select_station = this; return true; }
        String getUrl() { return "https://fmplapla.com/api/select_stream?station=" + String(id) + "&burst=5"; }
        
        AudioGenerator * getDecoder();
        AudioFileSourceWebSockets * getSource();
        
      private:
        const char* name;
        const char* id;
    };

    virtual bool begin() override;
    void stop();
    virtual void handle() override {
#ifndef SEPARATE_DOWNLOAD_TASK
      downloadTaskCore();
#endif
    }
    
  private:
    virtual void downloadTaskCore() override;
    virtual void decodeTaskCore() override;
    virtual void saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) override;
    virtual WebRadio::Station * restoreStationCore(uint32_t nvs_handle) override;
      
  private:
    AudioFileSourceWebSockets * source = nullptr;
    uint8_t * buffer = nullptr;
    uint32_t bufferSize;
    byte stopDecode = 0;    // 2:request stop 1:accept request

};

#endif
