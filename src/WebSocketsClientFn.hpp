/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_AUDIOFILESOURCE_WEBSOCKETS_HPP_
#define _WAKWAK_KOBA_AUDIOFILESOURCE_WEBSOCKETS_HPP_

#include <functional>
#include <WebSocketsClient.h>

class WebSocketsClientFn : public WebSocketsClient {
  public:
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length) override {
      switch(type) {
        case WStype_CONNECTED:
          if(onConnect)
            onConnect();
          break;
        case WStype_DISCONNECTED:
          if(onDisconnect)
            onDisconnect();
          break;
        case WStype_TEXT:
          if(onText)
            onText(payload, length);
          break;            
        case WStype_BIN:
          if(onBinary)
            onBinary(payload, length);
          break;            
        case WStype_ERROR:      
          if(onError)
            onError(payload, length);
          break;            
        case WStype_FRAGMENT_TEXT_START:
          if(onFragmentTextStart)
            onFragmentTextStart(payload, length);
          break;            
        case WStype_FRAGMENT_BIN_START:
          if(onFragmentBinaryStart)
            onFragmentBinaryStart(payload, length);
          break;            
        case WStype_FRAGMENT:
          if(onFragment)
            onFragment(payload, length);
          break;            
        case WStype_FRAGMENT_FIN:
          if(onFragmentFin)
            onFragmentFin(payload, length);
          break;            
      }
    }

    std::function<void()> onConnect;
    std::function<void()> onDisconnect;
    std::function<void(uint8_t * payload, size_t length)> onBinary;
    std::function<void(uint8_t * payload, size_t length)> onText;
    std::function<void(uint8_t * payload, size_t length)> onError;
    std::function<void(uint8_t * payload, size_t length)> onFragmentTextStart;
    std::function<void(uint8_t * payload, size_t length)> onFragmentBinaryStart;
    std::function<void(uint8_t * payload, size_t length)> onFragment;
    std::function<void(uint8_t * payload, size_t length)> onFragmentFin;
};

#endif
