# artpi
Experience with the STM32H7, Uboot and the Linux kernel on the ART-Pi board 


# Why
The board was developed by the
https://www.rt-thread.org/

It has kernel and u-boot support. Here is how it works.

https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi/tree/master/projects/art_pi_kboot

To understand better the ry command, you must look in the art_pi_kboot project:
 Most significantly here, projects/art_pi_kboot/applications/main.c
 The command ry is added to MSH that uploads binaries with YMODEM.
 
I have modified the original project to be able to run without RT-Thread
and instead run as far as possible with qemu. The patches in the qemu folder relies on qemu 7.0

   > gdb u-boot -ex 'target remote:1234'

This is a great oportunity to learn more about arm, the stm32h7, uboot, Ghidra, qemu and arm Linux kernel without MMU support.



   "bootargs=console=ttySTM0,2000000 root=/dev/ram loglevel=8"

   Note from the device tree,
   aliases {
		serial0 = &uart4;
		serial1 = &usart3;
	};

So far with this qemu it hangs here common/board_f.c
     if (initcall_run_list(init_sequence_f)) 
       hang(); 

## Arm simulator

To fiddle with simple arm programs, 

https://cpulator.01xz.net/?sys=arm


## Precautions for the artpi

- Debug serial port to serial port 4 mapping instructions

    PI9 ------> USART4_RX

    PA0 ------> USART4_TX


   On the art-pi board uart4 is connected to the Stlink

## Some notes from running u-boot


When looking at the sources and reverse engineering the u-boot binary in
/projects/art_pi_kboot/demo
It seems like the Device tree blob should be located at,
   fdt_blob = 0x9004_0044

Other boot-time parameters,

   kernel_addr_r=0xC000_8000
   fdtfile=stm32h750i-art-pi.dtb
   fdt_addr_r=0xC0408000
   scriptaddr=0xC0418000
   pxefile_addr_r=0xC0428000
   ramdisk_addr_r=0xC0438000
   boot_scripts=boot.scr.uimg boot.scr

global_data is a structure used by u-boot.
   struct global_data *gd_ptr;
   r9 + 0x70


   (gdb) p *gd_ptr             
   $13 = {bd = 0x0, flags = 0, baudrate = 0, cpu_clk = 0, bus_clk = 0, pci_clk = 0, mem_clk = 0, have_console = 0,
   env_addr = 0,	env_valid = 0, env_has_init = 0, env_load_prio = 0, ram_base = 0, ram_top = 0, relocaddr = 0,
   ram_size = 0,	mon_len = 241896, irq_sp = 0, start_addr_sp = 0, reloc_off = 0,	new_gd = 0x0, dm_root = 0x0,
   dm_root_f = 0x0, uclass_root_s = {next = 0x0,	prev = 0x0}, uclass_root = 0x0,	timer = 0x0, 
   fdt_blob = 0x90040044, new_fdt = 0x0,	fdt_size = 0, fdt_src = FDTSRC_SEPARATE, jt = 0x0,
   env_buf = '\000' * 31 , timebase_h = 0, timebase_l = 0, malloc_base = 604238080,	malloc_limit = 0,
   malloc_ptr = 0, cur_serial_dev = 0x0, arch = {timer_rate_hz = 0, tbu = 0, tbl = 0, lastinc = 0,
    timer_reset_value = 0, tlb_addr = 0, tlb_size = 0}, dmtag_list = {next = 0x0, prev = 0x0}}

When using qemu we can load u-boot and kernel

(gdb) restore org-u-boot binary 0x90000000 0 275089
(gdb) restore art.itb binary 0x90080000 0 4822940
(gdb) restore u-boot-dtb.bin binary 0x90040044 0

(gdb) p/x *0x90000004
(gdb) s	$pc=0x900003fd




# Devices

8MiB QSPI flash (Boot flash)
16MiB SPI flash (W25Q64)              ORIGIN =0x90000000,LENGTH =8192k
32MiB SDRAM
AP6212 wifi,bt,fm comb


https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi


Most of the examples use the following adress mapping,
```

norflash0 | addr: 0x00000000 | len: 0x01000000 | blk_size: 0x00001000 |initialized finish.
norflash1 | addr: 0x00000000 | len: 0x00800000 | blk_size: 0x00001000 |initialized finish.

[I/FAL] ==================== FAL partition table ====================
[I/FAL] | name       | flash_dev |   offset   |    length  |
[I/FAL] -------------------------------------------------------------
[I/FAL] | wifi_image | norflash0 | 0x00000000 | 0x00080000 |
[I/FAL] | bt_image   | norflash0 | 0x00080000 | 0x00080000 |
[I/FAL] | download   | norflash0 | 0x00100000 | 0x00200000 |
[I/FAL] | easyflash  | norflash0 | 0x00300000 | 0x00100000 |
[I/FAL] | filesystem | norflash0 | 0x00400000 | 0x00a00000 |
[I/FAL] | factory    | norflash0 | 0x00e00000 | 0x00200000 |
[I/FAL] | app        | norflash1 | 0x00000000 | 0x00800000 |


Linker,
https://stackoverflow.com/questions/51323589/are-all-ram-sections-used-in-this-linkerscript-of-a-stm32h743-microcontroller-c

Internal memory,

 DTCMRAM:         128 KB 
 RAM_D1:          512 KB 
 RAM_D2:          288 KB 
 RAM_D3:          64 KB  
 ITCMRAM:         64 KB  


From this mapping

ROM (rx) : ORIGIN =0x90000000,LENGTH =8192k
RAM (rw) : ORIGIN =0x24000000,LENGTH =512k
RxDecripSection (rw) : ORIGIN =0x30040000,LENGTH =32k
TxDecripSection (rw) : ORIGIN =0x30040060,LENGTH =32k
RxArraySection (rw) : ORIGIN =0x30040200,LENGTH =32k
ROM (rx)    : ORIGIN =0x08000000,LENGTH =8192k

To this to run in qemu

QFLASH (rx) : ORIGIN =0x90000000,LENGTH =8192k
RAM (rw)    : ORIGIN =0x24000000,LENGTH =512k
ROM (rx) : ORIGIN =0x08000000,LENGTH =128k
RAM (rw) : ORIGIN =0x24000000,LENGTH =512k


   .section  .vectors
ENTRY(_start)
	.long	CONFIG_SYS_INIT_SP_ADDR		@ 0 - Reset stack pointer
	.long	reset				@ 1 - Reset
	.long	__invalid_entry			@ 2 - NMI
	.long	__hard_fault_entry		@ 3 - HardFault
	.long	__mm_fault_entry		@ 4 - MemManage
	.long	__bus_fault_entry		@ 5 - BusFault
	.long	__usage_fault_entry		@ 6 - UsageFault
	.long	__invalid_entry			@ 7 - Reserved
	.long	__invalid_entry			@ 8 - Reserved
	.long	__invalid_entry			@ 9 - Reserved
	.long	__invalid_entry			@ 10 - Reserved
	.long	__invalid_entry			@ 11 - SVCall
	.long	__invalid_entry			@ 12 - Debug Monitor
	.long	__invalid_entry			@ 13 - Reserved
	.long	__invalid_entry			@ 14 - PendSV
	.long	__invalid_entry			@ 15 - SysTick
	.rept	255 - 16
	.long	__invalid_entry			@ 16..255 - External Interrupts
	.endr


```



AP6212 is a low-power and high-performance WiFi+BT4.2 module launched by AMPAK. This module conforms to 802.11b/g/n. The WiFi function adopts SDIO interface, and the Bluetooth adopts UART/I2S/PCM interface, with StationMode, SoftAP, P2P function, etc. The hardware circuit connection of the chip is shown in the figure.
![WIFI-OTA](./figures/wifi_hardware.png)

(Broadcom BCM43438 A1 chip inside)  https://blog.quarkslab.com/reverse-engineering-broadcom-wireless-chipsets.html


# Linux for the st32m_h8
One of the few MMU-less arm boards supported in the kernel source tree

You can learn more about 
https://linux-kernel-labs.github.io/refs/heads/master/so2/lab11-arm-kernel-development.html


sw context:
- as stm32h750 just has 128k bytes internal flash, so running a fw on
  internal flash to download u-boot/kernel to qspi flash, boot
  u-boot/kernel from qspi flash. this fw is based on rt-thread.
- kernel can be xip on qspi flash or load to sdram
- root filesystem is jffs2(created by buildroot), stored on spi flash


to support the boad, add following changes.
- fix r0-r3, r12 register restore failed after svc call,
- add dts binding
- update yaml doc


## Build procedure

On an ubuntu LTS system

   apt install git make gcc device-tree-compiler bison flex libssl-dev libncurses-dev python3-ply python3-git libgmp3-dev libmpc-dev

   sudo apt-get install libc6-armel-cross gcc-arm-none-eabi libc6-dev-armel-cross binutils-arm-none-eabi libncurses5-dev build-essential bison flex libssl-dev bc

   sudo apt install gdb-multiarch

## U-boot

   git clone https://github.com/u-boot/u-boot



      * _main execution sequence is:
   *
   * 1. Set up initial environment for calling board_init_f().
   *    This environment only provides a stack and a place to store
   *    the GD ('global data') structure, both located in some readily
   *    available RAM (SRAM, locked cache...). In this context, VARIABLE
   *    global data, initialized or not (BSS), are UNAVAILABLE; only
   *    CONSTANT initialized data are available. GD should be zeroed
   *    before board_init_f() is called.
   *  Note that register r9 points to gd.
   *
   * 2. Call board_init_f(). This function prepares the hardware for
   *    execution from system RAM (DRAM, DDR...) As system RAM may not
   *    be available yet, , board_init_f() must use the current GD to
   *    store any data which must be passed on to later stages. These
   *    data include the relocation destination, the future stack, and
   *    the future GD location.
   *
   * 3. Set up intermediate environment where the stack and GD are the
   *    ones allocated by board_init_f() in system RAM, but BSS and
   *    initialized non-const data are still not available.
   *
   * 4a.For U-Boot proper (not SPL), call relocate_code(). This function
   *    relocates U-Boot from its current location into the relocation
   *    destination computed by board_init_f().
   *
   * 4b.For SPL, board_init_f() just returns (to crt0). There is no
   *    code relocation in SPL.
   *
   * 5. Set up final environment for calling board_init_r(). This
   *    environment has BSS (initialized to 0), initialized non-const
   *    data (initialized to their intended value), and stack in system
   *    RAM (for SPL moving the stack and GD into RAM is optional - see
   *    CONFIG_SPL_STACK_R). GD has retained values set by board_init_f().
   *
   * 6. For U-Boot proper (not SPL), some CPUs have some work left to do
   *    at this point regarding memory, so call c_runtime_cpu_setup.
   *
   * 7. Branch to board_init_r().
   *
   * For more information see 'Board Initialisation Flow in README.
   */



   ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make stm32h750-art-pi_defconfig

   ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make 


   ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M  -bios ../artpi/libraries/qemu/bootloader.bin -nographic  -s -S

   ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M -kernel u-boot  -nographic  -s -S

   gdb --args ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M  -bios  bootloader.bin -nographic  -s -S -kernel u-boot

### Hardfloat use this toolchain

$ sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf # for arm32
$ sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu     # for arm64

### Linaro toolchain differences
   https://wiki-archive.linaro.org/WorkingGroups/ToolChain/FAQ#What_is_the_differences_between_.2BIBw-arm-none-eabi-.2BIB0_and_.2BIBw-arm-linux-gnueabihf.2BIB0.3F_Can_I_use_.2BIBw-arm-linux-gnueabihf.2BIB0_tool_chain_in_bare-metal_environment.3F_How_do_you_know_which_toolchain_binary_to_use_where.3F


   wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz

### Optional is to install Linaro arm cross compiler
   Optional

   wget http://releases.linaro.org/components/toolchain/gcc-linaro/7.5-2019.12/gcc-linaro-7.5-2019.12.tar.xz
   sudo tar xf gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf.tar.xz -C /opt

   git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

   sudo tar xf gcc-linaro-7.5-2019.12.tar.xz -C /opt


### Get kernel and compile

   git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

Or get latest version 

   wget ttps://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tag/?h=v5.18-rc4


    
# compile the kernel


    cd linux
    export ARCH=arm
    export CROSS_COMPILE=arm-linux-gnueabi-

    make stm32_defconfig
      then
    make menuconfig
      Add debug configuration and remove MMU support

    make
    make dtbs

    ls arch/arm/boot/dts/stm32h750*

    ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make -j8
    ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make -j8
    file vmlinux
    vmlinux: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), statically linked, BuildID[sha1]=304e52462758033e520112825260e36c8589e18b, with debug_info, not stripped


# Run qemu

   You need qemu 7 with patches to emulate the artpi board

   qemu-img create -f raw sd.bin 64M
   if [ ! -f "sd.bin" ]; then
     dd if=/dev/zero of=sd.bin bs=1024 count=65536
   fi

   qemu-system-arm -M artpi -kernel rtthread.elf -serial stdio -sd sd.bin -S -s



   Get a minimal qemu-arm filesystem


   wget http://downloads.yoctoproject.org/releases/yocto/yocto-2.5/machines/qemu/qemuarm/core-image-minimal-qemuarm.ext4


    Start qemu 

    ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M -kernel arch/arm/boot/zImage -nographic  -dtb arch/arm/boot/dts/stm32h750i-art-pi.dtb  -append "root=/dev/mmcblk0 rw console=ttymxc0 loglevel=8 earlycon printk" -serial stdio -sd core-image-minimal-qemuarm.ext4 -S


    Start debugger in other window
    file vmlinux


    killall -9 qemu-system-arm
   
    gdb-multiarch vmlinux
    (gdb) set arch arm
    (gdb) target remote:1234



Note that qemu does not use the device tree to create all devices, it just passes the device tree to the kernel



### Kernel tree art-pi highlights

    Documentation/arm/stm32/stm32h750-overview.rst
    Documentation/arm/index.rst                        |   1 +
    Documentation/arm/stm32/stm32h750-overview.rst     |  33 ++
    .../devicetree/bindings/arm/stm32/stm32.yaml       |   4 +
    .../bindings/pinctrl/st,stm32-pinctrl.yaml         |   1 +
    arch/arm/boot/dts/Makefile                         |   1 +
    arch/arm/boot/dts/stm32h7-pinctrl.dtsi             | 392 +++++++++++++++++++++
    arch/arm/boot/dts/stm32h743-pinctrl.dtsi           | 307 +---------------
    arch/arm/boot/dts/stm32h743.dtsi                   |  30 ++
    arch/arm/boot/dts/stm32h750-pinctrl.dtsi           |  11 +
    arch/arm/boot/dts/stm32h750.dtsi                   |   5 +
    arch/arm/boot/dts/stm32h750i-art-pi.dts            | 228 ++++++++++++
    arch/arm/mach-stm32/board-dt.c                     |   1 +
    drivers/pinctrl/stm32/Kconfig                      |   2 +-
    drivers/pinctrl/stm32/pinctrl-stm32h743.c          |   3 +
    Documentation/arm/stm32/stm32h750-overview.rst
    arch/arm/boot/dts/stm32h7-pinctrl.dtsi
    arch/arm/boot/dts/stm32h750-pinctrl.dtsi
    arch/arm/boot/dts/stm32h750.dtsi
    arch/arm/boot/dts/stm32h750i-art-pi.dts


###  Other

https://medium.com/@jan_75582/setup-arm-web-application-development-environment-with-qemu-for-stm32-500f5650a489


https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt

I do suggest that you try to include your rootfs in the zImage because you do not need to point to the disk, just the init. qemu-system-arm -M versatileab -m 128M -kernel zImage -append "rdinit=/bin/sh" (I think, its been a couple of months) – 
look at the section "What is initramfs?" here for information, but note that I had to point my kernel to the _install directory from busybox NOT the cpio file I created as instructed here. This is more of a what is the difference between ramDisk and initramfs than a correct how to. Here is another one


### Arm kernel options
https://www.kernel.org/doc/html/latest/arm/index.html

# Links

https://balau82.wordpress.com/2010/04/12/booting-linux-with-u-boot-on-qemu-arm/

# Bare metal projects
https://github.com/chcbaram/ART-Pi

https://wiki.st.com/stm32mpu/wiki/Debugging_the_Linux_kernel_using_the_GDB


https://www.emcraft.com/stm32f769i-discovery-board/installing-uboot-to-stm32f7-discovery-board


ART-Pi setup guide.
https://www.programmersought.com/article/60736526194/

QSPI support
https://wiki.segger.com/STM32H7#STM32H753xx.2F50xx.2F43xx_QSPI_support


# Somewhat related

https://hackaday.com/2021/05/19/raspberry-pi-rp2040-hands-on-experiences-from-an-stm32-perspective/

Cross compile
https://jensd.be/1126/linux/cross-compiling-for-arm-or-aarch64-on-debian-or-ubuntu

Interesting crazyflie
https://lup.lub.lu.se/luur/download?func=downloadFile&recordOId=9052405&fileOId=9052409

Gfx lib
https://support.touchgfx.com/4.19/docs/introduction/welcome

More qemu
https://www.youtube.com/watch?v=CdH-cad8DSw

Trusted platfom mode with qemu
https://www.linaro.org/blog/how-to-emulate-trusted-platform-module-in-qemu-with-u-boot/

Qemu for stm32
https://medium.com/@jan_75582/setup-arm-web-application-development-environment-with-qemu-for-stm32-500f5650a489


# qemu options
Not all are needed here, but is saved for reference

    -smp cores=4 -m 1024 -device sdhci-pci -device sd-card,drive=mydrive -drive id=mydrive,if=none,format=raw,file=image.bin


    -drive file=flash_image.bin,if=mtd,format=raw

    -global driver=timer.esp32.timg,property=wdt_disable,value=true

    -drive file=sd_image.bin,if=sd,format=raw


# Jump to built in bootloader on h7

In STM32H7, the base address of system memory is different from the entry point of the bootloader. 

Thus, in order to jump to the bootloader, address "0x1FF09800" should be used instead of "0x1FFF0000". 

Below, you find a sample code permitting the jump to the bootloader in STM32H7 devices:

```
void JumpToBootloader(void)
{
  uint32_t i=0;
  void (*SysMemBootJump)(void);
 
  /* Set the address of the entry point to bootloader */
     volatile uint32_t BootAddr = 0x1FF09800;
 
  /* Disable all interrupts */
     __disable_irq();

  /* Disable Systick timer */
     SysTick->CTRL = 0;
	 
  /* Set the clock to the default state */
     HAL_RCC_DeInit();

  /* Clear Interrupt Enable Register & Interrupt Pending Register */
     for (i=0;i<5;i++)
     {
	  NVIC->ICER[i]=0xFFFFFFFF;
	  NVIC->ICPR[i]=0xFFFFFFFF;
     }	
	 
  /* Re-enable all interrupts */
     __enable_irq();
	
  /* Set up the jump to booloader address + 4 */
     SysMemBootJump = (void (*)(void)) (*((uint32_t *) ((BootAddr + 4))));
 
  /* Set the main stack pointer to the bootloader stack */
     __set_MSP(*(uint32_t *)BootAddr);
 
  /* Call the function to jump to bootloader location */
     SysMemBootJump(); 
  
  /* Jump is done successfully */
     while (1)
     {
      /* Code should never reach this loop */
     }
}

```


gdb-multiarch u-boot -ex 'target remote:1234'

(gdb) s $pc=0x90000f46+1

Possibility to run in qemu.
https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi/commit/244051251dddd944cccfa478a84e67e812cfbc6b

nvalid read at addr 0x9003EAE4, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F42, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F44, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F46, size 2, region '(null)', reason: rejected
Invalid read at addr 0x90000F48, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFA, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected


