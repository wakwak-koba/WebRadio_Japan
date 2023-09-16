//#define WIFI_SSID "SET YOUR WIFI SSID"
//#define WIFI_PASS "SET YOUR WIFI PASS"
//#define SYSLOG_TO "SET YOUR SYSLOG SERVER"

#include <math.h>
#include <WiFi.h>
#include <SD.h>
#include <M5UnitLCD.h>
#include <M5UnitOLED.h>
#include <M5Unified.h>
#include <nvs.h>

//#define SEPARATE_DOWNLOAD_TASK
#include <WebRadio_ListenRadio.h>
#include <AudioOutputM5Speaker.h>

/// set M5Speaker virtual channel (0-7)
static constexpr uint8_t m5spk_virtual_channel = 0;
static constexpr uint8_t m5spk_task_pinned_core = APP_CPU_NUM;

#define FFT_SIZE 256
class fft_t
{
  float _wr[FFT_SIZE + 1];
  float _wi[FFT_SIZE + 1];
  float _fr[FFT_SIZE + 1];
  float _fi[FFT_SIZE + 1];
  uint16_t _br[FFT_SIZE + 1];
  size_t _ie;

public:
  fft_t(void)
  {
#ifndef M_PI
#define M_PI 3.141592653
#endif
    _ie = logf( (float)FFT_SIZE ) / log(2.0) + 0.5;
    static constexpr float omega = 2.0f * M_PI / FFT_SIZE;
    static constexpr int s4 = FFT_SIZE / 4;
    static constexpr int s2 = FFT_SIZE / 2;
    for ( int i = 1 ; i < s4 ; ++i)
    {
    float f = cosf(omega * i);
      _wi[s4 + i] = f;
      _wi[s4 - i] = f;
      _wr[     i] = f;
      _wr[s2 - i] = -f;
    }
    _wi[s4] = _wr[0] = 1;

    size_t je = 1;
    _br[0] = 0;
    _br[1] = FFT_SIZE / 2;
    for ( size_t i = 0 ; i < _ie - 1 ; ++i )
    {
      _br[ je << 1 ] = _br[ je ] >> 1;
      je = je << 1;
      for ( size_t j = 1 ; j < je ; ++j )
      {
        _br[je + j] = _br[je] + _br[j];
      }
    }
  }

  void exec(const int16_t* in)
  {
    memset(_fi, 0, sizeof(_fi));
    for ( size_t j = 0 ; j < FFT_SIZE / 2 ; ++j )
    {
      float basej = 0.25 * (1.0-_wr[j]);
      size_t r = FFT_SIZE - j - 1;

      /// perform han window and stereo to mono convert.
      _fr[_br[j]] = basej * (in[j * 2] + in[j * 2 + 1]);
      _fr[_br[r]] = basej * (in[r * 2] + in[r * 2 + 1]);
    }

    size_t s = 1;
    size_t i = 0;
    do
    {
      size_t ke = s;
      s <<= 1;
      size_t je = FFT_SIZE / s;
      size_t j = 0;
      do
      {
        size_t k = 0;
        do
        {
          size_t l = s * j + k;
          size_t m = ke * (2 * j + 1) + k;
          size_t p = je * k;
          float Wxmr = _fr[m] * _wr[p] + _fi[m] * _wi[p];
          float Wxmi = _fi[m] * _wr[p] - _fr[m] * _wi[p];
          _fr[m] = _fr[l] - Wxmr;
          _fi[m] = _fi[l] - Wxmi;
          _fr[l] += Wxmr;
          _fi[l] += Wxmi;
        } while ( ++k < ke) ;
      } while ( ++j < je );
    } while ( ++i < _ie );
  }

  uint32_t get(size_t index)
  {
    return (index < FFT_SIZE / 2) ? (uint32_t)sqrtf(_fr[ index ] * _fr[ index ] + _fi[ index ] * _fi[ index ]) : 0u;
  }
};

static constexpr size_t WAVE_SIZE = 320;
static AudioOutputM5Speaker out(&M5.Speaker, m5spk_virtual_channel);
static ListenRadio radio(&out, 1-m5spk_task_pinned_core);

static fft_t fft;
static bool fft_enabled = false;
static bool wave_enabled = false;
static uint16_t prev_y[(FFT_SIZE / 2)+1];
static uint16_t peak_y[(FFT_SIZE / 2)+1];
static int16_t wave_y[WAVE_SIZE];
static int16_t wave_h[WAVE_SIZE];
static int16_t raw_data[WAVE_SIZE * 2];
static int header_height = 0;
static char stream_title[128] = { 0 };
static const char* meta_text[2] = { nullptr, stream_title };
static const size_t meta_text_num = sizeof(meta_text) / sizeof(meta_text[0]);
static uint8_t meta_mod_bits = 0;

static uint32_t bgcolor(LGFX_Device* gfx, int y)
{
  auto h = gfx->height();
  auto dh = h - header_height;
  int v = ((h - y)<<5) / dh;
  if (dh > 44)
  {
    int v2 = ((h - y - 1)<<5) / dh;
    if ((v >> 2) != (v2 >> 2))
    {
      return 0x666666u;
    }
  }
  return gfx->color888(v + 2, v, v + 6);
}

static void gfxSetup(LGFX_Device* gfx)
{
  if (gfx == nullptr) { return; }
  if (gfx->width() < gfx->height())
  {
    gfx->setRotation(gfx->getRotation()^1);
  }
  gfx->setFont(&fonts::lgfxJapanGothic_12);
  gfx->setEpdMode(epd_mode_t::epd_fastest);
  gfx->setTextWrap(false);
  gfx->setCursor(0, 8);
  gfx->println("WebRadio player");
  gfx->fillRect(0, 6, gfx->width(), 2, TFT_BLACK);

  header_height = (gfx->height() > 80) ? 33 : 21;
  fft_enabled = !gfx->isEPD();
  if (fft_enabled)
  {
    wave_enabled = (gfx->getBoard() != m5gfx::board_M5UnitLCD);

    for (int y = header_height; y < gfx->height(); ++y)
    {
      gfx->drawFastHLine(0, y, gfx->width(), bgcolor(gfx, y));
    }
  }

  for (int x = 0; x < (FFT_SIZE/2)+1; ++x)
  {
    prev_y[x] = INT16_MAX;
    peak_y[x] = INT16_MAX;
  }
  for (int x = 0; x < WAVE_SIZE; ++x)
  {
    wave_y[x] = gfx->height();
    wave_h[x] = 0;
  }
}

void gfxLoop(LGFX_Device* gfx)
{
  if (gfx == nullptr) { return; }
  if (header_height > 32)
  {
    if (meta_mod_bits)
    {
      gfx->startWrite();
      for (int id = 0; id < meta_text_num; ++id)
      {
        if (0 == (meta_mod_bits & (1<<id))) { continue; }
        meta_mod_bits &= ~(1<<id);
        size_t y = id * 12;
        if (y+12 >= header_height) { continue; }
        gfx->setCursor(4, 8 + y);
        gfx->fillRect(0, 8 + y, gfx->width(), 12, gfx->getBaseColor());
        gfx->print(meta_text[id]);
        gfx->print(" "); // Garbage data removal when UTF8 characters are broken in the middle.
      }
      gfx->display();
      gfx->endWrite();
    }
  }
  else
  {
    static int title_x;
    static int title_id;
    static int wait = INT16_MAX;

    if (meta_mod_bits)
    {
      if (meta_mod_bits & 1)
      {
        title_x = 4;
        title_id = 0;
        gfx->fillRect(0, 8, gfx->width(), 12, gfx->getBaseColor());
      }
      meta_mod_bits = 0;
      wait = 0;
    }

    if (--wait < 0)
    {
      int tx = title_x;
      int tid = title_id;
      wait = 3;
      gfx->startWrite();
      uint_fast8_t no_data_bits = 0;
      do
      {
        if (tx == 4) { wait = 255; }
        gfx->setCursor(tx, 8);
        const char* meta = meta_text[tid];
        if (meta[0] != 0)
        {
          gfx->print(meta);
          gfx->print("  /  ");
          tx = gfx->getCursorX();
          if (++tid == meta_text_num) { tid = 0; }
          if (tx <= 4)
          {
            title_x = tx;
            title_id = tid;
          }
        }
        else
        {
          if ((no_data_bits |= 1 << tid) == ((1 << meta_text_num) - 1))
          {
            break;
          }
          if (++tid == meta_text_num) { tid = 0; }
        }
      } while (tx < gfx->width());
      --title_x;
      gfx->display();
      gfx->endWrite();
    }
  }

  if (fft_enabled)
  {
    static int prev_x[2];
    static int peak_x[2];

    auto buf = out.getBuffer();
    if (buf)
    {
      memcpy(raw_data, buf, WAVE_SIZE * 2 * sizeof(int16_t)); // stereo data copy
      gfx->startWrite();

      // draw stereo level meter
      for (size_t i = 0; i < 2; ++i)
      {
        int32_t level = 0;
        for (size_t j = i; j < 640; j += 32)
        {
          uint32_t lv = abs(raw_data[j]);
          if (level < lv) { level = lv; }
        }

        int32_t x = (level * gfx->width()) / INT16_MAX;
        int32_t px = prev_x[i];
        if (px != x)
        {
          gfx->fillRect(x, i * 3, px - x, 2, px < x ? 0xFF9900u : 0x330000u);
          prev_x[i] = x;
        }
        px = peak_x[i];
        if (px > x)
        {
          gfx->writeFastVLine(px, i * 3, 2, TFT_BLACK);
          px--;
        }
        else
        {
          px = x;
        }
        if (peak_x[i] != px)
        {
          peak_x[i] = px;
          gfx->writeFastVLine(px, i * 3, 2, TFT_WHITE);
        }
      }
      gfx->display();

      // draw FFT level meter
      fft.exec(raw_data);
      size_t bw = gfx->width() / 60;
      if (bw < 3) { bw = 3; }
      int32_t dsp_height = gfx->height();
      int32_t fft_height = dsp_height - header_height - 1;
      size_t xe = gfx->width() / bw;
      if (xe > (FFT_SIZE/2)) { xe = (FFT_SIZE/2); }
      int32_t wave_next = ((header_height + dsp_height) >> 1) + (((256 - (raw_data[0] + raw_data[1])) * fft_height) >> 17);

      uint32_t bar_color[2] = { 0x000033u, 0x99AAFFu };

      for (size_t bx = 0; bx <= xe; ++bx)
      {
        size_t x = bx * bw;
        if ((x & 7) == 0) { gfx->display(); taskYIELD(); }
        int32_t f = fft.get(bx);
        int32_t y = (f * fft_height) >> 18;
        if (y > fft_height) { y = fft_height; }
        y = dsp_height - y;
        int32_t py = prev_y[bx];
        if (y != py)
        {
          gfx->fillRect(x, y, bw - 1, py - y, bar_color[(y < py)]);
          prev_y[bx] = y;
        }
        py = peak_y[bx] + 1;
        if (py < y)
        {
          gfx->writeFastHLine(x, py - 1, bw - 1, bgcolor(gfx, py - 1));
        }
        else
        {
          py = y - 1;
        }
        if (peak_y[bx] != py)
        {
          peak_y[bx] = py;
          gfx->writeFastHLine(x, py, bw - 1, TFT_WHITE);
        }


        if (wave_enabled)
        {
          for (size_t bi = 0; bi < bw; ++bi)
          {
            size_t i = x + bi;
            if (i >= gfx->width() || i >= WAVE_SIZE) { break; }
            y = wave_y[i];
            int32_t h = wave_h[i];
            bool use_bg = (bi+1 == bw);
            if (h>0)
            { /// erase previous wave.
              gfx->setAddrWindow(i, y, 1, h);
              h += y;
              do
              {
                uint32_t bg = (use_bg || y < peak_y[bx]) ? bgcolor(gfx, y)
                            : (y == peak_y[bx]) ? 0xFFFFFFu
                            : bar_color[(y >= prev_y[bx])];
                gfx->writeColor(bg, 1);
              } while (++y < h);
            }
            size_t i2 = i << 1;
            int32_t y1 = wave_next;
            wave_next = ((header_height + dsp_height) >> 1) + (((256 - (raw_data[i2] + raw_data[i2 + 1])) * fft_height) >> 17);
            int32_t y2 = wave_next;
            if (y1 > y2)
            {
              int32_t tmp = y1;
              y1 = y2;
              y2 = tmp;
            }
            y = y1;
            h = y2 + 1 - y;
            wave_y[i] = y;
            wave_h[i] = h;
            if (h>0)
            { /// draw new wave.
              gfx->setAddrWindow(i, y, 1, h);
              h += y;
              do
              {
                uint32_t bg = (y < prev_y[bx]) ? 0xFFCC33u : 0xFFFFFFu;
                gfx->writeColor(bg, 1);
              } while (++y < h);
            }
          }
        }
      }
      gfx->display();
      gfx->endWrite();
    }
  }

  if (!gfx->displayBusy())
  { // draw volume bar
    static int px;
    uint8_t v = M5.Speaker.getVolume();
    int x = v * (gfx->width()) >> 8;
    if (px != x)
    {
      gfx->fillRect(x, 6, px - x, 2, px < x ? 0xAAFFAAu : 0u);
      gfx->display();
      px = x;
    }
  }
}

void setup(void)
{
  audioLogger = &Serial;
  
  auto cfg = M5.config();

  cfg.external_spk = true;    /// use external speaker (SPK HAT / ATOMIC SPK)
//cfg.external_spk_detail.omit_atomic_spk = true; // exclude ATOMIC SPK
//cfg.external_spk_detail.omit_spk_hat    = true; // exclude SPK HAT
//cfg.external_speaker.module_rca = true; // RCA Module 13.2

  M5.begin(cfg);
  M5.update();
  if(M5.BtnA.isPressed() && M5.BtnB.isPressed() && M5.BtnC.isPressed()) {
    uint32_t nvs_handle;
    if(ESP_OK == nvs_open("WebRadio", NVS_READWRITE, &nvs_handle)) {
      M5.Display.println("nvs_flash_erase");
      nvs_erase_all(nvs_handle);
      delay(3000);
    }
  }

  if(!cfg.external_speaker.module_rca) { /// custom setting
    auto spk_cfg = M5.Speaker.config();
    /// Increasing the sample_rate will improve the sound quality instead of increasing the CPU load.
    spk_cfg.sample_rate = 144000; // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 , 96000 , 100000 , 128000 , 144000 , 192000 , 200000
    spk_cfg.task_pinned_core = m5spk_task_pinned_core;
    M5.Speaker.config(spk_cfg);
  }


  M5.Speaker.begin();

  WiFi.disconnect();

  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

#if defined ( WIFI_SSID ) && defined ( WIFI_PASS )
  WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif

  WiFi.begin();

  /// settings
  if (SD.begin(GPIO_NUM_4, SPI, 25000000)) {
    /// wifi
    auto fs = SD.open("/wifi.txt", FILE_READ);
    if(fs) {
      size_t sz = fs.size();
      char buf[sz + 1];
      fs.read((uint8_t*)buf, sz);
      buf[sz] = 0;
      fs.close();

      int y = 0;
      for(int x = 0; x < sz; x++) {
        if(buf[x] == 0x0a || buf[x] == 0x0d)
          buf[x] = 0;
        else if (!y && x > 0 && !buf[x - 1] && buf[x])
          y = x;
      }
      WiFi.begin(buf, &buf[y]);
    }
    SD.end();
  }

  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("WebRadio", NVS_READONLY, &nvs_handle)) {
      size_t volume;
      nvs_get_u32(nvs_handle, "volume", &volume);
      M5.Speaker.setVolume(volume);
      nvs_close(nvs_handle);
    }
  }

  if(M5.BtnA.isPressed()) {
    // smart config
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.beginSmartConfig();
    M5.Display.println();
    M5.Display.print("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      M5.Display.print(".");
    }
    M5.Display.println();
    M5.Display.print("SmartConfig received.");
    while (WiFi.status() != WL_CONNECTED)
      delay(100);
    WiFi.stopSmartConfig();
    WiFi.mode(WIFI_MODE_STA);
  } else {
    M5.Display.println("Connecting to WiFi");
    for(int i = 0; i < 100 && WiFi.status() != WL_CONNECTED; i++) {
      M5.Display.print(".");
      delay(100);
    }
    if(WiFi.status() != WL_CONNECTED)
      ESP.restart();
  }

  Serial.print("IP address:");
  Serial.println(WiFi.localIP());  
  M5.Display.clear();

  gfxSetup(&M5.Display);

  radio.onPlay = [](const char * station_name, const size_t station_idx) {
    Serial.printf("onPlay:%d %s", station_idx, station_name);
    Serial.println();
    meta_text[0] = station_name;
    stream_title[0] = 0;
    meta_mod_bits = 3;
  };
  radio.onInfo = [](const char *text) {
    Serial.println(text);
  };
  radio.onError = [](const char * text) {
    Serial.println(text);
  };
  radio.onProgram = [](const char * program_title) {
    strcpy(stream_title, program_title);
    meta_mod_bits |= 2;
  };
#ifdef SYSLOG_TO
  radio.setSyslog(SYSLOG_TO);
#endif
  if(!radio.begin()) {
    Serial.println("failed: radio.begin()");
    for(;;);
  } 
  if(!radio.play()) {
    Serial.println("failed: radio.play()");
    for(;;);    
  }
}

void loop(void)
{
  static unsigned long long saveSettings = 0;
  radio.handle();
  gfxLoop(&M5.Display);

  {
    static int prev_frame;
    int frame;
    do
    {
      delay(1);
    } while (prev_frame == (frame = millis() >> 3)); /// 8 msec cycle wait
    prev_frame = frame;
  }

  M5.update();
  int8_t adjStation = 0;
  int8_t adjVolume = 0;

  if (M5.BtnA.wasPressed())
  {
    M5.Speaker.tone(440, 50);
  }
  if (M5.BtnA.wasDecideClickCount())
  {
    switch (M5.BtnA.getClickCount())
    {
    case 1:
      adjStation = +1;
      break;

    case 2:
      adjStation = -1;
      break;
    }
  }
  if (M5.BtnA.isHolding() || M5.BtnB.isPressed() || M5.BtnC.isPressed())
  {
    adjVolume = (M5.BtnB.isPressed()) ? -1 : +1;
    if (M5.BtnA.isHolding())
      adjVolume = M5.BtnA.getClickCount() ? -1 : +1;
  }

  if(M5.Touch.getCount() > 0)
  {
    auto t = M5.Touch.getDetail();
    static auto prev_state = t.state;
    if (prev_state != t.state && t.state == m5::touch_state_t::flick_end) {
      if (t.distanceY() + (M5.Display.height() >> 2) < 0)
        adjStation = -1;
      else if (t.distanceY() - (M5.Display.height() >> 2) > 0)
        adjStation = +1;
    } else if (t.y < M5.Display.height() && t.state & m5::touch_state_t::flick) {
      adjVolume = t.deltaX();
    }
    prev_state = t.state;
  }

  if(adjStation) {
    M5.Speaker.tone(adjStation > 0 ? 1000 : 800, 100);
    radio.play(adjStation > 0);
  }

  if(adjVolume) {
    int v = M5.Speaker.getVolume() + adjVolume;
    if (v < 0)
      v = 0;
    else if (v > 255)
      v = 255;

    M5.Speaker.setVolume(v);
    saveSettings = millis() + 5000;
  }

  if (saveSettings > 0 && millis() > saveSettings)
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("WebRadio", NVS_READWRITE, &nvs_handle)) {
      size_t volume = M5.Speaker.getVolume();
      nvs_set_u32(nvs_handle, "volume", volume);
      nvs_close(nvs_handle);
    }
    saveSettings = 0;
  }
}
