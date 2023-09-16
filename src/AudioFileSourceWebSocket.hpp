/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_AUDIOFILESOURCE_WEBSOCKET_HPP_
#define _WAKWAK_KOBA_AUDIOFILESOURCE_WEBSOCKET_HPP_

#include <AudioFileSource.h>
#include "WebSocketsClientFn.hpp"
#include "SimpleRingBuffer.hpp"

class AudioFileSourceWebSockets : public AudioFileSource {
  public:
    AudioFileSourceWebSockets(uint16_t _buffSize) : buffer(_buffSize) {
      Init();
    }
    
    AudioFileSourceWebSockets(uint8_t *_buffer, uint16_t _buffSize) : buffer(_buffer, _buffSize) {
      Init();
    }
    
    virtual bool close() override {
      wss.disconnect();
      return true;
    }

    virtual bool isOpen() override {
      return wss.isConnected();
    }

    virtual uint32_t getSize() override {
      return buffer.available();
    }

    virtual bool loop() override {
      return true;
    }

    virtual uint32_t read(void *data, uint32_t length) override {
      auto st = millis();
      while(getSize() < 1 && millis() - st < timeout) {
        delay(1);
      }
      return buffer.read((uint8_t *)data, length);
    }

    virtual void setTimeout(uint16_t _timeout) {
      timeout = _timeout;
    }
    
    bool fill() {
      if(buffer.size() < 10 * 1024 || buffer.free() > 5 * 1024)
        wss.loop();
      return true;
    }
    
    enum { STATUS_TOO_SLOW=10 };
    
  protected:   
    virtual bool waitConnect() {
      if(wss.isConnected())
        return true;

//    cb.st(STATUS_CONNECTING, PSTR("Attempting to connect"));
      auto st = millis();
      while(!wss.isConnected() && millis() - st < timeout)
        wss.loop();

      return wss.isConnected();
    }
    
    virtual void onConnect() { ; }
    
    virtual void onBinary(uint8_t * payload, size_t length) {
      buffer.write(payload, length);
    }

  private:
    void Init() {
      setTimeout(5000);
      wss.onConnect = [&]() {
        onConnect();
      };
      wss.onBinary = [&](uint8_t * payload, size_t length) {
        onBinary(payload, length);
      };
    }

  protected:
    WebSocketsClientFn  wss;   
    SimpleRingBuffer<uint8_t> buffer;
    uint16_t timeout;  
};

#endif