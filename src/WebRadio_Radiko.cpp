/*
 * https://twitter.com/wakwak_koba/
 */

#include <AudioGeneratorAAC.h>
#include <AudioFileSourceHTTPStream.h>
#include "WebRadio_Radiko.h"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <base64.h>

static const char * headerKeys[] = {"x-radiko-authtoken", "x-radiko-keyoffset", "x-radiko-keylength", "Set-Cookie"};

static const char * X_Radiko_App[]          = {"pc_html5", "aSmartPhone7a"};
static const char * X_Radiko_App_Version[]  = {"0.0.1", "7.4.6"};
static const char * X_Radiko_User           = "dummy_user";
static const char * X_Radiko_Device         = "pc";

AudioGenerator * Radiko :: station_t :: playlist_t :: getDecoder() {
  auto radiko = getRadiko();
  AudioGenerator * decoder;
  if(radiko->decode_buffer != nullptr)
    decoder = new AudioGeneratorAAC(radiko->decode_buffer, radiko->decode_buffer_size, radiko->enableSBR);
  else
    decoder = new AudioGeneratorAAC(radiko->enableSBR);

  decoder->RegisterMetadataCB(radiko->fnCbMetadata, radiko->fnCbMetadata_data);
  decoder->RegisterStatusCB  (radiko->fnCbStatus  , radiko->fnCbStatus_data  );
  return decoder;
}


AudioFileSource * Radiko :: station_t :: playlist_t :: chunk_t :: getStream() {
  auto radiko = playlist->getRadiko();
  auto stream = new AudioFileSourceHTTPStream();
  stream->RegisterMetadataCB(radiko->fnCbMetadata, radiko->fnCbMetadata_data);
  stream->RegisterStatusCB  (radiko->fnCbStatus  , radiko->fnCbStatus_data  );
  if(!stream->open(url.c_str())) {
    delete stream;
    stream = nullptr;
  }
  return stream;
}


std::vector<Radiko :: station_t :: playlist_t :: chunk_t *> * Radiko :: station_t :: playlist_t :: getChunks() {
  WiFiClient client;
  HTTPClient http;
  bool success = false;

  clearChunks();

  if (http.begin(client, url)) {
    http.addHeader("X-Radiko-AuthToken", getRadiko()->token);
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      getInner(http.getString(), "http://", "\n", [this](const String & value) {
        chunks.push_back(new Radiko :: station_t :: playlist_t :: chunk_t(this, value));
      });
      success = true;
    } else {
      char bufs[url.length() + 10];
      sprintf(bufs, "%s %d", url.c_str(), httpCode);
      getRadiko()->sendLog(bufs, true);
    }
    http.end();
  }
  if(success)
    return &chunks;
  else
    return nullptr;
}

void Radiko :: station_t :: playlist_t :: clearChunks() {
  for (auto itr : chunks)
    delete itr;
  chunks.clear();
}


std::vector<Radiko :: station_t :: playlist_t *> * Radiko :: station_t :: getPlaylists() {
  WiFiClient client;
  HTTPClient http;
  bool success = false;

  clearPlaylists();

  char url[120 + id.length()];
  sprintf(url, "http://f-radiko.smartstream.ne.jp/%s/_definst_/simul-stream.stream/playlist.m3u8", id.c_str());
  if (http.begin(client, url)) {
    http.addHeader("X-Radiko-AuthToken", getRadiko()->token);
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto m3u8 = http.getString();
      getRadiko()->sendLog(m3u8.c_str(), true);
      getInner(m3u8, "http://", "\n", [this](const String & value) {
        playlists.push_back(new Radiko :: station_t :: playlist_t(this, value));
      });
      success = true;
    } else {
      char bufs[strlen(url) + 10];
      sprintf(bufs, "%s %d", url, httpCode);
      getRadiko()->sendLog(bufs, true);
    }
    http.end();
  }
  if(success)
    return &playlists;
  else
    return nullptr;
}

String Radiko :: station_t :: getProgram() {
  String title;
  WiFiClient client;
  HTTPClient http;

  char url[40 + strlen(area.c_str())];
  sprintf(url, "http://radiko.jp/v3/program/now/%s.xml", area.c_str());
  if (http.begin(client, url)) {
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto stream = http.getStreamPtr();
      stream->setTimeout(5000);
      String tagF = String("<station id=\"") + (String)id + String("\">");
      if(stream->find(tagF.c_str()) && stream->find("<title>"))
        title = htmlDecode(stream->readStringUntil('<'));
    } else {
      char bufs[strlen(url) + 10];
      sprintf(bufs, "%s %d", url, httpCode);
      getRadiko()->sendLog(bufs, true);
    }
    http.end();
  }
  return title;
}

void Radiko :: station_t :: clearPlaylists() {
  for (auto itr : playlists)
    delete itr;
  playlists.clear();
}


void Radiko :: setAuthorization(const bool clear) {
  if(clear)
    setAuthorization(nullptr, nullptr, true);
  else {
    setAuthorization(nullptr, nullptr, false);
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("WebRadio", NVS_READWRITE, &nvs_handle)) {
      size_t length1;
      size_t length2;
      if(ESP_OK == nvs_get_str(nvs_handle, "radiko_user", nullptr, &length1) && ESP_OK == nvs_get_str(nvs_handle, "radiko_pass", nullptr, &length2) && length1 && length2) {
        char user[length1 + 1];
        char pass[length2 + 1];
        if(ESP_OK == nvs_get_str(nvs_handle, "radiko_user", user, &length1) && ESP_OK == nvs_get_str(nvs_handle, "radiko_pass", pass, &length2))
          setAuthorization(user, pass, false);
      }
      nvs_close(nvs_handle);
    }
  }
}

void Radiko :: setAuthorization(const char * user, const char *pass, const bool save) {
  if(this->user) {
    delete this->user;
    this->user = nullptr;
  }
  if(this->pass) {
    delete this->pass;
    this->pass = nullptr;
  }

  if(user && strlen(user)) {
    this->user = new char[strlen(user) + 1];
    strcpy(this->user, user);

  }
  if(pass && strlen(pass)) {
    this->pass = new char[strlen(pass) + 1];
    strcpy(this->pass, pass);
  }

  if(save) {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("WebRadio", NVS_READWRITE, &nvs_handle)) {
      if(!user || !pass) {
        nvs_erase_key(nvs_handle, "radiko_user");
        nvs_erase_key(nvs_handle, "radiko_pass");
      } else {
        nvs_set_str(nvs_handle, "radiko_user", user);
        nvs_set_str(nvs_handle, "radiko_pass", pass);
      }
      nvs_commit(nvs_handle);
      nvs_close(nvs_handle);
    }
  }
}

void Radiko :: setLocation(uint8_t pref) {
  auto area = getLocation(pref);
  if(area != nullptr) {
    setLocation(area->lat, area->lon);
    select_pref = pref;
  }
}

void Radiko :: setLocation(float lat, float lon) {
  this->lat = lat;
  this->lon = lon;
}

void Radiko :: setEnableSBR(bool sbr) {
  if(!decode_buffer)
    enableSBR = sbr;
}

bool Radiko :: begin(const char *secret_key) {
  if(strlen(secret_key) != 40 && strlen(secret_key) != 32000)
    return false;     

  if(!user && !pass)
    setAuthorization();

  if(!decode_buffer) {
    decode_buffer_size = enableSBR ? 89444: 26368;
    decode_buffer = heap_caps_malloc(decode_buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if(!decode_buffer)
      return false;
  }
  uint8_t keyType = strlen(secret_key) == 40 ? 0 : 1;
  WiFiClientSecure clients;
  HTTPClient http;
  String partialkey;
  String cookie;
  String radiko_session;

  deInit();
  areaFree = false;

  if(!bufferSize)
    bufferSize = std::max((uint32_t)(6 * 1024), std::min((uint32_t)(256 * 1024), (uint32_t)heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));

  buffer = new AudioFileSourceHLS(bufferSize);

  clients.setInsecure();
  if (user && pass && http.begin(clients, "https://radiko.jp/v4/api/member/login")) {
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));

    String payload = "mail=" + urlencode(String(user)) + "&pass=" + urlencode(String(pass));
    auto httpCode = http.POST(payload);
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto payload = http.getString();
      getInner(payload, "\"radiko_session\":\"", "\"", [&](const String & value) {
        radiko_session = String("?radiko_session=") + value;
      }, true);
      getInner(payload, "\"areafree\":\"", "\"", [this](const String & value) {
        if(value.equals("1"))
          areaFree = true;
      }, true);
    } else
      onSerious((String("https://radiko.jp/v4/api/member/login ") + String(httpCode)).c_str());
    http.end();
  }

  if (http.begin(clients, "https://radiko.jp/v2/api/auth1")) {
    http.addHeader("X-Radiko-App", X_Radiko_App[keyType]);
    http.addHeader("X-Radiko-App-Version", X_Radiko_App_Version[keyType]);
    http.addHeader("X-Radiko-User", X_Radiko_User);
    http.addHeader("X-Radiko-Device", X_Radiko_Device);
    if(cookie.length())
      http.addHeader("Cookie", cookie);
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto offset = http.header(headerKeys[1]).toInt();
      auto length = http.header(headerKeys[2]).toInt();
      
      if(keyType == 0) {
        char key[length + 1];
        strncpy(key, &secret_key[offset], length);
        key[length] = 0;
        partialkey = base64::encode(key);
      } else if(keyType == 1) {
        uint8_t key[length];
        unHex(&secret_key[offset * 2], key, length * 2);
        partialkey = base64::encode(key, length);
    //          partialkey = base64::encode(&secret_key[offset], length);  // when uint8_t[]
      } 
      
      token = http.header(headerKeys[0]);
    } else
      onSerious((String("https://radiko.jp/v2/api/auth1 ") + String(httpCode)).c_str());
    http.end();
  }

  if (partialkey.length() && http.begin(clients, String("https://radiko.jp/v2/api/auth2") + radiko_session)) {
    http.addHeader("X-Radiko-App", X_Radiko_App[keyType]);
    http.addHeader("X-Radiko-App-Version", X_Radiko_App_Version[keyType]);
    http.addHeader("X-Radiko-User", X_Radiko_User);
    http.addHeader("X-Radiko-Device", X_Radiko_Device);
    http.addHeader("X-Radiko-AuthToken", token);
    http.addHeader("X-Radiko-Partialkey", partialkey);
    http.addHeader("X-Radiko-Connection", "wifi");
    if(cookie.length())
      http.addHeader("Cookie", cookie);
      
    if(keyType == 1 && lat && lon) {
      char loc[30];
      sprintf(loc, "%.6f,%.6f,gps", lat, lon);
      http.addHeader("X-Radiko-Location", loc);
    }

    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto payload = http.getString();
      area = payload.substring(0, payload.indexOf(","));
    } else
      onSerious((String("https://radiko.jp/v2/api/auth2 ") + String(httpCode)).c_str());
    http.end();
  }

  if(!area.length() || !token.length())
  return false;

  if(areaFree && !select_pref && http.begin(clients, "https://radiko.jp/v3/station/region/full.xml")) {
  auto httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
    auto stream = http.getStreamPtr();
    stream->setTimeout(5000);
    while(stream->find("<station>")) {
      auto station = new station_t(this);
      if(stream->find("<id>"))
        station->id = stream->readStringUntil('<');
      if(stream->find("<name>"))
        station->name = htmlDecode(stream->readStringUntil('<'));
      if(stream->find("<area_id>"))
        station->area = stream->readStringUntil('<');
      stations.push_back(station);
    }
  } else
    onSerious((String("https://radiko.jp/v3/station/region/full.xml ") + String(httpCode)).c_str());
  http.end();
  } else {
    if(cookie.length() && select_pref)
      area = "JP" + String(select_pref);
    char url[40 + area.length()];
    sprintf(url, "https://radiko.jp/v2/station/list/%s.xml", area.c_str());
    if (http.begin(clients, url)) {
      auto httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          getInner(http.getStreamPtr(), "station", [&](const String & value) {
            auto station = new station_t(this);
            getInner(value, "id"         , [station](const String & value) { station->id   = value; });
            getInner(value, "name"       , [station](const String & value) { station->name = htmlDecode(value); });
            station->area = area;
            stations.push_back(station);
          } );
      } else
        onSerious((String(url) + String(" ") + String(httpCode)).c_str());
      http.end();
    }
  }

  if(!stations.size())
  return false;

  startTask();
  restart(false);
  return true;
}

void Radiko :: stop() {
  current_playlist = nullptr;
  chunks = nullptr;

  if(decoder) {
    stopDecode = 2;
    while(stopDecode == 2) {delay(100);}
    buffer->init();      
  }

  if(stream) {
    buffer->setSource(nullptr);
    delete stream;
    stream = nullptr;
  }
  stopDecode = 0;
}

bool Radiko :: RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) {
  return WebRadio::RegisterMetadataCB(fn, data) && buffer->RegisterMetadataCB(fn, data);
}
bool Radiko :: RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) {
  return WebRadio::RegisterStatusCB(fn, data) && buffer->RegisterStatusCB(fn, data);
}

String Radiko :: getInfoBuffer() {
  return buffer->getInfoBuffer();
}

void Radiko :: deInit() {
  if(buffer) {
    delete buffer;
    buffer = nullptr;
  }
  WebRadio::deInit();
}

void Radiko :: downloadTaskCore() {
  if(select_station || select_playlist) {
    stop();
    chunks = nullptr;
  }

  if(select_station) {
    auto playlists = ((station_t *)select_station)->getPlaylists();
    if(playlists && playlists->size() > 0) {
      select_playlist = (*playlists)[0];
      saveSettings = millis() + 10000;
    } else {
      current_playlist = nullptr;
      onSerious("select_station->getPlaylists(): false");
  //        token.clear();  // need reAuth
    }
    select_station = nullptr;
  }

  if(select_playlist) {
    auto last_station = current_station;
    current_station = select_playlist->getStation();
    current_playlist = select_playlist;
    if(last_station && last_station != current_station)
      last_station->dispose();
    if(onPlay)
      onPlay(current_station->getName(), getIndex(current_station));
    select_playlist = nullptr;
  }

  if(current_playlist && !chunks) {
    chunks = current_playlist->getChunks();
    if(chunks == nullptr) {
      sendLog("current_playlist->getChunks(): false", true);
      delay(5000);
      select_station = current_station;
    } else if(onProgram)
      onProgram(((station_t *)current_station)->getProgram().c_str());
    chunk_index = 0;
  }

  if(chunks && !stream) {
    if(chunk_index >= chunks->size())
      chunks = nullptr;
    else {
      auto chunk = (*chunks)[chunk_index];
      chunk_index++;
      
      {
        auto chunkText = chunk->toString();
        char text[7 + chunkText.length()];
        sprintf(text, "%2d/%2d %s", chunk_index, chunks->size(), chunkText.c_str());
        sendLog(text);
      }
      
      stream = chunk->getStream();
      if(stream)
        buffer->setSource(stream);
    }
  }

  if(current_playlist && !decoder && buffer->isFilled()) {
    decoder = current_playlist->getDecoder();
    if(!decoder->begin(buffer, out)) {
      delete decoder;
      decoder = nullptr;
      onSerious("failed: decoder->begin()");
    }
  }

  if(stream && stream->getPos() >= stream->getSize())
    nextChunk = true;

  if(stream)
    if(!buffer->fill())
      nextChunk = true;

  if(nextChunk) {
    buffer->setSource(nullptr);
    if(stream) {
      delete stream;
      stream = nullptr;
    }
    nextChunk = false;
  }           

  if (saveSettings > 0 && millis() > saveSettings) {
    saveStation(current_station);
    saveSettings = 0;
  }
}

void Radiko :: decodeTaskCore() {
  static auto last_loop = millis();
  auto now_millis = millis();

  if(stopDecode > 0) {
    if(decoder) {
      if(decoder->isRunning())
        decoder->stop();
      delete decoder;
      decoder = nullptr;
    }
    stopDecode--;
    while(stopDecode) {delay(100);}
  }

  if(!decoder) {
    if (current_playlist && now_millis - last_loop > 10000)
      onSerious("Streaming reception aborted");
    return;
  }

  if (decoder->isRunning()) {
    if(buffer->getSize() >= 2*1024) {
      if(decoder->loop())
        last_loop = now_millis;
      else {
        decoder->stop();
        decoder = nullptr;
        nextChunk = true;
      }
    } else if (now_millis - last_loop > 5000) {
      sendLog("Streaming reception time out", true);
      decoder->stop();
      decoder = nullptr;
      nextChunk = true;
    }
  }
}

void Radiko :: saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) {
  char key[8 + area.length()];
  sprintf(key, "radiko_%s", area.c_str());
  nvs_set_str(nvs_handle, "radiko", ((station_t *)station)->id.c_str());
  nvs_set_str(nvs_handle, key     , ((station_t *)station)->id.c_str());
}

WebRadio :: Station * Radiko :: restoreStationCore(uint32_t nvs_handle) {
  WebRadio::Station * result = nullptr;
  size_t length = 0;
  char *value;

  // 同一エリアの前回局
  char key[8 + area.length()];   
  sprintf(key, "radiko_%s", area.c_str());
  nvs_get_str(nvs_handle, key, nullptr, &length);
  if(length) {
    value = new char[length];
    nvs_get_str(nvs_handle, key, value  , &length);
    for(auto itr : stations) {
      if(((station_t *)itr)->id.equals(value)) {
        result = itr;
        break;
      }
    }
    delete []value;
  }

  // 前回局が存在するか
  length = 0;
  nvs_get_str(nvs_handle, "radiko", nullptr, &length);
  if(length) {
    value = new char[length];
    nvs_get_str(nvs_handle, "radiko", value  , &length);
    for(auto itr : stations) {
      if(((station_t *)itr)->id.equals(value))
      {
        result = itr;
        break;
      }
    }
    delete []value;
  }
  return result;  
}

