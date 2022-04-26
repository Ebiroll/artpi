# ART-Pi-QBoot routine

## Introduction

The main function of this routine is to make the program jump from `0x08000000` to `0x90000000`.

The size of the on-chip ROM of STM32H750 is 128K, which cannot meet the general program development, and must use XIP to run the program. So you need to use BootLoader to jump to the XIP(0x90000000) address.
This routine is implemented based on **package** [Quick bootloader](https://github.com/qiyongzhong0/rt-thread-qboot).

## Hardware description

<img src="./figures\qspi_flash.png" alt="qspi_flash" style="zoom:50%;" />

**QSPI_FLASH** is connected to the QSPI peripheral pin of the microcontroller to execute the user logic code (APP). After the memory mapping is started, the QSPI FLASH will become unwriteable. If you want to achieve program upgrade, you need to The write data to QSPI FLASH is completed before the `boot` phase does not enable memory mapping mode.

<img src="./figures\spi_flash.png" alt="spi_flash" style="zoom:50%;" />

**SPI_FLASH** is connected to the SPI peripheral pins of the microcontroller, it is used to save the user upgrade code and restore the factory code, and write the upgrade firmware conforming to the `RBL` format into the `download of **SPI_FLASH** during the APP execution process ` partition, the firmware upgrade can be completed after restarting.

## Software Description

The main work of this routine is implemented by the software package [Quick bootloader](https://github.com/qiyongzhong0/rt-thread-qboot).

However, the `qboot_stm32.c` file has been modified to support jumping to the execution address `0x90000000` of **QSPI_FLASH**.

The following is the incomplete code:

```
#define QBOOT_APP_RUN_IN_QSPI_FLASH

#ifdef QBOOT_APP_RUN_IN_QSPI_FLASH

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x90000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;

    SCB_DisableICache();
    SCB_DisableDCache();

    SysTick->CTRL = 0;
    __disable_irq();
    __set_CONTROL(0);
    __set_MSP(stk_addr);
    
    app_func = (app_func_t) (*(__IO uint32_t*) (QSPI_BASE + 4));
    app_func();

    LOG_E("Qboot jump to application fail.");
```
## run
### Compile & Download

After compiling, connect the ST-Link USB port of the development board to the PC, and then download the firmware to the development board.

### running result
```
   ___  ______ _____     ______ _   ___________  _____  _____ _____ 
  / _ \ | ___ \_   _|    | ___ (_) |  _  | ___ \|  _  ||  _  |_   _|
 / /_\ \| |_/ / | |______| |_/ /_  | | | | |_/ /| | | || | | | | |  
 |  _  ||    /  | |______|  __/| | | | | | ___ \| | | || | | | | |   
 | | | || |\ \  | |      | |   | | \ \/' / |_/ /\ \_/ /\ \_/ / | |   
 \_| |_/\_| \_| \_/      \_|   |_|  \_/\_\____/  \___/  \___/  \_/  

 Powered by RT-Thread.

[D/FAL] (fal_flash_init:63) Flash device |                norflash0 | addr: 0x00000000 | len: 0x01000000 | blk_size: 0x00001000 |initialized finish.
[D/FAL] (fal_flash_init:63) Flash device |                norflash1 | addr: 0x00000000 | len: 0x00800000 | blk_size: 0x00001000 |initialized finish.
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
[I/FAL] =============================================================
[I/FAL] RT-Thread Flash Abstraction Layer (V0.5.0) initialize success.

Qboot startup ... 
Qboot version   : V1.0.5 2020.10.05 
Product name    : ART-Pi QBoot 
Product version : v1.00 2021.02.07 
Product mcu     : stm32h750 
Press [Enter] key into shell in 3 s : 

```
You can see that `norflash0` and `norflash1` are managed by [fal](https://github.com/RT-Thread-packages/fal). Read and write operations during the upgrade process are done by `fal`.

## use
### How to generate upgrade firmware

You need to obtain the `bin` format file of the upgrade firmware first, and then use the `QBootPackager_V1.02` tool in the `art_pi_qboot\packages\qboot-v1.05\tools` directory to package it.

![qboot](./figures/qboot.png)

### How to download the latest firmware

To upgrade using `YMODEM`, you need to use the `ymodem_ota` command to download in the `boot` stage.

![update](./figures/qboot.gif)

## Precautions

1. `RT_NAME_MAX` must be greater than **8**

2. Only tested in **RTT-Studio** environment. Compile error in **MDK** environment.

3. The functions of [Quick bootloader](https://github.com/qiyongzhong0/rt-thread-qboot) are not fully displayed. After all the functions are displayed, the `ROM` space will exceed `128K`. You can cut it yourself, or adjust the optimization level.

4. If you encounter problems during use, you can ask questions in [RT-Thread Forum]() [Invitation to Answer](https://club.rt-thread.org/u/8644).

## contact information

Maintenance by:

-  [whj4674672](https://github.com/whj4674672), 邮箱：<whj4674672@163.com>

