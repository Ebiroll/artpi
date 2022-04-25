# artpi
Experience with the STM32H7, Uboot and the Linux kernel on the ART-Pi board 



# Why
The board was developed by the
https://www.rt-thread.org/

It has kernel and u-boot support. Here is how to build.

https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi/tree/master/projects/art_pi_kboot


# Devices

8MiB QSPI flash
16MiB SPI flash
32MiB SDRAM
AP6212 wifi,bt,fm comb


https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi



# Linux
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

   sudo apt-get install libc6-armel-cross gcc-arm-linux-gnueabi libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev build-essential bison flex libssl-dev bc

   sudo apt install gdb-multiarch

## U-boot

   git clone https://github.com/u-boot/u-boot

   ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make stm32h750-art-pi_defconfig

   ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make 


### N/A
$ sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf # for arm32
$ sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu     # for arm64


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

   Get a minimal qemu-arm filesystem

   wget http://downloads.yoctoproject.org/releases/yocto/yocto-2.5/machines/qemu/qemuarm/core-image-minimal-qemuarm.ext4


    Start qemu 

    ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M netduinoplus2 -cpu cortex-m4 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M -kernel arch/arm/boot/zImage -nographic  -dtb arch/arm/boot/dts/stm32h750i-art-pi.dtb  -append "root=/dev/mmcblk0 rw console=ttymxc0 loglevel=8 earlycon printk" -sd core-image-minimal-qemuarm.ext4 -S


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