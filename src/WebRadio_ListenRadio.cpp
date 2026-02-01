/*
 * https://twitter.com/wakwak_koba/
 */

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <AudioFileSourceHTTPStream.h>
#include <AudioGeneratorAAC.h>
#include "WebRadio_ListenRadio.h"

struct station_t {
    const char * name;
    const uint16_t id;
};

static const station_t station_list[] = {
/*00*/	{"FM ABASHIRI / 北海道",30058},
/*01*/	{"FMりべーる / 北海道",30074},
/*02*/	{"FMねむろ / 北海道",30045},
/*03*/	{"FMくしろ / 北海道",30029},
/*04*/	{"FMはまなす / 北海道",30004},
/*05*/	{"FM JAGA / 北海道",30016},
/*06*/	{"FM WING / 北海道",30038},
/*07*/	{"さっぽろ村ラジオ / 北海道",30032},
/*08*/	{"RadioYAMASHO FMドラマシティ / 北海道",30044},
/*09*/	{"三角山放送局（札幌市西区） / 北海道",30005},
/*10*/	{"ラジオカロスサッポロ / 北海道",30034},
/*11*/	{"FMアップル / 北海道",30090},
/*12*/	{"FMおたる / 北海道",30025},
/*13*/	{"FMメイプルきたひろボールパークラジオ / 北海道",30015},
/*14*/	{"wi-radio / 北海道",30087},
/*15*/	{"BeFM / 東北",30079},
/*16*/	{"カシオペアFM / 東北",30050},
/*17*/	{"ラヂオもりおか / 東北",30017},
/*18*/	{"みやこハーバーラジオ / 東北",30097},
/*19*/	{"ラヂオ気仙沼 / 東北",30094},
/*20*/	{"ラジオ石巻 / 東北",30037},
/*21*/	{"BAY WAVE / 東北",30056},
/*22*/	{"RADIO3 / 東北",30007},
/*23*/	{"fmいずみ / 東北",30018},
/*24*/	{"なとらじ801 / 東北",30092},
/*25*/	{"鹿角きりたんぽFM / 東北",30089},
/*26*/	{"エフエム椿台 / 東北",30014},
/*27*/	{"横手かまくらエフエム / 東北",30076},
/*28*/	{"FMゆーとぴあ / 東北",30030},
/*29*/	{"FM Mot.com / 東北",30019},
/*30*/	{"KOCOラジ / 東北",30020},
/*31*/	{"FMいわき / 東北",30009},
/*32*/	{"FMひたち / 関東",30023},
/*33*/	{"FMぱるるん / 関東",30022},
/*34*/	{"FMうしくうれしく放送 / 関東",30021},
/*35*/	{"まえばしラジオ / 関東",30043},
/*36*/	{"FMふっかちゃん / 関東",30047},
/*37*/	{"FMわたらせ / 関東",30048},
/*38*/	{"フラワーラジオ / 関東",30002},
/*39*/	{" ハローハッピー・こしがやエフエム / 関東",30096},
/*40*/	{"REDS WAVE / 関東",30008},
/*41*/	{"FM Kawaguchi / 関東",30035},
/*42*/	{"775ライブリーFM / 関東",30026},
/*43*/	{"レインボータウンFM / 関東",30036},
/*44*/	{"エフエム世田谷 / 関東",30027},
/*45*/	{"調布FM / 関東",30039},
/*46*/	{"FMたちかわ / 関東",30033},
/*47*/	{"Tokyo Star Radio(八王子FM) / 関東",30081},
/*48*/	{"ＦＭカオン / 関東",30057},
/*49*/	{"エフエムEGAO / 東海",30040},
/*50*/	{"FM N1 / 北信越",30001},
/*51*/	{"ラジオ・ミュー / 北信越",30006},
/*52*/	{"敦賀FM / 北信越",30012},
/*53*/	{"ラジオスイート / 近畿",30061},
/*54*/	{"エフエム花 / 近畿",30067},
/*55*/	{"FM87.0 RADIO MIX KYOTO / 近畿",30071},
/*56*/	{"京都三条ラジオカフェ / 近畿",30082},
/*57*/	{"FMおとくに / 近畿",30063},
/*58*/	{"FMたんご / 近畿",30073},
/*59*/	{"BAN-BANラジオ / 近畿",30078},
/*60*/	{"FM ジャングル / 近畿",30013},
/*61*/	{"DARAZ　FM / 中国・四国",30053},
/*62*/	{"FMびざん / 中国・四国",30010},
/*63*/	{"FM815（高松） / 中国・四国",30024},
/*64*/	{"AIR STATION HIBIKI / 九州・沖縄",30052},
/*65*/	{"チョクラジ / 九州・沖縄",30085},
/*66*/	{"FMのべおか / 九州・沖縄",30088},
/*67*/	{"エフエムたつごう / 九州・沖縄",30072},
/*68*/	{"あまみエフエム / 九州・沖縄",30054},
/*69*/	{"FMやんばる / 九州・沖縄",30095},
/*70*/	{"オキラジ / 九州・沖縄",30066},
/*71*/	{"FMぎのわん / 九州・沖縄",30093},
/*72*/	{"ぎのわんシティFM / 九州・沖縄",30098},
/*73*/	{"fm那覇 / 九州・沖縄",30068},
/*74*/	{"FMよなばる / 九州・沖縄",30091},
/*75*/	{"FMとよみ / 九州・沖縄",30083},
/*76*/	{"FMいしがきサンサンラジオ / 九州・沖縄",30069},
/*77*/	{"Cwave / ネット放送局",30051},
/*78*/	{"試験放送 / ネット放送局",30099},
};
static byte initial_station = 9;   // 三角山放送局（札幌市西区）

static const char * headerKeys[] = {"Content-Encoding"};

AudioGenerator * ListenRadio :: station_t :: playlist_t :: getDecoder() {
  auto ListenRadio = getListenRadio();
  AudioGenerator * decoder;
  if(ListenRadio->decode_buffer != nullptr)
    decoder = new AudioGeneratorAAC(ListenRadio->decode_buffer, ListenRadio->decode_buffer_size, ListenRadio->enableSBR);
  else
    decoder = new AudioGeneratorAAC(ListenRadio->enableSBR);
  
  decoder->RegisterMetadataCB(ListenRadio->fnCbMetadata, ListenRadio->fnCbMetadata_data);
  decoder->RegisterStatusCB  (ListenRadio->fnCbStatus  , ListenRadio->fnCbStatus_data  );
  return decoder;
}


AudioFileSource * ListenRadio :: station_t :: playlist_t :: chunk_t :: getStream() {
  auto ListenRadio = playlist->getListenRadio();
  auto stream = new AudioFileSourceHTTPStream();
  stream->RegisterMetadataCB(ListenRadio->fnCbMetadata, ListenRadio->fnCbMetadata_data);
  stream->RegisterStatusCB  (ListenRadio->fnCbStatus  , ListenRadio->fnCbStatus_data  );
  if(!stream->open(url.c_str())) {
    delete stream;
    stream = nullptr;
  }
  return stream;
}


std::vector<ListenRadio :: station_t :: playlist_t :: chunk_t *> * ListenRadio :: station_t :: playlist_t :: getChunks() {
  WiFiClient client;
  HTTPClient http;
  bool success = false;
  
  clearChunks();
  
  if (http.begin(client, url)) {
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto payload = http.getString();
      
      if(http.header(headerKeys[0]).equalsIgnoreCase("gzip"))
        payload = uncompress((uint8_t *)(payload.c_str()), payload.length());
      getInner(payload, "media", "\n", [this](const String & value) {
        auto url = getStation()->getUrl() + value;
        chunks.push_back(new chunk_t(this, url));
      });
      success = true;
    } else {
      char bufs[url.length() + 10];
      sprintf(bufs, "%s %d", url.c_str(), httpCode);
      getListenRadio()->sendLog(bufs, true);
    }
    http.end();
  }
  if(success)
    return &chunks;
  else
    return nullptr;
}

void ListenRadio :: station_t :: playlist_t :: clearChunks() {
  for (auto itr : chunks)
    delete itr;
  chunks.clear();
}


std::vector<ListenRadio :: station_t :: playlist_t *> * ListenRadio :: station_t :: getPlaylists() {
  WiFiClient client;
  HTTPClient http;
  bool success = false;
  
  clearPlaylists();
  
  auto url = getUrl() + "playlist.m3u8";
  if (http.begin(client, url)) {
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
    auto httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      auto payload = http.getString();
      if(http.header(headerKeys[0]).equalsIgnoreCase("gzip"))
        payload = uncompress((uint8_t *)(payload.c_str()), payload.length());
      
      getInner(payload, "chunklist", "\n", [this](const String & value) {
        auto url = getUrl() + value;
        playlists.push_back(new playlist_t(this, url));
      });
      success = true;
    } else {
      char bufs[url.length() + 10];
      sprintf(bufs, "%s %d", url.c_str(), httpCode);
      getListenRadio()->sendLog(bufs, true);
    }
    http.end();
  }
  if(success)
    return &playlists;
  else
    return nullptr;
}

void ListenRadio :: station_t :: clearPlaylists() {
  for (auto itr : playlists)
    delete itr;
  playlists.clear();
}


void ListenRadio :: setEnableSBR(bool sbr) {
  if(!decode_buffer)
    enableSBR = sbr;
}

bool ListenRadio :: begin() {
  for(int i = 0; i < sizeof(station_list) / sizeof(station_list[0]); i++)
    stations.push_back(new station_t(this, station_list[i].id, station_list[i].name));
  defaultStationIdx = initial_station;

  if(!decode_buffer) {
    decode_buffer_size = enableSBR ? 89444: 26368;
    decode_buffer = heap_caps_malloc(decode_buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if(!decode_buffer)
      return false;
  }
  deInit();
  
  if(!bufferSize)
    bufferSize = std::max((uint32_t)(6 * 1024), std::min((uint32_t)(256 * 1024), (uint32_t)heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));
  
  buffer = new AudioFileSourceTS(bufferSize);
  buffer->identifyPid(0x101);
  
  startTask();
  
  return true;
}

void ListenRadio :: stop() {
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

bool ListenRadio :: RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) {
  return WebRadio::RegisterMetadataCB(fn, data) && buffer->RegisterMetadataCB(fn, data);
}
bool ListenRadio :: RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) {
  return WebRadio::RegisterStatusCB(fn, data) && buffer->RegisterStatusCB(fn, data);
}

String ListenRadio :: getInfoBuffer() {
  return buffer->getInfoBuffer();
}


void ListenRadio :: deInit() {
  if(buffer) {
    delete buffer;
    buffer = nullptr;
  }
}

void ListenRadio :: downloadTaskCore() {
  if(select_station || select_playlist) {
    stop();
    chunks = nullptr;
    saveSettings = millis() + 10000;
  }
  
  if(select_station) {
    auto playlists = ((station_t *)select_station)->getPlaylists();
    if(playlists && playlists->size() > 0) {
      select_playlist = (*playlists)[0];
    } else {
      current_playlist = nullptr;
      onSerious("select_station->getPlaylists(): false");
    }
    current_station = select_station;
    if(onPlay)
      onPlay(current_station->getName(), getIndex(current_station));
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
    }
//      else if(onProgram)
//        onProgram(((station_t *)current_station)->getProgram().c_str());
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

void ListenRadio :: decodeTaskCore() {
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
    if (current_playlist && now_millis - last_loop > 30000)
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

void ListenRadio :: saveStationCore(uint32_t nvs_handle, WebRadio::Station * station) {
  nvs_set_u8(nvs_handle, "ListenRadio", getIndex(station));
}

WebRadio::Station * ListenRadio :: restoreStationCore(uint32_t nvs_handle) {
  uint8_t idx_8;
  if(nvs_get_u8(nvs_handle, "ListenRadio", &idx_8))
    return nullptr;
  
  if(idx_8 > getNumOfStations() - 1)
    idx_8 = getNumOfStations() - 1;
  return getStation(idx_8);
}
