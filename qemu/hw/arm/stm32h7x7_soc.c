/*
 * STM32F405 SoC
 *
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

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/module.h"
#include "qemu/log.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/arm/stm32h7x7_soc.h"
#include "hw/qdev-clock.h"
#include "hw/misc/unimp.h"
#include "migration/vmstate.h"

////////////////////



static uint64_t stm32h7x7_powermgt_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
    STM32H7X7PowerMgtState *s = (STM32H7X7PowerMgtState *)opaque;
    uint32_t res = 0;

    switch (offset) {
          case 0x0:
            res = s->cfg;
            static char bit_num=0;
            bit_num = bit_num +1;
            if (bit_num == 32) {
                bit_num=0;
            }
            res= (16384 | (1 << bit_num));

            qemu_log_mask(LOG_UNIMP,
                "stm32h7x7_powermgt_read: cfg offset 0x%08"HWADDR_PRIx
                " %d\n", offset,res);
                // RCC_FLAG_HSERDY
        break;

            case 0x10:
            res= s->cfg2;
          /*
             <name>SVOS</name>
              <description>System Stop mode voltage scaling
              selection These bits control the VCORE voltage level
              in system Stop mode, to obtain the best trade-off
              between power consumption and
              performance.</description>
              <bitOffset>14</bitOffset>
              <bitWidth>2</bitWidth>
              */
          break;

         case 0x18:
             res = s->offset_18;
            break;

           case 0x28:
              res = s->rpcsr;
           break;
          case 0xe8:
              res = s->hsem_lock;
           break;


          case 0x00000418:
          /*
              voltage levels ready bit for currently
              used VOS and SDLEVEL This bit is set to 1 by hardware
              when the voltage regulator and the SD converter are
              both disabled and Bypass mode is selected in PWR
              control register 3 (PWR_CR3)
          */
            res=8192;
          break;

    default:
        qemu_log_mask(LOG_UNIMP,
                      "stm32h7x7_powermgt_read: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void stm32h7x7_powermgt_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
    STM32H7X7PowerMgtState *s = (STM32H7X7PowerMgtState *)opaque;


    switch (offset) {
        case 0:
        qemu_log_mask(LOG_UNIMP,
                "stm32h7x7_powermgt_write: CFG\n");
        s->cfg = value;
        break;
        case 0x10:
           s->cfg2=value;
        break;

        /* 0x58024400
        // HSEM Read lock register
        tm32h7xx_powermgt_read: Unknown offset 0x000000e8
        stm32h7x7_powermgt_write: Unknown offset 0x000000e8


        RCC PLLs Clock Source Selection
        <resetValue>0x02020200</resetValue>
*/
    case 0xe8:
        s->hsem_lock = value;
        break;

    case 0x18:
        s->offset_18 = value;
        break;


    case 0x28:
        s->rpcsr = value;
        break;

    //case R_WDOG:
    //    qemu_log_mask(LOG_UNIMP,
    //                  "stm32h7x7_powermgt_write: WDOG\n");
    //    s->wdog = value;
    //    break;

    default:
        qemu_log_mask(LOG_UNIMP,
                      "stm32h7x7_powermgt_write: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        break;
    }
}

static const MemoryRegionOps stm32h7x7_powermgt_ops = {
    .read = stm32h7x7_powermgt_read,
    .write = stm32h7x7_powermgt_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static const VMStateDescription vmstate_stm32h7x7_powermgt = {
    .name = TYPE_STM32H7X7_POWERMGT,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(cfg, STM32H7X7PowerMgtState),
        VMSTATE_END_OF_LIST()
    }
};

static void stm32h7x7_powermgt_init(Object *obj)
{
    STM32H7X7PowerMgtState *s = STM32H7X7_POWERMGT(obj);

    memory_region_init_io(&s->iomem, obj, &stm32h7x7_powermgt_ops, s,
                          TYPE_STM32H7X7_POWERMGT, 0xc00);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void stm32h7x7_powermgt_reset(DeviceState *dev)
{
    STM32H7X7PowerMgtState *s = STM32H7X7_POWERMGT(dev);

    s->cfg = 0x00000000;
    s->rpcsr = 0x02020200;
    s->hsem_lock=0;
}

static void stm32h7x7_powermgt_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32h7x7_powermgt_reset;
    dc->vmsd = &vmstate_stm32h7x7_powermgt;
}

static const TypeInfo stm32h7x7_powermgt_info = {
    .name          = TYPE_STM32H7X7_POWERMGT,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32H7X7PowerMgtState),
    .class_init    = stm32h7x7_powermgt_class_init,
    .instance_init = stm32h7x7_powermgt_init,
};

static void stm32h7x7_powermgt_register_types(void)
{
    type_register_static(&stm32h7x7_powermgt_info);
}

type_init(stm32h7x7_powermgt_register_types)




////////////////////



#define SYSCFG_ADD                     0x40013800
static const uint32_t usart_addr[] = { 0x40011000, 0x40004400, 0x40004800,
                                       0x40004C00, 0x40005000, 0x40011400,
                                       0x40007800, 0x40007C00 };
/* At the moment only Timer 2 to 5 are modelled */
// CC Internal Clock Source Calibration , 0x40000000
static const uint32_t timer_addr[] = { 0x40000000, 0x40000400,
                                       0x40000800, 0x40000C00 };
static const uint32_t adc_addr[] = { 0x40012000, 0x40012100, 0x40012200,
                                     0x40012300, 0x40012400, 0x40012500 };
static const uint32_t spi_addr[] =   { 0x40013000, 0x40003800, 0x40003C00,
                                       0x40013400, 0x40015000, 0x40015400 };
#define EXTI_ADDR                      0x40013C00

#define SYSCFG_IRQ               71
static const int usart_irq[] = { 37, 38, 39, 52, 53, 71, 82, 83 };
static const int timer_irq[] = { 28, 29, 30, 50 };
#define ADC_IRQ 18
static const int spi_irq[] =   { 35, 36, 51, 0, 0, 0 };
static const int exti_irq[] =  { 6, 7, 8, 9, 10, 23, 23, 23, 23, 23, 40,
                                 40, 40, 40, 40, 40} ;


static void stm32h7x7_soc_initfn(Object *obj)
{
    STM32H7X7State *s = STM32H7X7_SOC(obj);
    int i;

    object_initialize_child(obj, "armv7m", &s->armv7m, TYPE_ARMV7M);

    object_initialize_child(obj, "syscfg", &s->syscfg, TYPE_STM32F4XX_SYSCFG);

    for (i = 0; i < STM_NUM_USARTS; i++) {
        object_initialize_child(obj, "usart[*]", &s->usart[i],
                                TYPE_STM32_UART);
    }

    for (i = 0; i < STM_NUM_TIMERS; i++) {
        object_initialize_child(obj, "timer[*]", &s->timer[i],
                                TYPE_STM32F2XX_TIMER);
    }

    for (i = 0; i < STM_NUM_ADCS; i++) {
        object_initialize_child(obj, "adc[*]", &s->adc[i], TYPE_STM32F2XX_ADC);
    }

    for (i = 0; i < STM_NUM_SPIS; i++) {
        object_initialize_child(obj, "spi[*]", &s->spi[i], TYPE_STM32H7XX_SPI);
    }

   object_initialize_child(obj, "qspi", &s->qspi,TYPE_STM32_QSPI);


    object_initialize_child(obj, "exti", &s->exti, TYPE_STM32F4XX_EXTI);

    s->sysclk = qdev_init_clock_in(DEVICE(s), "sysclk", NULL, NULL, 0);
    s->refclk = qdev_init_clock_in(DEVICE(s), "refclk", NULL, NULL, 0);

    object_initialize_child(obj, "pwr_crc", &s->pwr,TYPE_STM32H7X7_POWERMGT);

    object_initialize_child(obj, "flash", &s->flash_controller[1],TYPE_STM32H7XX_FLASH);

    object_initialize_child(obj, "ltdc", &s->ltdc,TYPE_LTDC);





}

static void stm32h7x7_soc_realize(DeviceState *dev_soc, Error **errp)
{
    STM32H7X7State *s = STM32H7X7_SOC(dev_soc);
    MemoryRegion *system_memory = get_system_memory();
    DeviceState *dev, *armv7m;
    SysBusDevice *busdev;
    Error *err = NULL;
    int i;

    /*
     * We use s->refclk internally and only define it with qdev_init_clock_in()
     * so it is correctly parented and not leaked on an init/deinit; it is not
     * intended as an externally exposed clock.
     */
    if (clock_has_source(s->refclk)) {
        error_setg(errp, "refclk clock must not be wired up by the board code");
        return;
    }

    if (!clock_has_source(s->sysclk)) {
        error_setg(errp, "sysclk clock must be wired up by the board code");
        return;
    }

    /*
     * TODO: ideally we should model the SoC RCC and its ability to
     * change the sysclk frequency and define different sysclk sources.
     */

    /* The refclk always runs at frequency HCLK / 8 */
    clock_set_mul_div(s->refclk, 8, 1);
    clock_set_source(s->refclk, s->sysclk);

    // In order to be able to load elf file, set as ram
    memory_region_init_ram(&s->flash, OBJECT(dev_soc), "STM32H750.flash",
                           FLASH_SIZE, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_init_alias(&s->flash_alias, OBJECT(dev_soc),
                             "STM32H750.flash.alias", &s->flash, 0,
                             FLASH_SIZE);

    memory_region_add_subregion(system_memory, FLASH_BASE_ADDRESS, &s->flash);
    memory_region_add_subregion(system_memory, 0, &s->flash_alias);

    memory_region_init_ram(&s->sram, OBJECT(dev_soc), "STM32H750.sram", SRAM_SIZE,
                           &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_add_subregion(system_memory, SRAM_BASE_ADDRESS, &s->sram);


// 0x24000938


// 512 KB
//#define RAM_D1_SIZE 0x80000
// 1MB
#define RAM_D1_SIZE 0x100000
//                 0x20000000
// Stack start     0x24044440
//                 0x24000938

    memory_region_init_ram(&s->DTC_RAM, OBJECT(dev_soc), "STM32H750.DTC_RAM",
                            0x20000, &err);

    //                      invalid             0x24000938
    // From qemu bootloader
    memory_region_add_subregion(system_memory, 0x20000000, &s->DTC_RAM);

    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }


    memory_region_init_ram(&s->RAM_D1, OBJECT(dev_soc), "STM32H750.RAM_D1",
                            0x80000, &err);

    //                      invalid             0x24000938
    // From qemu bootloader
    memory_region_add_subregion(system_memory, 0x24000000, &s->RAM_D1);

    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    memory_region_init_ram(&s->RAM_D2, OBJECT(dev_soc), "STM32H750.RAM_D2",
                            0x93000, &err);

    // From qemu bootloader
    memory_region_add_subregion(system_memory, 0x30000000, &s->RAM_D2);

    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    memory_region_init_ram(&s->RAM_D3, OBJECT(dev_soc), "STM32H750.RAM_D3",
                            0x10000, &err);

    // From qemu bootloader
    memory_region_add_subregion(system_memory, 0x38000000, &s->RAM_D3);

    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }


/*
DTCMRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
RAM_D1 (xrw)      : ORIGIN = 0x24000000, LENGTH = 512K
RAM_D2 (xrw)      : ORIGIN = 0x30000000, LENGTH = 288K
RAM_D3 (xrw)      : ORIGIN = 0x38000000, LENGTH = 64K
ITCMRAM (xrw)      : ORIGIN = 0x00000000, LENGTH = 64K

 DTCMRAM:         128 KB 
 RAM_D1:          512 KB 
 RAM_D2:          288 KB 
 RAM_D3:          64 KB  
 ITCMRAM:         64 KB  


    target_add_ram(t, 0x00000000, 0x10000);  // ITCM Ram,  64 k 
	target_add_ram(t, 0x20000000, 0x20000); // DTCM Ram, 128 k 
	target_add_ram(t, 0x24000000, 0x80000); // AXI Ram,  512 k 
	target_add_ram(t, 0x30000000, 0x20000); // AHB SRAM1, 128 k 
	target_add_ram(t, 0x30020000, 0x20000); // AHB SRAM2, 128 k 
	target_add_ram(t, 0x30040000, 0x08000); // AHB SRAM3,  32 k 
	target_add_ram(t, 0x38000000, 0x10000); // AHB SRAM4,  64 k 

*/


    // dram, olas...
    memory_region_init_ram(&s->dram, OBJECT(dev_soc), "STM32H750.dram", DRAM_SIZE,
                           &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    memory_region_add_subregion(system_memory, DRAM_BASE_ADDRESS,  &s->dram);

//spi_flash

    // spi_flash, olas...
    memory_region_init_ram(&s->spi_flash, OBJECT(dev_soc), "STM32H750.spiflash", 4 * SPI_FLASH__SIZE,
                           &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    memory_region_add_subregion(system_memory, SPI_FLASH_BASE_ADDRESS,  &s->spi_flash);



// #define SPI_FLASH_BASE_ADDRESS 0x90000000
// #define SPI_FLASH__SIZE 8 * (1024 * 1024)



    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_uint32(armv7m, "num-irq", 96);
    qdev_prop_set_string(armv7m, "cpu-type", s->cpu_type);
    // Olas, bitband??
    qdev_prop_set_bit(armv7m, "enable-bitband", true);
    qdev_connect_clock_in(armv7m, "cpuclk", s->sysclk);
    qdev_connect_clock_in(armv7m, "refclk", s->refclk);
    object_property_set_link(OBJECT(&s->armv7m), "memory",
                             OBJECT(system_memory), &error_abort);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->armv7m), errp)) {
        return;
    }

    /* System configuration controller */
    dev = DEVICE(&s->syscfg);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->syscfg), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, SYSCFG_ADD);
    sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, SYSCFG_IRQ));

    /* Attach UART (uses USART registers) and USART controllers */
    for (i = 0; i < STM_NUM_USARTS; i++) {
        dev = DEVICE(&(s->usart[i]));
        qdev_prop_set_chr(dev, "chardev", serial_hd(i));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->usart[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, usart_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, usart_irq[i]));
    }

    /* Timer 2 to 5 */
    for (i = 0; i < STM_NUM_TIMERS; i++) {
        dev = DEVICE(&(s->timer[i]));
        qdev_prop_set_uint64(dev, "clock-frequency", 1000000000);
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->timer[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, timer_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, timer_irq[i]));
    }



    /* ADC device, the IRQs are ORed together */
    if (!object_initialize_child_with_props(OBJECT(s), "adc-orirq",
                                            &s->adc_irqs, sizeof(s->adc_irqs),
                                            TYPE_OR_IRQ, errp, NULL)) {
        return;
    }
    object_property_set_int(OBJECT(&s->adc_irqs), "num-lines", STM_NUM_ADCS,
                            &error_abort);
    if (!qdev_realize(DEVICE(&s->adc_irqs), NULL, errp)) {
        return;
    }
    qdev_connect_gpio_out(DEVICE(&s->adc_irqs), 0,
                          qdev_get_gpio_in(armv7m, ADC_IRQ));

    for (i = 0; i < STM_NUM_ADCS; i++) {
        dev = DEVICE(&(s->adc[i]));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->adc[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, adc_addr[i]);
        sysbus_connect_irq(busdev, 0,
                           qdev_get_gpio_in(DEVICE(&s->adc_irqs), i));
    }

    /* SPI devices */
    for (i = 0; i < STM_NUM_SPIS; i++) {
        dev = DEVICE(&(s->spi[i]));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->spi[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, spi_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, spi_irq[i]));
    }
    {
        dev = DEVICE(&s->qspi);
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->qspi), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, 0x52005000);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, 92));
        
    }
   // "QUADSPI",0x52005000,0x400

    /* EXTI device */
    dev = DEVICE(&s->exti);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->exti), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, EXTI_ADDR);
    for (i = 0; i < 16; i++) {
        sysbus_connect_irq(busdev, i, qdev_get_gpio_in(armv7m, exti_irq[i]));
    }
    for (i = 0; i < 16; i++) {
        qdev_connect_gpio_out(DEVICE(&s->syscfg), i, qdev_get_gpio_in(dev, i));
    }


    /* Power Management */
    dev = DEVICE(&s->pwr);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->pwr), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, 0x58024400);


    // create_unimplemented_device("Flash",0x52002000,0x1400);

    dev = DEVICE(&s->flash_controller[1]);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->flash_controller[1]), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, 0x52002000);


    dev = DEVICE(&s->ltdc);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->ltdc), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, 0x50001000);






    //create_unimplemented_device("PWR_CRC_RCC",0x58024400,0xc00);
    // STM32H750_POWERMGT

/*
    f4xx
    create_unimplemented_device("timer[7]",    0x40001400, 0x400);
    create_unimplemented_device("timer[12]",   0x40001800, 0x400);
    create_unimplemented_device("timer[6]",    0x40001000, 0x400);
    create_unimplemented_device("timer[13]",   0x40001C00, 0x400);
    create_unimplemented_device("timer[14]",   0x40002000, 0x400);
    create_unimplemented_device("RTC and BKP", 0x40002800, 0x400);
*/
/* 7x7
"Name","Start","Length"
"LPTIM1_TIM14_TIM13_TIM12_TIM7_TIM6_TIM5_TIM4_TIM3_TIM2","40000000","0x2800"
"WWDG1","40002c00","0x400"
"I2C1_I2C2_I2C3_UART5_UART4_USART3_USART2_SPDIFRX_SPI3_SPI2","40003800","0x2800"
"CEC","40006c00","0x400"
"DAC_UART7_UART8","40007400","0xc00"
"CRS_SWPMI","40008400","0x800"
"OPAMP_MDIOS","40009000","0x800"
"FDCAN1_FDCAN2_CAN_CCU","4000a000","0xc00"
"TIM1_TIM8","40010000","0x800"
"USART1_USART6","40011000","0x800"
"SPI1_SPI4","40013000","0x800"
"TIM16_TIM17_TIM15","40014000","0xc00"
"SPI5","40015000","0x400"
"SAI1_SAI2_SAI3","40015800","0xc00"
"DFSDM_HRTIM_Master_HRTIM_TIMA_HRTIM_TIMB_HRTIM_TIMC_HRTIM_TIMD_HRTIM_TIME","40017000","0x700"
"HRTIM_Common","40017780","0x80"
"DMAMUX1_DMA2_DMA1","40020000","0xc00"
"ADC1_2","40022000","0x400"
"ART","40024400","0x400"
"Ethernet_MAC","40028000","0x1400"
"OTG_HS_HOST_OTG_HS_GLOBAL_OTG_HS_DEVICE","40040000","0xd00"
"OTG_FS_HOST_OTG_FS_GLOBAL_OTG_HS_PWRCLK_OTG_FS_DEVICE","40040e00","0x3ff00"
"OTG_FS_PWRCLK","40080e00","0x3f200"
"DCMI","48020000","0x400"
"RNG_HASH_CRYP","48021000","0xc00"
"SDMMC2","48022400","0x400"
"RAMECC2","48023000","0x400"
"LTDC","50001000","0x1000"

"WWDG2","50003000","0x400"
"DMA2D_MDMA","52000000","0x1400"
"FMC_JPEG_QUADSPI","52003000","0x3000"
"SDMMC1","52007000","0x1000"
"RAMECC1","52009000","0x400"
"SYSCFG_EXTI","58000000","0x800"
"LPUART1","58000c00","0x400"
"SPI6","58001400","0x400"
"I2C4","58001c00","0x400"
"LPTIM2_LPTIM3_LPTIM4_LPTIM5","58002400","0x1000"
"COMP1_VREFBUF_RTC","58003800","0xc00"
"IWDG1_IWDG2","58004800","0x800"
"SAI4","58005400","0x400"
"GPIOA_GPIOB_GPIOC_GPIOD_GPIOE_GPIOF_GPIOG_GPIOH_GPIOI_GPIOJ_GPIOK","58020000","0x2c00"
"PWR_CRC_RCC","58024400","0xc00"
"BDMA_DMAMUX2","58025400","0x800"
"HSEM_ADC3","58026000","0x800"
"RAMECC3","58027000","0x400"
"SCB_ACTRL","e000e008","0x5"
"STK","e000e010","0x11"
"NVIC","e000e100","0x351"
"SCB","e000ed00","0x41"
"FPU_CPACR","e000ed88","0x5"
"MPU","e000ed90","0x15"
"NVIC_STIR","e000ef00","0x5"
*/

////////////
    create_unimplemented_device("CEC",0x40006c00,0x400);
    create_unimplemented_device("DAC_UART7_UART8",0x40007400,0xc00);
    create_unimplemented_device("CRS_SWPMI",0x40008400,0x800);
    create_unimplemented_device("MDIOS_OPAMP",0x40009000,0x800);
    create_unimplemented_device("FDCAN1_FDCAN2_CAN_CCU",0x4000a000,0xc00);
    //create_unimplemented_device("TIM1_TIM8",0x40010000,0x800);
    //create_unimplemented_device("USART1_USART6",0x40011000,0x800);
    create_unimplemented_device("SPI1_SPI4",0x40013000,0x800);
    create_unimplemented_device("TIM16_TIM17_TIM15",0x40014000,0xc00);
    create_unimplemented_device("SPI5",0x40015000,0x400);
    create_unimplemented_device("SAI1_SAI2_SAI3",0x40015800,0xc00);
    create_unimplemented_device("HRTIM_Master_HRTIM_TIMA_HRTIM_TIMB_HRTIM_TIMC_HRTIM_TIMD_HRTIM_TIME_DFSDM",0x40017000,0x700);
    create_unimplemented_device("HRTIM_Common",0x40017780,0x80);
    create_unimplemented_device("DMAMUX1_DMA2_DMA1",0x40020000,0xc00);
    create_unimplemented_device("ADC1",0x40022000,0xd1);
    create_unimplemented_device("ADC2",0x40022100,0xd1);
    create_unimplemented_device("ADC12_Common",0x40022300,0x11);
    create_unimplemented_device("Ethernet_MAC",0x40028000,0x1400);
    create_unimplemented_device("OTG1_HS_GLOBAL_OTG1_HS_HOST_OTG1_HS_DEVICE",0x40040000,0xc00);
    create_unimplemented_device("OTG2_HS_GLOBAL_OTG2_HS_HOST_OTG2_HS_DEVICE_OTG1_HS_PWRCLK",0x40040e00,0x3fe00);
    create_unimplemented_device("OTG2_HS_PWRCLK",0x40080e00,0x3f200);
    create_unimplemented_device("DCMI",0x48020000,0x400);
    create_unimplemented_device("RNG_HASH_CRYP",0x48021000,0xc00);
    create_unimplemented_device("SDMMC2",0x48022400,0x3fd);
//    "","",""
   create_unimplemented_device("RAMECC2",0x48023000,0x400);
//    create_unimplemented_device("DELAY_Block_SDMMC2",0x48022800,0x1000);
//    create_unimplemented_device("LTDC",0x50001000,0x1000);
    create_unimplemented_device("WWDG2",0x50003000,0x400);
    create_unimplemented_device("DMA2D_MDMA",0x52000000,0x1400);
    create_unimplemented_device("FMC_JPEG_QUADSPI",0x52003000,0x3000);
    create_unimplemented_device("SDMMC1",0x52007000,0x1000);
    create_unimplemented_device("RAMECC1",0x52009000,0x400);
    create_unimplemented_device("SYSCFG_EXTI",0x58000000,0x800);
    create_unimplemented_device("LPUART1",0x58000c00,0x400);
    create_unimplemented_device("SPI6",0x58001400,0x400);
    create_unimplemented_device("I2C4",0x58001c00,0x400);
    create_unimplemented_device("LPTIM2_LPTIM3_LPTIM4_LPTIM5",0x58002400,0x1000);
    create_unimplemented_device("COMP1_VREFBUF_RTC",0x58003800,0xc00);
    create_unimplemented_device("IWDG1_IWDG2",0x58004800,0x800);
    create_unimplemented_device("SAI4",0x58005400,0x400);
    create_unimplemented_device("GPIOA_GPIOB_GPIOC_GPIOD_GPIOE_GPIOF_GPIOG_GPIOH_GPIOI_GPIOJ_GPIOK",0x58020000,0x2c00);
    //create_unimplemented_device("PWR_CRC_RCC",0x58024400,0xc00);
    create_unimplemented_device("BDMA_DMAMUX2",0x58025400,0x800);
    create_unimplemented_device("HSEM_ADC3",0x58026000,0x800);
    create_unimplemented_device("RAMECC3",0x58027000,0x400);
    create_unimplemented_device("SCB_ACTRL",0xe000e008,0x5);
    create_unimplemented_device("STK",0xe000e010,0x11);
    create_unimplemented_device("NVIC",0xe000e100,0x351);
    create_unimplemented_device("SCB",0xe000ed00,0x41);
    create_unimplemented_device("FPU_CPACR",0xe000ed88,0x5);
    create_unimplemented_device("MPU",0xe000ed90,0x15);
    create_unimplemented_device("NVIC_STIR",0xe000ef00,0x5);

    // create_unimplemented_device("Flash",0x52002000,0x1400);
    create_unimplemented_device("DBGMCU",0x5c001000,0x400);

    create_unimplemented_device("SWITCH_MATRIX",0x51008100,0x200);

    


 /*   
    create_unimplemented_device("WWDG",0x50003000,0x400);
    create_unimplemented_device("AXI",0x51000000,0x100000);
    create_unimplemented_device("DMA2D_MDMA",0x52000000,0x1400);
                                       //   0x52005000
    create_unimplemented_device("JPEG_Flash",0x52002000,0x1400);
    create_unimplemented_device("FMC",0x52004000,0x400);
    //create_unimplemented_device("QUADSPI",0x52005000,0x400);
    create_unimplemented_device("SDMMC1_DELAY_Block_QUADSPI",0x52006000,0x13fd);
    create_unimplemented_device("DELAY_Block_SDMMC1",0x52008000,0x1000);
    create_unimplemented_device("SYSCFG_EXTI",0x58000000,0x800);
    create_unimplemented_device("LPUART1",0x58000c00,0x400);
    create_unimplemented_device("SPI6",0x58001400,0x400);
    create_unimplemented_device("I2C4",0x58001c00,0x400);
    create_unimplemented_device("LPTIM2_LPTIM3_LPTIM4_LPTIM5",0x58002400,0x1000);
    create_unimplemented_device("COMP1_VREFBUF_RTC",0x58003800,0xc00);
    create_unimplemented_device("IWDG",0x58004800,0x400);
    create_unimplemented_device("SAI4",0x58005400,0x400);
    create_unimplemented_device("GPIOA_GPIOB_GPIOC_GPIOD_GPIOE_GPIOF_GPIOG_GPIOH_GPIOI_GPIOJ_GPIOK",0x58020000,0x2c00);
    //create_unimplemented_device("PWR_CRC_RCC",0x58024400,0xc00);
    // STM32H750_POWERMGT
    create_unimplemented_device("BDMA_DMAMUX2",0x58025400,0x800);
    create_unimplemented_device("ADC3",0x58026000,0xd1);
    create_unimplemented_device("ADC3_Common",0x58026300,0x11);
    create_unimplemented_device("HSEM",0x58026400,0x400);
    create_unimplemented_device("DBGMCU",0x5c001000,0x400);
    create_unimplemented_device("SCB_ACTRL",0xe000e008,0x5);
    create_unimplemented_device("STK",0xe000e010,0x11);
    create_unimplemented_device("NVIC",0xe000e100,0x355);
    create_unimplemented_device("SCB",0xe000ed00,0x41);
    create_unimplemented_device("PF",0xe000ed78,0xd);
    create_unimplemented_device("FPU_CPACR",0xe000ed88,0x5);
    create_unimplemented_device("MPU",0xe000ed90,0x15);
    create_unimplemented_device("NVIC_STIR",0xe000ef00,0x5);
    create_unimplemented_device("FPU",0xe000ef34,0xd);
    create_unimplemented_device("AC",0xe000ef90,0x1d);
 */

}

static Property stm32h7x7_soc_properties[] = {
    DEFINE_PROP_STRING("cpu-type", STM32H7X7State, cpu_type),
    DEFINE_PROP_END_OF_LIST(),
};

static void stm32h7x7_soc_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = stm32h7x7_soc_realize;
    device_class_set_props(dc, stm32h7x7_soc_properties);
    /* No vmstate or reset required: device has no internal state */
}

static const TypeInfo stm32h7x7_soc_info = {
    .name          = TYPE_STM32H7X7_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32H7X7State),
    .instance_init = stm32h7x7_soc_initfn,
    .class_init    = stm32h7x7_soc_class_init,
};

static void stm32h7x7_soc_types(void)
{
    type_register_static(&stm32h7x7_soc_info);
}

type_init(stm32h7x7_soc_types)
