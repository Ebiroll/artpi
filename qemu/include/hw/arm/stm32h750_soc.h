/*
 * STM32H7 SoC
 *
 * Copyright (c) 2022 Olof Astrand <olof.astrand@gmail.com>
 * Copyright (c) 2014 Alistair Francis <alistair@alistair23.me>
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

#ifndef HW_ARM_STM32H750_SOC_H
#define HW_ARM_STM32H750_SOC_H

#include "hw/misc/stm32f4xx_syscfg.h"
#include "hw/timer/stm32f2xx_timer.h"
#include "hw/char/stm32_uart.h"
#include "hw/adc/stm32f2xx_adc.h"
#include "hw/misc/stm32f4xx_exti.h"
#include "hw/or-irq.h"
#include "hw/ssi/stm32f2xx_spi.h"
#include "hw/ssi/stm32_qspi.h"
#include "hw/arm/armv7m.h"
#include "qom/object.h"

#define TYPE_STM32H750_SOC "stm32h750-soc"
OBJECT_DECLARE_SIMPLE_TYPE(STM32H750State, STM32H750_SOC)

#define STM_NUM_USARTS 7
#define STM_NUM_TIMERS 4
#define STM_NUM_ADCS 6
#define STM_NUM_SPIS 6

#define FLASH_BASE_ADDRESS 0x08000000
#define FLASH_SIZE 8 * (1024 * 1024)
#define SRAM_BASE_ADDRESS 0x20000000
//#define SRAM_SIZE (192 * 1024)
#define SRAM_SIZE 8 * (1024 * 1024)


#define DRAM_BASE_ADDRESS 0xc0000000
#define DRAM_SIZE 0x2000000
// 8000 * 1024,  MPU_REGION_SIZE_32MB


#define SPI_FLASH_BASE_ADDRESS 0x90000000
#define SPI_FLASH__SIZE 8 * (1024 * 1024)


// ----------------- PWR_CRC_RCC


#define TYPE_STM32H750_POWERMGT "stm32h7-power"
OBJECT_DECLARE_SIMPLE_TYPE(STM32H750PowerMgtState, STM32H750_POWERMGT)

struct STM32H750PowerMgtState {
    SysBusDevice busdev;
    MemoryRegion iomem;

    uint32_t cfg;
};


// -------------------------


struct STM32H750State {
    /*< private >*/
    SysBusDevice parent_obj;
    /*< public >*/

    char *cpu_type;

    ARMv7MState armv7m;
    //ARMv7MState armv4m;


    STM32F4xxSyscfgState syscfg;
    STM32F4xxExtiState exti;
    STM32UartState usart[STM_NUM_USARTS];
    STM32F2XXTimerState timer[STM_NUM_TIMERS];
    qemu_or_irq adc_irqs;
    STM32F2XXADCState adc[STM_NUM_ADCS];
    STM32F2XXSPIState spi[STM_NUM_SPIS];
    STM32QSPIState qspi;

    MemoryRegion sram;
    MemoryRegion flash;
    MemoryRegion flash_alias;
    MemoryRegion spi_flash;

    MemoryRegion dram;
    MemoryRegion  RAM_D1;

    STM32H750PowerMgtState pwr;


    Clock *sysclk;
    Clock *refclk;
};

#endif
