#pragma once
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "st7701s_defines.h"
#include "st7701s_init.h"

namespace esphome {
namespace st7701s {

static const char *const TAG = "st7701s";
const size_t ST7701S_TRANSFER_BUFFER_SIZE = 126;  // ensure this is divisible by 6

enum ST7701SColorMode {
  BITS_8 = 0x08,
  BITS_8_INDEXED = 0x09,
  BITS_16 = 0x10,
};

#ifndef ST7701SDisplay_DATA_RATE
#define ST7701SDisplay_DATA_RATE spi::DATA_RATE_40MHZ
#endif  // ST7701SDisplay_DATA_RATE

class ST7701SDisplay : public display::DisplayBuffer,
                       public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                             spi::CLOCK_PHASE_LEADING, ST7701SDisplay_DATA_RATE> {
 public:
  ST7701SDisplay() = default;
  ST7701SDisplay(uint8_t const *init_sequence, int16_t width, int16_t height, bool invert_colors)
      : init_sequence_{init_sequence}, width_{width}, height_{height}, pre_invertcolors_{invert_colors} {
    uint8_t cmd, num_args, bits;
    const uint8_t *addr = init_sequence;
    while ((cmd = *addr++) != 0) {
      num_args = *addr++ & 0x7F;
      bits = *addr;
      esph_log_d(TAG, "Command %02X, length %d, bits %02X", cmd, num_args, bits);
      switch (cmd) {
        case ST7701S_MADCTL: {
          this->swap_xy_ = (bits & MADCTL_MV) != 0;
          this->mirror_x_ = (bits & MADCTL_MX) != 0;
          this->mirror_y_ = (bits & MADCTL_MY) != 0;
          this->color_order_ = (bits & MADCTL_BGR) ? display::COLOR_ORDER_BGR : display::COLOR_ORDER_RGB;
          break;
        }

        case ST7701S_PIXFMT: {
          if ((bits & 0xF) == 6)
            this->is_18bitdisplay_ = true;
          break;
        }

        default:
          break;
      }
      addr += num_args;
    }
  }

  void set_dc_pin(GPIOPin *dc_pin) { dc_pin_ = dc_pin; }
  float get_setup_priority() const override;
  void set_reset_pin(GPIOPin *reset) { this->reset_pin_ = reset; }
  void set_palette(const uint8_t *palette) { this->palette_ = palette; }
  void set_buffer_color_mode(ST7701SColorMode color_mode) { this->buffer_color_mode_ = color_mode; }
  void set_dimensions(int16_t width, int16_t height) {
    this->height_ = height;
    this->width_ = width;
  }
  void set_offsets(int16_t offset_x, int16_t offset_y) {
    this->offset_x_ = offset_x;
    this->offset_y_ = offset_y;
  }
  void invert_colors(bool invert);
  virtual void command(uint8_t value);
  virtual void data(uint8_t value);
  void send_command(uint8_t command_byte, const uint8_t *data_bytes, uint8_t num_data_bytes);
  void set_color_order(display::ColorOrder color_order) { this->color_order_ = color_order; }
  void set_swap_xy(bool swap_xy) { this->swap_xy_ = swap_xy; }
  void set_mirror_x(bool mirror_x) { this->mirror_x_ = mirror_x; }
  void set_mirror_y(bool mirror_y) { this->mirror_y_ = mirror_y; }

  void update() override;

  void fill(Color color) override;

  void dump_config() override;
  void setup() override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  void draw_pixels_at(int x_start, int y_start, int w, int h, const uint8_t *ptr, display::ColorOrder order,
                      display::ColorBitness bitness, bool big_endian, int x_offset, int y_offset, int x_pad) override;

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void setup_pins_();

  virtual void set_madctl();
  void display_();
  void init_lcd_();
  void set_addr_window_(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2);
  void reset_();

  uint8_t const *init_sequence_{};
  int16_t width_{0};   ///< Display width as modified by current rotation
  int16_t height_{0};  ///< Display height as modified by current rotation
  int16_t offset_x_{0};
  int16_t offset_y_{0};
  uint16_t x_low_{0};
  uint16_t y_low_{0};
  uint16_t x_high_{0};
  uint16_t y_high_{0};
  const uint8_t *palette_;

  ST7701SColorMode buffer_color_mode_{BITS_16};

  uint32_t get_buffer_length_();
  int get_width_internal() override;
  int get_height_internal() override;

  void start_command_();
  void end_command_();
  void start_data_();
  void end_data_();

  GPIOPin *reset_pin_{nullptr};
  GPIOPin *dc_pin_{nullptr};
  GPIOPin *busy_pin_{nullptr};

  bool prossing_update_ = false;
  bool need_update_ = false;
  bool is_18bitdisplay_ = false;
  bool pre_invertcolors_ = false;
  display::ColorOrder color_order_{display::COLOR_ORDER_BGR};
  bool swap_xy_{};
  bool mirror_x_{};
  bool mirror_y_{};
};

//-----------   M5Stack display --------------
class ST7701SM5Stack : public ST7701SDisplay {
 public:
  ST7701SM5Stack() : ST7701SDisplay(INITCMD_M5STACK, 320, 240, true) {}
};

//-----------   M5Stack display --------------
class ST7701SM5CORE : public ST7701SDisplay {
 public:
  ST7701SM5CORE() : ST7701SDisplay(INITCMD_M5CORE, 320, 240, true) {}
};

//-----------   ST7789V display --------------
class ST7701SST7789V : public ST7701SDisplay {
 public:
  ST7701SST7789V() : ST7701SDisplay(INITCMD_ST7789V, 240, 320, false) {}
};

//-----------   ST7701S_24_TFT display --------------
class ST7701SILI9341 : public ST7701SDisplay {
 public:
  ST7701SILI9341() : ST7701SDisplay(INITCMD_ILI9341, 240, 320, false) {}
};

//-----------   ST7701S_24_TFT rotated display --------------
class ST7701SILI9342 : public ST7701SDisplay {
 public:
  ST7701SILI9342() : ST7701SDisplay(INITCMD_ILI9341, 320, 240, false) {}
};

//-----------   ST7701S_??_TFT rotated display --------------
class ST7701SILI9481 : public ST7701SDisplay {
 public:
  ST7701SILI9481() : ST7701SDisplay(INITCMD_ILI9481, 480, 320, false) {}
};

//-----------   ILI9481 in 18 bit mode --------------
class ST7701SILI948118 : public ST7701SDisplay {
 public:
  ST7701SILI948118() : ST7701SDisplay(INITCMD_ILI9481_18, 320, 480, true) {}
};

//-----------   ST7701S_35_TFT rotated display --------------
class ST7701SILI9486 : public ST7701SDisplay {
 public:
  ST7701SILI9486() : ST7701SDisplay(INITCMD_ILI9486, 480, 320, false) {}
};

class ST7701SILI9488 : public ST7701SDisplay {
 public:
  ST7701SILI9488(const uint8_t *seq = INITCMD_ILI9488) : ST7701SDisplay(seq, 480, 320, true) {}

 protected:
  void set_madctl() override {
    uint8_t mad = this->color_order_ == display::COLOR_ORDER_BGR ? MADCTL_BGR : MADCTL_RGB;
    uint8_t dfun = 0x22;
    this->width_ = 320;
    this->height_ = 480;
    if (!(this->swap_xy_ || this->mirror_x_ || this->mirror_y_)) {
      // no transforms
    } else if (this->mirror_y_ && this->mirror_x_) {
      // rotate 180
      dfun = 0x42;
    } else if (this->swap_xy_) {
      this->width_ = 480;
      this->height_ = 320;
      mad |= 0x20;
      if (this->mirror_x_) {
        dfun = 0x02;
      } else {
        dfun = 0x62;
      }
    }
    this->command(ST7701S_DFUNCTR);
    this->data(0);
    this->data(dfun);
    this->command(ST7701S_MADCTL);
    this->data(mad);
  }
};
//-----------   Waveshare 3.5 Res Touch - ILI9488 interfaced via 16 bit shift register to parallel */
class WAVESHARERES35 : public ST7701SILI9488 {
 public:
  WAVESHARERES35() : ST7701SILI9488(INITCMD_WAVESHARE_RES_3_5) {}
  void data(uint8_t value) override {
    this->start_data_();
    this->write_byte(0);
    this->write_byte(value);
    this->end_data_();
  }
};

//-----------   ST7701S_35_TFT origin colors rotated display --------------
class ST7701SILI9488A : public ST7701SDisplay {
 public:
  ST7701SILI9488A() : ST7701SDisplay(INITCMD_ILI9488_A, 480, 320, true) {}
};

//-----------   ST7701S_35_TFT rotated display --------------
class ST7701SST7796 : public ST7701SDisplay {
 public:
  ST7701SST7796() : ST7701SDisplay(INITCMD_ST7796, 320, 480, false) {}
};

class ST7701SS3Box : public ST7701SDisplay {
 public:
  ST7701SS3Box() : ST7701SDisplay(INITCMD_S3BOX, 320, 240, false) {}
};

class ST7701SS3BoxLite : public ST7701SDisplay {
 public:
  ST7701SS3BoxLite() : ST7701SDisplay(INITCMD_S3BOXLITE, 320, 240, true) {}
};

}  // namespace st7701s
}  // namespace esphome
