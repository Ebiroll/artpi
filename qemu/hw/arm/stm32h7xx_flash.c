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



static uint64_t stm32h7xx_flash_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
    STM32H7XXFlashState *s = (STM32H7XXFlashState *)opaque;
    uint32_t res = 0;

    switch (offset) {
          case 0x0:
            res = s->acr;

            qemu_log_mask(LOG_UNIMP,
                "stm32h7xx_flash_read: cfg offset 0x%08"HWADDR_PRIx
                " %d\n", offset,res);
                // RCC_FLAG_HSERDY
        break;


    default:
        qemu_log_mask(LOG_UNIMP,
                      "stm32h7xx_flash_read: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void stm32h7xx_flash_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
    STM32H7XXFlashState *s = (STM32H7XXFlashState *)opaque;


    switch (offset) {
        case 0:
        qemu_log_mask(LOG_UNIMP,
                "stm32h7xx_flash_write: ACR\n");
        s->acr = value;
        break;
//        case 0x10:
//           s->cfg2=value;
//        break;

        /* 0x58024400
        // HSEM Read lock register
        tm32h7xx_powermgt_read: Unknown offset 0x000000e8
        stm32h7xx_flash_write: Unknown offset 0x000000e8


        RCC PLLs Clock Source Selection
        <resetValue>0x02020200</resetValue>
*/
//    case 0xe8:
//        s->hsem_lock = value;
//        break;


//    case 0x28:
//        s->rpcsr = value;
//        break;

    //case R_WDOG:
    //    qemu_log_mask(LOG_UNIMP,
    //                  "stm32h7xx_flash_write: WDOG\n");
    //    s->wdog = value;
    //    break;

    default:
        qemu_log_mask(LOG_UNIMP,
                      "stm32h7xx_flash_write: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        break;
    }
}

static const MemoryRegionOps stm32h7xx_flash_ops = {
    .read = stm32h7xx_flash_read,
    .write = stm32h7xx_flash_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static const VMStateDescription vmstate_stm32h7xx_flash = {
    .name = TYPE_STM32H7XX_FLASH,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(cfg, STM32H7X7PowerMgtState),
        VMSTATE_END_OF_LIST()
    }
};

static void stm32h7xx_flash_init(Object *obj)
{
    STM32H7XXFlashState *s = STM32H7XX_FLASH(obj);

    memory_region_init_io(&s->iomem, obj, &stm32h7xx_flash_ops, s,
                          TYPE_STM32H7XX_FLASH, 0xc00);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void stm32h7xx_flash_reset(DeviceState *dev)
{
    STM32H7XXFlashState *s = STM32H7XX_FLASH(dev);

    s->acr=0;
    //s->cfg = 0x00000000;
    //s->rpcsr = 0x02020200;
    //s->hsem_lock=0;
}

static void stm32h7xx_flash_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32h7xx_flash_reset;
    dc->vmsd = &vmstate_stm32h7xx_flash;
}

static const TypeInfo stm32h7xx_flash_info = {
    .name          = TYPE_STM32H7XX_FLASH,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32H7XXFlashState),
    .class_init    = stm32h7xx_flash_class_init,
    .instance_init = stm32h7xx_flash_init,
};

static void stm32h7xx_flash_register_types(void)
{
    type_register_static(&stm32h7xx_flash_info);
}

type_init(stm32h7xx_flash_register_types)


