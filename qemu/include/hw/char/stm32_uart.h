/*
 * STM32 UART
 *
 * Copyright (c) 2022 Olof Åstrand <olof.astrand@gmail.com>
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

#ifndef HW_STM32_UART_H
#define HW_STM32_UART_H

#include "hw/sysbus.h"
#include "chardev/char-fe.h"
#include "qom/object.h"


#define USART_CR1  0x00
#define USART_CR2  0x04
#define USART_CR3  0x08
#define USART_BRR  0x0c
#define USART_GTPR 0x10

#define USART_RTOR 0x14
#define USART_RQR  0x18
#define USART_ISR  0x1C
#define USART_ICR  0x20
#define USART_RDR  0x24
#define USART_TDR  0x28
#define USART_PRESC  0x2C

#define USART_SR   0x00


/*
 * NB: The reset value mentioned in "24.6.1 Status register" seems bogus.
 * Looking at "Table 98 USART register map and reset values", it seems it
 * should be 0xc0, and that's how real hardware behaves.
 */
#define USART_SR_RESET (USART_SR_TXE | USART_SR_TC)

#define USART_SR_TXE  (1 << 7)
#define USART_SR_TC   (1 << 6)
#define USART_SR_RXNE (1 << 5)

#define USART_ISR_RXNE_RXFNE_Msk        (0x1UL << USART_ISR_RXNE_RXFNE_Pos)    /*!< 0x00000020 */
#define USART_ISR_RXNE_RXFNE            USART_ISR_RXNE_RXFNE_Msk               /*!< Read Data Register or RX FIFO Not Empty */
#define USART_ISR_TC_Pos                (6U)



#define USART_CR1_RXNEIE  (1 << 5)
#define USART_CR1_TE  (1 << 3)

// USART enable
#define USART_CR1_UE  (1 << 0)
#define USART_CR1_RE  (1 << 2)

// 0x00000004

#define TYPE_STM32_UART "stm32-uart"
OBJECT_DECLARE_SIMPLE_TYPE(STM32UartState, STM32_UART)

struct STM32UartState {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t usart_sr;
    uint32_t usart_dr;
    uint32_t usart_brr;
    uint32_t usart_cr1;
    uint32_t usart_cr2;
    uint32_t usart_cr3;
    uint32_t usart_gtpr;

    CharBackend chr;
    qemu_irq irq;
};
#endif /* HW_STM32_UART_H */
