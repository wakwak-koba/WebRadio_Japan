#include <M5Unified.h>
#include <WebRadio_Radiko.h>
#include <AudioOutputM5Speaker.h>

static constexpr uint8_t m5spk_task_pinned_core = APP_CPU_NUM;

static AudioOutputM5Speaker out(&M5.Speaker, 0);
static Radiko radio(&out, 1-m5spk_task_pinned_core);

void setup() {
  {    
    auto cfg = M5.config();
    cfg.external_spk = true;    /// use external speaker (SPK HAT / ATOMIC SPK)
//  cfg.external_spk_detail.omit_atomic_spk = true; // exclude ATOMIC SPK
//  cfg.external_spk_detail.omit_spk_hat    = true; // exclude SPK HAT
//  cfg.external_speaker.module_rca = true; // RCA Module 13.2
    M5.begin(cfg);
  }
  
  if(!cfg.external_speaker.module_rca) { /// custom setting
    auto cfg = M5.Speaker.config();
    cfg.sample_rate = 144000; // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 , 96000 , 100000 , 128000 , 144000 , 192000 , 200000
    cfg.task_pinned_core = m5spk_task_pinned_core;
    M5.Speaker.config(cfg);
  }

  WiFi.begin();
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  if(!radio.begin()) {
    Serial.println("failed: radio.begin()");
    for(;;);
  }

  radio.play();
}

void loop() {
  radio.handle();
  delay(1);
}
