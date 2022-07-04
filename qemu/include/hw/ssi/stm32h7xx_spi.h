/*
 * STM32H7XX SPI
 *
 * Copyright (c) 2014 Alistair Francis <alistair@alistair23.me>
 * Copyright (c) 2022 Olof Astrand <olof.astrand@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef HW_STM32H7XX_SPI_H
#define HW_STM32H7XX_SPI_H

#include "hw/sysbus.h"
#include "hw/ssi/ssi.h"
#include "qom/object.h"

#define STM_SPI_CR1     0x00
#define STM_SPI_CR2     0x04
#define STM_SPI_CFG1    0x08
#define STM_SPI_CFG2    0x0c


#define STM_SPI_IER     0x10


#define STM_SPI_SR      0x14

// TXDR
#define STM_SPI_DR      0x20
#define STM_SPI_RXDR    0x30


#define STM_SPI_CRCPR   0x40
#define STM_SPI_RXCRCR  0x48
#define STM_SPI_TXCRCR  0x44
#define STM_SPI_I2SCFGR 0x50
#define STM_SPI_I2SPR   0x20


#if 0
CR1	control register 1
CR2	control register 2
CRCPR	CRC polynomial register
DR	data register
I2SCFGR	I2S configuration register
I2SPR	I2S prescaler register
RXCRCR	RX CRC register
RegisterBlock	Register block
SR	status register
TXCRCR	TX CRC register
===================================
  __IO uint32_t CR1;           /*!< SPI/I2S Control register 1,                      Address offset: 0x00 */
  __IO uint32_t CR2;           /*!< SPI Control register 2,                          Address offset: 0x04 */
  __IO uint32_t CFG1;          /*!< SPI Configuration register 1,                    Address offset: 0x08 */
  __IO uint32_t CFG2;          /*!< SPI Configuration register 2,                    Address offset: 0x0C */

  __IO uint32_t IER;           /*!< SPI/I2S Interrupt Enable register,               Address offset: 0x10 */
  __IO uint32_t SR;            /*!< SPI/I2S Status register,                         Address offset: 0x14 */
  __IO uint32_t IFCR;          /*!< SPI/I2S Interrupt/Status flags clear register,   Address offset: 0x18 */
  uint32_t      RESERVED0;     /*!< Reserved, 0x1C                                                        */
  __IO uint32_t TXDR;          /*!< SPI/I2S Transmit data register,                  Address offset: 0x20 */
  uint32_t      RESERVED1[3];  /*!< Reserved, 0x24-0x2C                                                   */
  __IO uint32_t RXDR;          /*!< SPI/I2S Receive data register,                   Address offset: 0x30 */
  uint32_t      RESERVED2[3];  /*!< Reserved, 0x34-0x3C                                                   */
  __IO uint32_t CRCPOLY;       /*!< SPI CRC Polynomial register,                     Address offset: 0x40 */
  __IO uint32_t TXCRC;         /*!< SPI Transmitter CRC register,                    Address offset: 0x44 */
  __IO uint32_t RXCRC;         /*!< SPI Receiver CRC register,                       Address offset: 0x48 */
  __IO uint32_t UDRDR;         /*!< SPI Underrun data register,                      Address offset: 0x4C */
  __IO uint32_t I2SCFGR;       /*!< I2S Configuration register,                      Address offset: 0x50 */



CFG1	CFG1 register accessor: an alias for Reg<CFG1_SPEC>
CFG2	CFG2 register accessor: an alias for Reg<CFG2_SPEC>
CR1	CR1 register accessor: an alias for Reg<CR1_SPEC>    8
CR2	CR2 register accessor: an alias for Reg<CR2_SPEC>
CRCPOLY	CRCPOLY register accessor: an alias for Reg<CRCPOLY_SPEC> 10
I2SCFGR	I2SCFGR register accessor: an alias for Reg<I2SCFGR_SPEC>  14
IER	IER register accessor: an alias for Reg<IER_SPEC>
IFCR	IFCR register accessor: an alias for Reg<IFCR_SPEC>
RXCRC	RXCRC register accessor: an alias for Reg<RXCRC_SPEC>
RXDR	RXDR register accessor: an alias for Reg<RXDR_SPEC>
SR	SR register accessor: an alias for Reg<SR_SPEC>
TXCRC	TXCRC register accessor: an alias for Reg<TXCRC_SPEC>
TXDR	TXDR register accessor: an alias for Reg<TXDR_SPEC>
UDRDR	UDRDR register accessor: an alias for Reg<UDRDR_SPEC>
#endif




#define STM_SPI_CR1_SPE  (1 << 6)
#define STM_SPI_CR1_MSTR (1 << 2)

#define STM_SPI_SR_RXNE   1

#define TYPE_STM32H7XX_SPI "stm32h7xx-spi"
OBJECT_DECLARE_SIMPLE_TYPE(STM32H7XXSPIState, STM32H7XX_SPI)

struct STM32H7XXSPIState {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t spi_cr1;
    uint32_t spi_cr2;
    uint32_t spi_sr;
    uint32_t spi_dr;
    uint32_t spi_crcpr;
    uint32_t spi_rxcrcr;
    uint32_t spi_txcrcr;
    uint32_t spi_i2scfgr;
    uint32_t spi_i2spr;

    qemu_irq irq;
    SSIBus *ssi;
};

#endif /* HW_STM32H7XX_SPI_H */
