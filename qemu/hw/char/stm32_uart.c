/*
 * STM32F2XX USART
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
#include "hw/char/stm32_uart.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-properties-system.h"
#include "qemu/log.h"
#include "qemu/module.h"

#ifndef STM_USART_ERR_DEBUG
#define STM_USART_ERR_DEBUG 0
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (STM_USART_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0)

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)

static int stm32_uart_can_receive(void *opaque)
{
    STM32UartState *s = opaque;

    if (!(s->usart_sr & USART_SR_RXNE)) {
        return 1;
    }

    return 0;
}

static void stm32_uart_receive(void *opaque, const uint8_t *buf, int size)
{
    STM32UartState *s = opaque;

    if (!(s->usart_cr1 & USART_CR1_UE && s->usart_cr1 & USART_CR1_RE)) {
        /* USART not enabled - drop the chars */
        DB_PRINT("Dropping the chars\n");
        return;
    }

    s->usart_dr = *buf;
    s->usart_sr |= USART_SR_RXNE;

    if (s->usart_cr1 & USART_CR1_RXNEIE) {
        qemu_set_irq(s->irq, 1);
    }

    DB_PRINT("Receiving: %c\n", s->usart_dr);
}

static void stm32_uart_reset(DeviceState *dev)
{
    STM32UartState *s = STM32_UART(dev);

    s->usart_sr = USART_SR_RESET;
    s->usart_dr = 0x00000000;
    s->usart_brr = 0x00000000;
    s->usart_cr1 = 0x00000000;
    s->usart_cr2 = 0x00000000;
    s->usart_cr3 = 0x00000000;
    s->usart_gtpr = 0x00000000;

    qemu_set_irq(s->irq, 0);
}

static uint64_t stm32_uart_read(void *opaque, hwaddr addr,
                                       unsigned int size)
{
    STM32UartState *s = opaque;
    uint64_t retvalue;

    DB_PRINT("Read 0x%"HWADDR_PRIx"\n", addr);

    switch (addr) {
        case USART_ISR:
         retvalue = 0x200000 | 0x400000 | 0x80;
        //qemu_log_mask(LOG_GUEST_ERROR,
        //              "ISR %s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
        return retvalue;
    case USART_TDR:
        retvalue = s->usart_sr;
        qemu_chr_fe_accept_input(&s->chr);
        return retvalue;
    case USART_RDR:
        DB_PRINT("Value: 0x%" PRIx32 ", %c\n", s->usart_dr, (char) s->usart_dr);
        retvalue = s->usart_dr & 0x3FF;
        s->usart_sr &= ~USART_SR_RXNE;
        qemu_chr_fe_accept_input(&s->chr);
        qemu_set_irq(s->irq, 0);
        return retvalue;
    case USART_BRR:
        return s->usart_brr;
    case USART_CR1:
        return s->usart_cr1;
    case USART_CR2:
        return s->usart_cr2;
    case USART_CR3:
        return s->usart_cr3;
    case USART_GTPR:
        return s->usart_gtpr;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
        return 0;
    }

    return 0;
}

static void stm32_uart_write(void *opaque, hwaddr addr,
                                  uint64_t val64, unsigned int size)
{
    STM32UartState *s = opaque;
    uint32_t value = val64;
    unsigned char ch;

    DB_PRINT("Write 0x%" PRIx32 ", 0x%"HWADDR_PRIx"\n", value, addr);

    switch (addr) {
    case USART_RDR:
        if (value <= 0x3FF) {
            /* I/O being synchronous, TXE is always set. In addition, it may
               only be set by hardware, so keep it set here. */
            s->usart_sr = value | USART_SR_TXE;
        } else {
            s->usart_sr &= value;
        }
        if (!(s->usart_sr & USART_SR_RXNE)) {
            qemu_set_irq(s->irq, 0);
        }
        return;
    case USART_TDR:
        if (value < 0xF000) {
            ch = value;
            printf("%c",ch);
            /* XXX this blocks entire thread. Rewrite to use
             * qemu_chr_fe_write and background I/O callbacks */
            qemu_chr_fe_write_all(&s->chr, &ch, 1);
            /* XXX I/O are currently synchronous, making it impossible for
               software to observe transient states where TXE or TC aren't
               set. Unlike TXE however, which is read-only, software may
               clear TC by writing 0 to the SR register, so set it again
               on each write. */
            s->usart_sr |= USART_SR_TC;
        }
        return;
    case USART_BRR:
        s->usart_brr = value;
        return;
    case USART_CR1:
        s->usart_cr1 = value;
            if (s->usart_cr1 & USART_CR1_RXNEIE &&
                s->usart_sr & USART_SR_RXNE) {
                qemu_set_irq(s->irq, 1);
            }
        return;
    case USART_CR2:
        s->usart_cr2 = value;
        return;
    case USART_CR3:
        s->usart_cr3 = value;
        return;
    case USART_GTPR:
        s->usart_gtpr = value;
        return;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
    }
}

static const MemoryRegionOps stm32_uart_ops = {
    .read = stm32_uart_read,
    .write = stm32_uart_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static Property stm32_uart_properties[] = {
    DEFINE_PROP_CHR("chardev", STM32UartState, chr),
    DEFINE_PROP_END_OF_LIST(),
};

static void stm32_uart_init(Object *obj)
{
    STM32UartState *s = STM32_UART(obj);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);

    memory_region_init_io(&s->mmio, obj, &stm32_uart_ops, s,
                          TYPE_STM32_UART, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void stm32_uart_realize(DeviceState *dev, Error **errp)
{
    STM32UartState *s = STM32_UART(dev);

    qemu_chr_fe_set_handlers(&s->chr, stm32_uart_can_receive,
                             stm32_uart_receive, NULL, NULL,
                             s, NULL, true);
}

static void stm32_uart_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32_uart_reset;
    device_class_set_props(dc, stm32_uart_properties);
    dc->realize = stm32_uart_realize;
}

static const TypeInfo stm32_uart_info = {
    .name          = TYPE_STM32_UART,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32UartState),
    .instance_init = stm32_uart_init,
    .class_init    = stm32_uart_class_init,
};

static void stm32_uart_register_types(void)
{
    type_register_static(&stm32_uart_info);
}

type_init(stm32_uart_register_types)
