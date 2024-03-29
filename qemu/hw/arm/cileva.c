/*
 * ART-PI Machine Model
 *
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

#include "qemu/osdep.h"
#include "qemu/datadir.h"
#include "qapi/error.h"
#include "hw/boards.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-clock.h"
#include "qemu/error-report.h"
#include "hw/arm/stm32h7a3vih_soc.h"
#include "qemu/module.h"
#include "hw/arm/boot.h"
#include "hw/core/generic-loader.h"
#include "target/arm/cpu.h"
#include "qom/object.h"
#include "hw/loader.h"


/* Main SYSCLK frequency in Hz (168MHz) */
#define SYSCLK_FRQ 240000000ULL

static int get_my_filesize(const char *filename)
{
    FILE *fp;
    int size;

    fp = fopen(filename, "rb");
    if (!fp) {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);
    return size;
}


static void cileva_init(MachineState *machine)
{
    DeviceState *dev;
    Clock *sysclk;
    AddressSpace *as;

    /* This clock doesn't need migration because it is fixed-frequency */
    sysclk = clock_new(OBJECT(machine), "SYSCLK");
    clock_set_hz(sysclk, SYSCLK_FRQ);

    dev = qdev_new(TYPE_STM32H7A3VIH_SOC);
    qdev_prop_set_string(dev, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m7"));
    qdev_connect_clock_in(dev, "sysclk", sysclk);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);


{
    STM32H7A3VIHState *s = STM32H7A3VIH_SOC(dev);
    CPUState *cs = CPU(&s->armv7m);
    int asidx;
    //ARMCPU *cpu = ARMCPU(&s->armv7m);

    armv7m_load_kernel(ARM_CPU(first_cpu),
                       machine->kernel_filename,
                       FLASH_SIZE);



   // if (arm_feature(&cs->env, ARM_FEATURE_EL3)) {
   //     asidx = ARMASIdx_S;
   // } else {
   //     asidx = ARMASIdx_NS;
   // }
   // Non secure
    //asidx = ARMASIdx_NS;
    //as = cpu_get_address_space(cs, asidx);

// FLASH_BASE_ADDRESS

#if 0

        char *rom_binary = qemu_find_file(QEMU_FILE_TYPE_BIOS, "bootloader.bin");
        if (rom_binary == NULL) {
            error_report("Error: -bios argument not set, and ROM bootloader not found (1)");
            exit(1);
        }

        int file_size=get_my_filesize(rom_binary);

//        cpu_physical_memory_write(0x08000000, rom_binary, 83576);

        int size = load_image_targphys_as(rom_binary, 0x08000000, file_size, CPU(&s->armv7m)->as);
        if (size < 0) {
            error_report("Error: could not load bootloader.bin binary '%s'", rom_binary);
            exit(1);
        }

        g_free(rom_binary);
#endif

}






    //rom_check_and_register_reset();
    //qemu_devices_reset();
}

static void cileva_machine_init(MachineClass *mc)
{
    mc->desc = "Portenta stm32h7a3 Machine (Cortex-M7)";
    mc->init = cileva_init;
}

DEFINE_MACHINE("h7a3", cileva_machine_init)
