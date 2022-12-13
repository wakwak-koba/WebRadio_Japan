/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_AUDIOFILESOURCE_TS_HPP_
#define _WAKWAK_KOBA_AUDIOFILESOURCE_TS_HPP_

#include "AudioFileSourceHLS.hpp"
#include "SimpleRingBuffer.hpp"

class AudioFileSourceTS : public AudioFileSourceHLS {
  public:
    AudioFileSourceTS(uint16_t _buffSize) : AudioFileSourceHLS(_buffSize) {}
    AudioFileSourceTS(uint8_t *_buffer, uint16_t _buffSize) : AudioFileSourceHLS(_buffer, _buffSize) {}
    AudioFileSourceTS(AudioFileSource * _src, uint16_t _buffSize) : AudioFileSourceHLS(_src, _buffSize) {}
    AudioFileSourceTS(AudioFileSource * _src, uint8_t *_buffer, uint16_t _buffSize) : AudioFileSourceHLS(_src, _buffer, _buffSize) {}

    void identifyPid(uint16_t _pid) {
      PID = _pid;
    }
  
  protected:
    virtual uint32_t readInternal(void *dst, uint32_t length, bool nonblock) override {
      uint32_t rLen = 0;
      uint8_t header[4];
      auto d = (uint8_t *)dst;
      while(buffer.available() >= 188 && length >= 184) {
        uint16_t read_len = 188;
        read_len -= buffer.read(header, 4);
        
        uint8_t syncbyte = header[0];
        uint8_t transport_error_indicator = header[1] >> 7;
        uint8_t payload_unit_start_indicator = (header[1] >> 6) & 0x01;
        uint8_t transport_priority = (header[1] >> 5) & 0x01;
        uint16_t pid = (((uint16_t)header[1] & 0x1F) << 8) + header[2];
        uint8_t transport_scrambling_control = header[3] >> 6;
        uint8_t adaptation_field_control = (header[3] >> 4) & 0x03;
        uint8_t continuity_counter = header[3] & 0x0f;
        
        if(syncbyte == 0x47 && adaptation_field_control & 0x01) {   // has payload
          if(adaptation_field_control & 0x02) { // has adaptation_field
            uint8_t adaptation_field_len;
            read_len -= buffer.read(&adaptation_field_len, 1);
            read_len -= buffer.read(d, adaptation_field_len);
          }
          
          read_len = buffer.read(d, read_len);
          
          if(pid == 0xFFFF || pid == PID) {
            rLen += read_len;
            d += read_len;
            length -= read_len;
          }
        }
      }
      return rLen;
    }

  private:
    uint16_t PID = 0xFFFF;
};

#endif
