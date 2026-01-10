/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_
#define _WAKWAK_KOBA_WEBRADIO_JCBASIMUL_HPP_

#include <AudioGenerator.h>
#include "AudioFileSourceWebSocket.hpp"
#include "WebRadio.h"

class Jcbasimul : public WebRadio {
  public:
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, priorityDecode)
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, const UBaseType_t priorityDecode = 3, const uint32_t buffSize = 0) : bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, priorityDecode, 1 - cpuDecode, 4096)
#endif
    {}
#ifndef SEPARATE_DOWNLOAD_TASK
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, 3)
#else
    Jcbasimul(AudioOutput * _out, int cpuDecode, uint8_t * _buffer, const uint32_t buffSize) : buffer(_buffer), bufferSize(buffSize), WebRadio(_out, cpuDecode, 16*1024, 3, 1 - cpuDecode, 4096)
#endif
    {}

    ~Jcbasimul() {
      if(source)
        delete source;
    }

    class station_t : public WebRadio::Station {
      public:
        station_t(Jcbasimul* _radio, const char * _name, const char * _id) : Station(_radio), name(_name), id(_id)  {}
        
        Jcbasimul * getJcbasimul() { return (Jcbasimul *)radio; }
        virtual const char * getName()  { return name; }
        virtual bool play() override { getJcbasimul()->select_station = this; return true; }
        String getUrl() { return "https://api.radimo.smen.biz/api/v1/select_stream?channel=0&quality=high&burst=5&station=" + String(id); }
        
        AudioGenerator * getDecoder();
        AudioFileSourceWebSockets * getSource();
        
      private:
        const char* name;
        const char* id;
    };

    virtual bool begin() override;
    virtual void stop() override;
    virtual void handle() override {
#ifndef SEPARATE_DOWNLOAD_TASK
      downloadTaskCore();
#endif
    }
    
    String getInfoBuffer();
    
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
