/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_SIMPLE_RINGBUFFER_HPP_
#define _WAKWAK_KOBA_SIMPLE_RINGBUFFER_HPP_

#include <functional>

template <typename T>
class SimpleRingBuffer {
  public:
    SimpleRingBuffer(uint32_t _buffSize) : buffSize(_buffSize + 1), writeMode(0), deallocateBuffer(true) {
      buffer = new T[buffSize];
      init();
    }

    SimpleRingBuffer(T *_buffer, uint32_t _buffSize) : buffSize(_buffSize), buffer(_buffer), writeMode(0), deallocateBuffer(false) {
      init();
    }

    ~SimpleRingBuffer() {
      if(deallocateBuffer)
        delete []buffer;
    }

    void init() {
      readPtr = 0;
      writePtr = 0;
      readSum = 0;
      writeSum = 0;
    }

    void clear() {
      init();
    }

    uint32_t available() {
      if (readPtr > writePtr)
        return buffSize + writePtr - readPtr;
      else
        return            writePtr - readPtr;
    }

    uint32_t size() {
      return buffSize;
    }

    uint32_t free() {
      return buffSize - available() - 1;
    }

    uint32_t write(T *payload, uint32_t length) {
      if (!payload || !length || length > buffSize) return 0;

      auto f = free();
      if (length > f) {
        if (writeMode == 2)
          readPtr = (readPtr + length - f) % buffSize;
        else {
          if (writeMode == 1)
            length = f;
          if (!length || !writeMode)
            return 0;
        }
      }

      auto bytesAvail = std::min(length, (uint32_t)(buffSize - writePtr));
      memcpy(&buffer[writePtr], &payload[0         ], sizeof(T) * bytesAvail);
      memcpy(&buffer[0       ], &payload[bytesAvail], sizeof(T) * (length - bytesAvail));
      writePtr = (writePtr + length) % buffSize;
      writeSum += length;

      return length;
    }

    uint32_t write(std::function<uint32_t(T * payload, uint32_t length)> func, uint32_t length = UINT16_MAX) {     
/*    if(!length)
        length = f;
      else if (length > f) {
        if (writeMode == 2)
          readPtr = (readPtr + length - f) % buffSize;
        else {
          if (writeMode == 1)
            length = f;
          if (!length || !writeMode)
            return 0;
        }
      }
      if(!length)
        return 0;*/

      auto f = free();
      length = std::min(length, f);
      if(!length) return 0;
      
      uint32_t wlen = 0;
      auto bytesAvail = std::min(length, (uint32_t)(buffSize - writePtr));
      if(bytesAvail > 0)
        wlen = func(&buffer[writePtr], bytesAvail);

      if(wlen >= bytesAvail && length - wlen > 0)
        wlen += func(&buffer[0], length - wlen );
      
      writePtr = (writePtr + wlen) % buffSize;
      writeSum += wlen;

      return wlen;
    }

    bool peek(T *data) {
      if (available() < 1) return false;
      memcpy(data, &buffer[readPtr], sizeof(T));
      return true;
    }

    uint32_t read(T *data, uint32_t length) {
      length = std::min(length, available());
      if (!data || !length) return 0;

      auto bytesAvail = std::min(length, (uint32_t)(buffSize - readPtr));
      memcpy(&data[0         ], &buffer[readPtr], sizeof(T) * bytesAvail);
      memcpy(&data[bytesAvail], &buffer[0      ], sizeof(T) * (length - bytesAvail));
      readPtr = (readPtr + length) % buffSize;
      readSum += length;

      return length;
    }

    uint32_t seek(uint32_t length) {
      uint32_t result = std::min( available(), length );
      readPtr = (readPtr + length) % buffSize;
      return result;
    }

    String getInfo() {
      return "Available:" + String(available()) 
        + " Free:" + String(free()) 
        + " WritePtr:" + String(writePtr) 
        + " ReadPtr:" + String(readPtr) 
        + " WriteSum:" + ( writeSum > (uint64_t)UINT32_MAX ? String((uint32_t)(writeSum / 1024)) + "K" : String((uint32_t)writeSum) )
        + " readSum:"  + ( readSum  > (uint64_t)UINT32_MAX ? String((uint32_t)( readSum / 1024)) + "K" : String((uint32_t) readSum) );
    }
    
    uint64_t getTotalRead() {
      return readSum;
    }
    
    uint64_t getTotalWrite() {
      return writeSum;
    }

  private:
    T *buffer;
    uint32_t buffSize;
    uint32_t readPtr;
    uint32_t writePtr;
    uint8_t writeMode;     // 空きが足りないときの挙動 0:writeしない 1:空きの分だけ 2:先頭を削除して強制挿入
    bool deallocateBuffer;
    uint64_t writeSum;
    uint64_t readSum;
};

#endif
