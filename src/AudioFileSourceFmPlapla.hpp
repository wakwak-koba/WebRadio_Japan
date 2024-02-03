/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_AUDIOFILESOURCE_JCBASIMUL_HPP_
#define _WAKWAK_KOBA_AUDIOFILESOURCE_JCBASIMUL_HPP_

#include <HTTPClient.h>
#include "AudioFileSourceWebSocket.hpp"
#include "common.h"

class AudioFileSourceFmPlapla : public AudioFileSourceWebSockets {
  public:
    AudioFileSourceFmPlapla(size_t buffSize = 8000) : AudioFileSourceWebSockets(buffSize), saveURL(nullptr), token(nullptr) {
      Init();
    }

    AudioFileSourceFmPlapla(uint8_t *buffer, size_t buffSize) : AudioFileSourceWebSockets(buffer, buffSize), saveURL(nullptr), token(nullptr) {
      Init();
    }

    ~AudioFileSourceFmPlapla() {
      deInit();
    }

    virtual bool open(const char *url) override {
      deInit();
      buffer.init();

      char* host = nullptr;
      char* path = nullptr;
      int port;

      auto https = new HTTPClient();
      auto client = new WiFiClientSecure();
      client->setInsecure();
      
      if (https->begin(*client, url)) {
        https->addHeader("Origin", "https://fmplapla.com");
        auto httpCode = https->POST(nullptr, 0);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String location;
          auto payload = https->getString();
          getInner(payload, "\"location\":\"", "\"", [&](const String & value) {
            location = value;
          }, true);
          
          char schema[location.length() + 1];
          host = new char[location.length() + 1];
          path = new char[location.length() + 1];
          
          if( sscanf(location.c_str(), "%[^:]://%[^:]:%hu%s", schema, host, &port, path) == 4 ) {
            getInner(payload, "\"token\":\"", "\"", [this](const String & value) {
              token = new char[value.length() + 1];
              strcpy(token, value.c_str());
            }, true);
            saveURL = new char[strlen(url) + 1];
            strcpy(saveURL, url);
          }
        }
        https->end();
      }
      delete client;
      delete https;

      if(token)
        wss.beginSSL(host, port, path, SSL_FINGERPRINT_NULL, "listener.fmplapla.com");

      if(path)
        delete []path;
      if(host)
        delete []host;

      if(token)
        return waitConnect();
      else
        return false;
    }

    String getInfoBuffer() {
      return "Buffer: " + buffer.getInfo();
    }

    uint64_t getTotalTransferred() {
      return buffer.getTotalWrite();
    }
    
  protected:
    virtual void onConnect() override {
      if(token) {
        wss.sendTXT(token);
        delete []token;
        token = nullptr;
      }
    }
    
    virtual void onBinary(uint8_t * payload, size_t length) override {
      auto f = buffer.free();
      auto segnum = payload[26];
      while(f < length && segnum > 0) {
        length -= payload[26 + segnum] + 1;
        segnum --;
      }
      if(!segnum)
        return;

      auto segdel = payload[26] - segnum;
      if(segdel > 0) {
        payload[26] = segnum;
        memmove(&payload[segdel], payload, 26 + segnum);
        payload += segdel;
      }
      
      AudioFileSourceWebSockets::onBinary(payload, length);
    }
    
  private:
    void Init() {
      saveURL = nullptr;
      token = nullptr;
      wss.setExtraHeaders("https://fmplapla.com");
    }
    
    void deInit() {
      if(token) {
        delete []token;
        token = nullptr;
      }
      if(saveURL) {
        delete []saveURL;
        token = nullptr;
      }
    }
    
  private:
    char *saveURL;
    char *token;
};

#endif
