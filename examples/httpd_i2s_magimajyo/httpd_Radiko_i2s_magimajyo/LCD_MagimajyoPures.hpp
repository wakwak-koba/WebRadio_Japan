#ifndef _LCD_MAGIMAJYO_PURES_HPP_
#define _LCD_MAGIMAJYO_PURES_HPP_

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LCD_MagimajoPures : public lgfx::LGFX_Device
{
  lgfx::v1::Panel_ILI9342 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;

public:
  LCD_MagimajoPures(void)
  {
    {                                     // バス制御の設定を行います。
      auto cfg = _bus_instance.config(); // バス設定用の構造体を取得します。

      // 8ビットパラレルバスの設定
      cfg.i2s_port = I2S_NUM_1;  // 使用するI2Sポートを選択 (0 or 1) (ESP32のI2S LCDモードを使用します)
      cfg.freq_write = 16000000; // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      cfg.pin_wr = 13;           // WR を接続しているピン番号     to LCD WR(14)
      cfg.pin_rd = 12;           // RD を接続しているピン番号     to LCD RD(13)
      cfg.pin_rs = 27;           // RS(D/C)を接続しているピン番号 to LCD DC(16)
/*
      cfg.pin_d0 = 21;           // D0を接続しているピン番号      to LCD D0(4)
      cfg.pin_d1 = 19;           // D1を接続しているピン番号      to LCD D1(5)
      cfg.pin_d2 = 18;           // D2を接続しているピン番号      to LCD D2(6)
      cfg.pin_d3 =  5;           // D3を接続しているピン番号      to LCD D3(7)
      cfg.pin_d4 = 17;           // D4を接続しているピン番号      to LCD D4(8)
      cfg.pin_d5 = 16;           // D5を接続しているピン番号      to LCD D5(9)
      cfg.pin_d6 =  4;           // D6を接続しているピン番号      to LCD D6(10)
      cfg.pin_d7 = 15;           // D7を接続しているピン番号      to LCD D7(11)
*/
      cfg.pin_d0 = 23;           // D0を接続しているピン番号      to LCD D0(4)
      cfg.pin_d1 = 21;           // D1を接続しているピン番号      to LCD D1(5)
      cfg.pin_d2 = 19;           // D2を接続しているピン番号      to LCD D2(6)
      cfg.pin_d3 = 18;           // D3を接続しているピン番号      to LCD D3(7)
      cfg.pin_d4 =  5;           // D4を接続しているピン番号      to LCD D4(8)
      cfg.pin_d5 =  4;           // D5を接続しているピン番号      to LCD D5(9)
      cfg.pin_d6 =  2;           // D6を接続しているピン番号      to LCD D6(10)
      cfg.pin_d7 = 15;           // D7を接続しているピン番号      to LCD D7(11)

      _bus_instance.config(cfg);              // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {                                       // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      
      cfg.pin_cs = 14;   // CSが接続されているピン番号   (-1 = disable)  to LCD CS(12/15)
      cfg.pin_rst = 33;  // RSTが接続されているピン番号  (-1 = disable)  to LCD RST(2)
      cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)
      
      // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。
      
      cfg.memory_width = 320;   // ドライバICがサポートしている最大の幅
      cfg.memory_height = 240;  // ドライバICがサポートしている最大の高さ
      cfg.panel_width = 320;    // 実際に表示可能な幅
      cfg.panel_height = 96;    // 実際に表示可能な高さ
      cfg.offset_x = 0;         // パネルのX方向オフセット量
      cfg.offset_y = 144;       // パネルのY方向オフセット量
      cfg.offset_rotation = 0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true;      // データ読出しが可能な場合 trueに設定
      cfg.invert = false;       // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order = false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit = false;   // データ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared = true;    // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)
      
      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }

  void writeBuffer(lgfx::LGFX_Sprite & sprite) {
    setAddrWindow(0, 0, 320, 96);
    pushPixels((lgfx::swap565_t*)sprite.getBuffer(), 640 * 48);
  }
};

#endif
