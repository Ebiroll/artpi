/*
 * stm32 Platform DMA2D emulation
 *
 * Copyright (c) 2022 Olof Astrand.
 *
 * Author:
 *   Olof Astrand <olof.astrand@digisign.se>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 or
 * (at your option) version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STM32_DMA2D_H
#define STM32_DMA2D_H

struct stm32_dma2d_chan {
    uint32_t control;
    uint32_t next_config;
    uint64_t next_bytes;
    uint64_t next_dst;
    uint64_t next_src;
    uint32_t exec_config;
    uint64_t exec_bytes;
    uint64_t exec_dst;
    uint64_t exec_src;
    int state;
};

#define STM32_DMA2D_CHANS           1
#define STM32_DMA2D_IRQS            (STM32_DMA2D_CHANS * 2)
#define STM32_DMA2D_REG_SIZE        0x100000
#define STM32_DMA2D_CHAN_NO(reg)    ((reg & (STM32_DMA2D_REG_SIZE - 1)) >> 12)

typedef struct Stm32DMA2DState {
    SysBusDevice parent;
    MemoryRegion iomem;
    qemu_irq irq[STM32_DMA2D_IRQS];

    struct stm32_dma2d_chan chan[STM32_DMA2D_CHANS];
} Stm32DMA2DState;

#define TYPE_STM32_DMA2D    "stm32.dma2d"

#define STM32_DMA2D(obj)    \
    OBJECT_CHECK(Stm32DMA2DState, (obj), TYPE_STM32_DMA2D)

#endif /* STM32_DMA2D_H */
