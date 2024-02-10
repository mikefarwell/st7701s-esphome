#pragma once

namespace esphome {
namespace st7701s {

// Color definitions
// clang-format off
static const uint8_t MADCTL_MY    = 0x80;   ///< Bit 7 Bottom to top
static const uint8_t MADCTL_MX    = 0x40;   ///< Bit 6 Right to left
static const uint8_t MADCTL_MV    = 0x20;   ///< Bit 5 Reverse Mode
static const uint8_t MADCTL_ML    = 0x10;   ///< Bit 4 LCD refresh Bottom to top
static const uint8_t MADCTL_RGB   = 0x00;  ///< Bit 3 Red-Green-Blue pixel order
static const uint8_t MADCTL_BGR   = 0x08;  ///< Bit 3 Blue-Green-Red pixel order
static const uint8_t MADCTL_MH    = 0x04;   ///< Bit 2 LCD refresh right to left
// clang-format on

// All ST7701S specific commands some are used by init()
static const uint8_t ST7701S_NOP = 0x00;
static const uint8_t ST7701S_SWRESET = 0x01;
static const uint8_t ST7701S_RDDID = 0x04;
static const uint8_t ST7701S_RDDST = 0x09;

static const uint8_t ST7701S_SLPIN = 0x10;
static const uint8_t ST7701S_SLPOUT = 0x11;
static const uint8_t ST7701S_PTLON = 0x12;
static const uint8_t ST7701S_NORON = 0x13;

static const uint8_t ST7701S_RDMODE = 0x0A;
static const uint8_t ST7701S_RDMADCTL = 0x0B;
static const uint8_t ST7701S_RDPIXFMT = 0x0C;
static const uint8_t ST7701S_RDIMGFMT = 0x0D;
static const uint8_t ST7701S_RDSELFDIAG = 0x0F;

static const uint8_t ST7701S_INVOFF = 0x20;
static const uint8_t ST7701S_INVON = 0x21;
static const uint8_t ST7701S_GAMMASET = 0x26;
static const uint8_t ST7701S_DISPOFF = 0x28;
static const uint8_t ST7701S_DISPON = 0x29;

static const uint8_t ST7701S_CASET = 0x2A;
static const uint8_t ST7701S_PASET = 0x2B;
static const uint8_t ST7701S_RAMWR = 0x2C;
static const uint8_t ST7701S_RAMRD = 0x2E;

static const uint8_t ST7701S_PTLAR = 0x30;
static const uint8_t ST7701S_VSCRDEF = 0x33;
static const uint8_t ST7701S_MADCTL = 0x36;
static const uint8_t ST7701S_VSCRSADD = 0x37;
static const uint8_t ST7701S_IDMOFF = 0x38;
static const uint8_t ST7701S_IDMON = 0x39;
static const uint8_t ST7701S_PIXFMT = 0x3A;
static const uint8_t ST7701S_COLMOD = 0x3A;

static const uint8_t ST7701S_GETSCANLINE = 0x45;

static const uint8_t ST7701S_WRDISBV = 0x51;
static const uint8_t ST7701S_RDDISBV = 0x52;
static const uint8_t ST7701S_WRCTRLD = 0x53;

static const uint8_t ST7701S_IFMODE = 0xB0;
static const uint8_t ST7701S_FRMCTR1 = 0xB1;
static const uint8_t ST7701S_FRMCTR2 = 0xB2;
static const uint8_t ST7701S_FRMCTR3 = 0xB3;
static const uint8_t ST7701S_INVCTR = 0xB4;
static const uint8_t ST7701S_DFUNCTR = 0xB6;
static const uint8_t ST7701S_ETMOD = 0xB7;

static const uint8_t ST7701S_PWCTR1 = 0xC0;
static const uint8_t ST7701S_PWCTR2 = 0xC1;
static const uint8_t ST7701S_PWCTR3 = 0xC2;
static const uint8_t ST7701S_PWCTR4 = 0xC3;
static const uint8_t ST7701S_PWCTR5 = 0xC4;
static const uint8_t ST7701S_VMCTR1 = 0xC5;
static const uint8_t ST7701S_IFCTR = 0xC6;
static const uint8_t ST7701S_VMCTR2 = 0xC7;
static const uint8_t ST7701S_GMCTR = 0xC8;
static const uint8_t ST7701S_SETEXTC = 0xC8;

static const uint8_t ST7701S_PWSET = 0xD0;
static const uint8_t ST7701S_VMCTR = 0xD1;
static const uint8_t ST7701S_PWSETN = 0xD2;
static const uint8_t ST7701S_RDID4 = 0xD3;
static const uint8_t ST7701S_RDINDEX = 0xD9;
static const uint8_t ST7701S_RDID1 = 0xDA;
static const uint8_t ST7701S_RDID2 = 0xDB;
static const uint8_t ST7701S_RDID3 = 0xDC;
static const uint8_t ST7701S_RDIDX = 0xDD;  // TBC

static const uint8_t ST7701S_GMCTRP1 = 0xE0;
static const uint8_t ST7701S_GMCTRN1 = 0xE1;

static const uint8_t ST7701S_CSCON = 0xF0;
static const uint8_t ST7701S_ADJCTL3 = 0xF7;

}  // namespace st7701s
}  // namespace esphome
