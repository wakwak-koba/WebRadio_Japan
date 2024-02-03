/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_SIMPLE_RINGBUFFER_HPP_
#define _WAKWAK_KOBA_SIMPLE_RINGBUFFER_HPP_

template <typename T>
class SimpleRingBuffer {
  public:
    SimpleRingBuffer(size_t _buffSize) : buffSize(_buffSize), writeMode(0), deallocateBuffer(true) {
      buffer = new T[buffSize];
      if(!buffer)
        Serial.printf("failed buffer = new T[%d]\n", buffSize);
      init();
    }

    SimpleRingBuffer(T *_buffer, size_t _buffSize) : buffSize(_buffSize), buffer(_buffer), writeMode(0), deallocateBuffer(false) {
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
    
    size_t available() {
      if (readPtr > writePtr)
        return buffSize + writePtr - readPtr;
      else
        return            writePtr - readPtr;
    }
    
    size_t size() {
      return buffSize;
    }
    
    size_t free() {
      return buffSize - available() - 1;
    }
    
    size_t write(T *payload, size_t length) {
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

      auto bytesAvail = std::min(length, (size_t)(buffSize - writePtr));
      memcpy(&buffer[writePtr], &payload[0         ], sizeof(T) * bytesAvail);
      memcpy(&buffer[0       ], &payload[bytesAvail], sizeof(T) * (length - bytesAvail));
      writePtr = (writePtr + length) % buffSize;
      writeSum += length;

      return length;
    }

    size_t write(std::function<size_t(T * payload, size_t length)> func, size_t length = UINT16_MAX) {     
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
      
      size_t wlen = 0;
      auto bytesAvail = std::min(length, (size_t)(buffSize - writePtr));
      if(bytesAvail > 0)
        wlen = func(&buffer[writePtr], bytesAvail);

      if(wlen >= bytesAvail && length - wlen > 0)
        wlen += func(&buffer[0], length - wlen );
      
      writePtr = (writePtr + wlen) % buffSize;
      writeSum += wlen;

      return wlen;
    }

    size_t read(T *data, size_t length) {
      length = std::min(length, available());
      if (!data || !length) return 0;

      auto bytesAvail = std::min(length, (size_t)(buffSize - readPtr));
      memcpy(&data[0         ], &buffer[readPtr], sizeof(T) * bytesAvail);
      memcpy(&data[bytesAvail], &buffer[0      ], sizeof(T) * (length - bytesAvail));
      readPtr = (readPtr + length) % buffSize;
      readSum += length;

      return length;
    }

    size_t seek(size_t length) {
      size_t result = std::min( available(), length );
      readPtr = (readPtr + length) % buffSize;
      return result;
    }

    String getInfo() {
      return "Available:" + String(available()) 
        + " Free:" + String(free()) 
        + " WritePtr:" + String(writePtr) 
        + " ReadPtr:" + String(readPtr) 
        + " WriteSum:" + ( writeSum > (uint64_t)UINT32_MAX ? String((size_t)(writeSum / 1024)) + "K" : String((size_t)writeSum) )
        + " readSum:"  + ( readSum  > (uint64_t)UINT32_MAX ? String((size_t)( readSum / 1024)) + "K" : String((size_t) readSum) );
    }
    
    uint64_t getTotalRead() {
      return readSum;
    }
    
    uint64_t getTotalWrite() {
      return writeSum;
    }
    
  private:
    T *buffer;
    size_t buffSize;
    size_t readPtr;
    size_t writePtr;
    uint8_t writeMode;     // 空きが足りないときの挙動 0:writeしない 1:空きの分だけ 2:先頭を削除して強制挿入
    bool deallocateBuffer;
    uint64_t writeSum;
    uint64_t readSum;
};

#endif
