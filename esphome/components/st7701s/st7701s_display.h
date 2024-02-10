#pragma once
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

class ST7701SDisplay : public display::DisplayBuffer {
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

//-----------   Default display --------------
class ST7701SDefault : public ST7701SDisplay {
 public:
  ST7701SDefault() : ST7701SDisplay(INITCMD_M5STACK, 480, 480, true) {}
};

}  // namespace st7701s
}  // namespace esphome
