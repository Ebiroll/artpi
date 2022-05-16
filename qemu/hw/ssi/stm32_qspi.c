/*
 * STM32 QSPI
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
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/ssi/stm32_qspi.h"
#include "migration/vmstate.h"

#ifndef STM_SPI_ERR_DEBUG
#define STM_SPI_ERR_DEBUG 0
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (STM_SPI_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0)

#define DB_PRINT(fmt, args...) DB_PRINT_L(0, fmt, ## args)

static void stm32_qspi_reset(DeviceState *dev)
{
    STM32QSPIState *s = STM32_QSPI(dev);

    s->spi_cr1 = 0x00000000;
    s->spi_cr2 = 0x00000000;
    s->spi_cfg1 = 0x00000000;
    s->spi_cfg2 = 0x00000000;
    s->spi_sr = 0x0000000A;
    s->spi_dr = 0x0000000C;
    s->spi_crcpr = 0x00000007;
    s->spi_rxcrcr = 0x00000000;
    s->spi_txcrcr = 0x00000000;
    s->spi_i2scfgr = 0x00000000;
    s->spi_i2spr = 0x00000002;
}

static void stm32_qspi_transfer(STM32QSPIState *s)
{
    DB_PRINT("Data to send: 0x%x\n", s->spi_dr);

    s->spi_dr = ssi_transfer(s->ssi, s->spi_dr);
    s->spi_sr |= STM_SPI_SR_RXNE;

    DB_PRINT("Data received: 0x%x\n", s->spi_dr);
}

static uint64_t stm32_qspi_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    STM32QSPIState *s = opaque;

    DB_PRINT("Address: 0x%" HWADDR_PRIx "\n", addr);

    switch (addr) {
    case STM32_SPI_CR1:
        return s->spi_cr1;
    case STM32_SPI_CR2:
        qemu_log_mask(LOG_UNIMP, "%s: CR2 are not implemented\n",
                      __func__);
        return s->spi_cr2;
    case STM32_SPI_CFG1:
        qemu_log_mask(LOG_UNIMP, "%s: CFG1\n",
                      __func__);
        return s->spi_cfg1;
    case STM32_SPI_CFG2:
        qemu_log_mask(LOG_UNIMP, "%s: CFG2\n",
                      __func__);
        return s->spi_cfg2;
    case STM32_SPI_SR:
        return s->spi_sr;
    case STM32_SPI_TXDR:
        stm32_qspi_transfer(s);
        s->spi_sr &= ~STM_SPI_SR_RXNE;
        return s->spi_dr;
    //case STM32_SPI_RXCRCR:
    //    qemu_log_mask(LOG_UNIMP, "%s: CRC is not implemented, the registers " 
    //                  "are included for compatibility\n", __func__);
    //    return s->spi_crcpr;
    case STM32_SPI_RXCRCR:
        qemu_log_mask(LOG_UNIMP, "%s: CRC is not implemented, the registers " \
                      "are included for compatibility\n", __func__);
        return s->spi_rxcrcr;
    case STM32_SPI_TXCRCR:
        qemu_log_mask(LOG_UNIMP, "%s: CRC is not implemented, the registers " \
                      "are included for compatibility\n", __func__);
        return s->spi_txcrcr;
    case STM32_SPI_CFGR:
        qemu_log_mask(LOG_UNIMP, "%s: cfgr not implemented, the registers " \
                      "are included for compatibility\n", __func__);
        return s->spi_i2scfgr;
    //case STM32_SPI_I2SPR:
    //    qemu_log_mask(LOG_UNIMP, "%s: I2S is not implemented, the registers " 
    //                  "are included for compatibility\n", __func__);
    //    return s->spi_i2spr;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
                      __func__, addr);
    }

    return 0;
}

static void stm32_qspi_write(void *opaque, hwaddr addr,
                                uint64_t val64, unsigned int size)
{
    STM32QSPIState *s = opaque;
    uint32_t value = val64;

    DB_PRINT("Address: 0x%" HWADDR_PRIx ", Value: 0x%x\n", addr, value);

    switch (addr) {
    case STM32_SPI_CR1:
        s->spi_cr1 = value;
        return;
    case STM32_SPI_CR2:
        qemu_log_mask(LOG_UNIMP, "%s: " \
                      "cr2 not implemented\n", __func__);
        s->spi_cr2 = value;
        return;
    case STM32_SPI_CFG1:
        qemu_log_mask(LOG_UNIMP, "%s: " \
                      "CFG1 not implemented\n", __func__);
        s->spi_cfg1 = value;
        return;
    case STM32_SPI_CFG2:
        qemu_log_mask(LOG_UNIMP, "%s: " \
                      "CFG2 not implemented\n", __func__);
        s->spi_cfg2 = value;
        return;

    case STM32_SPI_SR:
        /* Read only register, except for clearing the CRCERR bit, which
         * is not supported
         */
        return;
    case STM32_SPI_TXDR:
        s->spi_dr = value;
        stm32_qspi_transfer(s);
        return;
    //case STM32_SPI_CRCPR:
    //    qemu_log_mask(LOG_UNIMP, "%s: CRC is not implemented\n", __func__);
    //    return;
    case STM32_SPI_RXCRCR:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Read only register: " \
                      "0x%" HWADDR_PRIx "\n", __func__, addr);
        return;
    case STM32_SPI_TXCRCR:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Read only register: " \
                      "0x%" HWADDR_PRIx "\n", __func__, addr);
        return;
    case STM32_SPI_CFGR:
        qemu_log_mask(LOG_UNIMP, "%s: " \
                      "CFGR is not implemented\n", __func__);
        return;
    //case STM32_SPI_I2SPR:
    //    qemu_log_mask(LOG_UNIMP, "%s: " \
    //                  "I2S is not implemented\n", __func__);
    //    return;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Bad offset 0x%" HWADDR_PRIx "\n", __func__, addr);
    }
}

static const MemoryRegionOps stm32_qspi_ops = {
    .read = stm32_qspi_read,
    .write = stm32_qspi_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static const VMStateDescription vmstate_stm32_qspi = {
    .name = TYPE_STM32_QSPI,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(spi_cr1, STM32QSPIState),
        VMSTATE_UINT32(spi_cr2, STM32QSPIState),
        VMSTATE_UINT32(spi_sr, STM32QSPIState),
        VMSTATE_UINT32(spi_dr, STM32QSPIState),
        VMSTATE_UINT32(spi_crcpr, STM32QSPIState),
        VMSTATE_UINT32(spi_rxcrcr, STM32QSPIState),
        VMSTATE_UINT32(spi_txcrcr, STM32QSPIState),
        VMSTATE_UINT32(spi_i2scfgr, STM32QSPIState),
        VMSTATE_UINT32(spi_i2spr, STM32QSPIState),
        VMSTATE_END_OF_LIST()
    }
};

static void stm32_qspi_init(Object *obj)
{
    STM32QSPIState *s = STM32_QSPI(obj);
    DeviceState *dev = DEVICE(obj);

    memory_region_init_io(&s->mmio, obj, &stm32_qspi_ops, s,
                          TYPE_STM32_QSPI, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);

    s->ssi = ssi_create_bus(dev, "ssi");
}

static void stm32_qspi_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32_qspi_reset;
    dc->vmsd = &vmstate_stm32_qspi;
}

static const TypeInfo stm32_qspi_info = {
    .name          = TYPE_STM32_QSPI,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32QSPIState),
    .instance_init = stm32_qspi_init,
    .class_init    = stm32_qspi_class_init,
};

static void stm32_qspi_register_types(void)
{
    type_register_static(&stm32_qspi_info);
}

type_init(stm32_qspi_register_types)
