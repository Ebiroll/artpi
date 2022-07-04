#pragma once

#include "hw/sysbus.h"
#include "ui/console.h"
#include "qom/object.h"



// Flags for TFT_MADCTL
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_RGB 0x00
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH  0x04
#define TFT_MAD_SS  0x02
#define TFT_MAD_GS  0x01

#define ST7789_NOP			0x00
#define ST7789_SWRESET		0x01
#define ST7789_RDDID		0x04
#define ST7789_RDDST		0x09

#define ST7789_RDDPM		0x0A      // Read display power mode
#define ST7789_RDD_MADCTL	0x0B      // Read display MADCTL
#define ST7789_RDD_COLMOD	0x0C      // Read display pixel format
#define ST7789_RDDIM		0x0D      // Read display image mode
#define ST7789_RDDSM		0x0E      // Read display signal mode
#define ST7789_RDDSR		0x0F      // Read display self-diagnostic result (ST7789V)

#define ST7789_SLPIN		0x10
#define ST7789_SLPOUT		0x11
#define ST7789_PTLON		0x12
#define ST7789_NORON		0x13

#define ST7789_INVOFF		0x20
#define ST7789_INVON		0x21
#define ST7789_GAMSET		0x26      // Gamma set
#define ST7789_DISPOFF		0x28
#define ST7789_DISPON		0x29
#define ST7789_CASET		0x2A
#define ST7789_RASET		0x2B
#define ST7789_RAMWR		0x2C
#define ST7789_RGBSET		0x2D      // Color setting for 4096, 64K and 262K colors
#define ST7789_RAMRD		0x2E

#define ST7789_PTLAR		0x30
#define ST7789_VSCRDEF		0x33      // Vertical scrolling definition (ST7789V)
#define ST7789_TEOFF		0x34      // Tearing effect line off
#define ST7789_TEON			0x35      // Tearing effect line on
#define ST7789_MADCTL		0x36      // Memory data access control
#define ST7789_IDMOFF		0x38      // Idle mode off
#define ST7789_IDMON		0x39      // Idle mode on
#define ST7789_RAMWRC		0x3C      // Memory write continue (ST7789V)
#define ST7789_RAMRDC		0x3E      // Memory read continue (ST7789V)
#define ST7789_COLMOD		0x3A

#define ST7789_RAMCTRL		0xB0      // RAM control
#define ST7789_RGBCTRL		0xB1      // RGB control
#define ST7789_PORCTRL		0xB2      // Porch control
#define ST7789_FRCTRL1		0xB3      // Frame rate control
#define ST7789_PARCTRL		0xB5      // Partial mode control
#define ST7789_GCTRL		0xB7      // Gate control
#define ST7789_GTADJ		0xB8      // Gate on timing adjustment
#define ST7789_DGMEN		0xBA      // Digital gamma enable
#define ST7789_VCOMS		0xBB      // VCOMS setting
#define ST7789_LCMCTRL		0xC0      // LCM control
#define ST7789_IDSET		0xC1      // ID setting
#define ST7789_VDVVRHEN		0xC2      // VDV and VRH command enable
#define ST7789_VRHS			0xC3      // VRH set
#define ST7789_VDVSET		0xC4      // VDV setting
#define ST7789_VCMOFSET		0xC5      // VCOMS offset set
#define ST7789_FRCTR2		0xC6      // FR Control 2
#define ST7789_CABCCTRL		0xC7      // CABC control
#define ST7789_REGSEL1		0xC8      // Register value section 1
#define ST7789_REGSEL2		0xCA      // Register value section 2
#define ST7789_PWMFRSEL		0xCC      // PWM frequency selection
#define ST7789_PWCTRL1		0xD0      // Power control 1
#define ST7789_VAPVANEN		0xD2      // Enable VAP/VAN signal output
#define ST7789_CMD2EN		0xDF      // Command 2 enable
#define ST7789_PVGAMCTRL	0xE0      // Positive voltage gamma control
#define ST7789_NVGAMCTRL	0xE1      // Negative voltage gamma control
#define ST7789_DGMLUTR		0xE2      // Digital gamma look-up table for red
#define ST7789_DGMLUTB		0xE3      // Digital gamma look-up table for blue
#define ST7789_GATECTRL		0xE4      // Gate control
#define ST7789_SPI2EN		0xE7      // SPI2 enable
#define ST7789_PWCTRL2		0xE8      // Power control 2
#define ST7789_EQCTRL		0xE9      // Equalize time control
#define ST7789_PROMCTRL		0xEC      // Program control
#define ST7789_PROMEN		0xFA      // Program mode enable
#define ST7789_NVMSET		0xFC      // NVM setting
#define ST7789_PROMACT		0xFE      // Program action



#define SSCR	 0x08              
#define BPCR	 0x0c              
#define AWCR	 0x10 
#define TWCR     0x14
#define GCR      0x18
#define SRCR     0x24
#define BCCR     0x2C
#define IER      0x34
#define ISR      0x38

#define ICR      0x3C
#define LIPCR    0x40
#define CPSR     0x44
#define CDSR     0x48


#define CR	     0x84 
#define WHPCR	 0x88              
#define WVPCR	 0x8C 
#define CKCR     0x90
#define PFCR     0x94
#define CACR     0x98
#define DCCR     0x9C
#define BFCR     0xA0
#define CFBAR    0xAC
#define CFBLR    0xB0

#define CFBLNR   0xB4
#define CLUTWR   0x144




#if 0
typedef struct
{
  uint32_t      RESERVED0[2];  /*!< Reserved, 0x00-0x04                                                       */
  __IO uint32_t SSCR;          /*!< LTDC Synchronization Size Configuration Register,    Address offset: 0x08 */
  __IO uint32_t BPCR;          /*!< LTDC Back Porch Configuration Register,              Address offset: 0x0C */
  __IO uint32_t AWCR;          /*!< LTDC Active Width Configuration Register,            Address offset: 0x10 */
  __IO uint32_t TWCR;          /*!< LTDC Total Width Configuration Register,             Address offset: 0x14 */
  __IO uint32_t GCR;           /*!< LTDC Global Control Register,                        Address offset: 0x18 */
  uint32_t      RESERVED1[2];  /*!< Reserved, 0x1C-0x20                                                       */
  __IO uint32_t SRCR;          /*!< LTDC Shadow Reload Configuration Register,           Address offset: 0x24 */
  uint32_t      RESERVED2[1];  /*!< Reserved, 0x28                                                            */
  __IO uint32_t BCCR;          /*!< LTDC Background Color Configuration Register,        Address offset: 0x2C */
  uint32_t      RESERVED3[1];  /*!< Reserved, 0x30                                                            */
  __IO uint32_t IER;           /*!< LTDC Interrupt Enable Register,                      Address offset: 0x34 */
  __IO uint32_t ISR;           /*!< LTDC Interrupt Status Register,                      Address offset: 0x38 */
  __IO uint32_t ICR;           /*!< LTDC Interrupt Clear Register,                       Address offset: 0x3C */
  __IO uint32_t LIPCR;         /*!< LTDC Line Interrupt Position Configuration Register, Address offset: 0x40 */
  __IO uint32_t CPSR;          /*!< LTDC Current Position Status Register,               Address offset: 0x44 */
  __IO uint32_t CDSR;         /*!< LTDC Current Display Status Register,                 Address offset: 0x48 */
} LTDC_TypeDef;


typedef struct
{
  __IO uint32_t CR;            /*!< LTDC Layerx Control Register                                  Address offset: 0x84 */
  __IO uint32_t WHPCR;         /*!< LTDC Layerx Window Horizontal Position Configuration Register Address offset: 0x88 */
  __IO uint32_t WVPCR;         /*!< LTDC Layerx Window Vertical Position Configuration Register   Address offset: 0x8C */
  __IO uint32_t CKCR;          /*!< LTDC Layerx Color Keying Configuration Register               Address offset: 0x90 */
  __IO uint32_t PFCR;          /*!< LTDC Layerx Pixel Format Configuration Register               Address offset: 0x94 */
  __IO uint32_t CACR;          /*!< LTDC Layerx Constant Alpha Configuration Register             Address offset: 0x98 */
  __IO uint32_t DCCR;          /*!< LTDC Layerx Default Color Configuration Register              Address offset: 0x9C */
  __IO uint32_t BFCR;          /*!< LTDC Layerx Blending Factors Configuration Register           Address offset: 0xA0 */
  uint32_t      RESERVED0[2];  /*!< Reserved */
  __IO uint32_t CFBAR;         /*!< LTDC Layerx Color Frame Buffer Address Register               Address offset: 0xAC */
  __IO uint32_t CFBLR;         /*!< LTDC Layerx Color Frame Buffer Length Register                Address offset: 0xB0 */
  __IO uint32_t CFBLNR;        /*!< LTDC Layerx ColorFrame Buffer Line Number Register            Address offset: 0xB4 */
  uint32_t      RESERVED1[3];  /*!< Reserved */
  __IO uint32_t CLUTWR;         /*!< LTDC Layerx CLUT Write Register                               Address offset: 0x144 */

} LTDC_Layer_TypeDef;


#endif


/*

//AWCR	Active Width Configuration Register
//BCCR	Background Color Configuration Register
//BPCR	Back Porch Configuration Register
CDSR	Current Display Status Register
CPSR	Current Position Status Register
GCR	Global Control Register
ICR	Interrupt Clear Register
IER	Interrupt Enable Register
ISR	Interrupt Status Register
L1CR	Layerx Control Register
L1WHPCR	Layerx Window Horizontal Position Configuration Register
L1WVPCR	Layerx Window Vertical Position Configuration Register
L1CKCR	Layerx Color Keying Configuration Register
L1PFCR	Layerx Pixel Format Configuration Register
L1CACR	Layerx Constant Alpha Configuration Register
L1DCCR	Layerx Default Color Configuration Register
L1BFCR	Layerx Blending Factors Configuration Register
L1CFBAR	Layerx Color Frame Buffer Address Register
L1CFBLR	Layerx Color Frame Buffer Length Register
L1CFBLNR	Layerx ColorFrame Buffer Line Number Register
L1CLUTWR	Layerx CLUT Write Register
L2CR	Layerx Control Register
L2WHPCR	Layerx Window Horizontal Position Configuration Register
L2WVPCR	Layerx Window Vertical Position Configuration Register
L2CKCR	Layerx Color Keying Configuration Register
L2PFCR	Layerx Pixel Format Configuration Register
L2CACR	Layerx Constant Alpha Configuration Register
L2DCCR	Layerx Default Color Configuration Register
L2BFCR	Layerx Blending Factors Configuration Register
L2CFBAR	Layerx Color Frame Buffer Address Register
L2CFBLR	Layerx Color Frame Buffer Length Register
L2CFBLNR	Layerx ColorFrame Buffer Line Number Register
L2CLUTWR	Layerx CLUT Write Register
LIPCR	Line Interrupt Position Configuration Register
RegisterBlock	Register block
SRCR	Shadow Reload Configuration Register
//Synchronization Size Configuration Register
TWCR	Total Width Configuration Register

*/

#define TYPE_LTDC "ltdc"
OBJECT_DECLARE_SIMPLE_TYPE(LtdcState, LTDC)


typedef struct ConsoleState {
    QemuConsole *con;
    uint32_t redraw;
    int width,height; // lcd size in panel memory
    int x_offset,y_offset; // offset in the panel memory
    int skin_x_offset, skin_y_offset; // offset on the skin
    int skin_width;
    int32_t x_start; // area to draw into
    int32_t x_end;
    int32_t y_start;
    int32_t y_end;
    int32_t x; // current draw position
    int32_t y;
    int little_endian;
    int backlight;
    uint32_t current_command;
    int cmd_mode;
    uint32_t *data; // surface data
} ConsoleState;


struct  LtdcState {
    SysBusDevice busdev;

    SSIPeripheral ssidev;
    ConsoleState *con;
    qemu_irq button[2];

    AddressSpace dma_as;
    MemoryRegion iomem;
    MemoryRegionSection fbsection;
    qemu_irq ltcd_irq;


    MemoryRegionSection layerA;
    MemoryRegionSection layerB;
};


