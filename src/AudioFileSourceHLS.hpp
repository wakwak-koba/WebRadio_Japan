/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_AUDIOFILESOURCE_HLS_HPP_
#define _WAKWAK_KOBA_AUDIOFILESOURCE_HLS_HPP_

#include <AudioFileSource.h>
#include "SimpleRingBuffer.hpp"

class AudioFileSourceHLS : public AudioFileSource {
  public:
    AudioFileSourceHLS(size_t _buffSize) : buffer(_buffSize) {
      init();
    }
    
    AudioFileSourceHLS(uint8_t *_buffer, size_t _buffSize) : buffer(_buffer, _buffSize) {
      init();
    }

    AudioFileSourceHLS(AudioFileSource * _src, uint16_t _buffSize) : buffer(_buffSize) {
      init();
      setSource(_src);
    }
    
    AudioFileSourceHLS(AudioFileSource * _src, uint8_t *_buffer, uint16_t _buffSize) : buffer(_buffer, _buffSize) {
      init();
      setSource(_src);
    }

    virtual bool setSource(AudioFileSource * _src, bool _fill = false) {
      src = _src;
      if(src)
        fill(!_fill);
      return true;
    }
    
    virtual bool close() override {
      return src && src->close();
    }

    virtual bool isOpen() override {
      return src;
    }

    virtual size_t getSize() override {
      return buffer.available();
    }

    virtual bool loop() override {
      return src && src->loop();
    }

    virtual size_t read(void *dst, size_t length) override {
      return readInternal(dst, length, false);
    }

    virtual size_t readNonBlock(void *dst, size_t length) override {
      return readInternal(dst, length, true);
    }

    virtual void setTimeout(uint16_t _timeout) {
      src = nullptr;
      timeout = _timeout;
    }

    bool isFilled() {
      return !buffer.free();
    }
    
    bool fill(bool nonBlock = true) {
      if(!src) return false;

      uint16_t wLen;
      auto f = buffer.free();
      auto source = src;
      if(nonBlock)
        wLen = buffer.write(
          [source](uint8_t * payload, uint16_t length) {
            auto result = (uint16_t)source->readNonBlock((void *)payload, length);
            return result;
          } );
      else {
        wLen = buffer.write(
          [source](uint8_t * payload, uint16_t length) {
            auto result = (uint16_t)source->read((void *)payload, length);
            return result;
          } );
      }

      if(!millis_last_read || wLen > 0 || buffer.available())
        millis_last_read = millis();
      else if(millis() - millis_last_read > timeout) {
        millis_last_read = 0;
        return false;
      }
      return true;
    }
    
    void init() {
      setTimeout(5000);
      src = nullptr;
      buffer.init();
    }

    String getInfoBuffer() {
      return "Buffer: " + buffer.getInfo();
    }

    enum { STATUS_TOO_SLOW=10 };

  protected:
    AudioFileSource * src;
    SimpleRingBuffer<uint8_t> buffer;
    uint16_t timeout;

    virtual size_t readInternal(void *dst, size_t length, bool nonblock) {
      auto rLen = buffer.read((uint8_t *)dst, length);
      if(length && rLen < length)
        cb.st(STATUS_TOO_SLOW, PSTR("receive is too slow.."));
      return rLen;
    }

  private:
    uint64_t millis_last_read = 0;
};

#endif
