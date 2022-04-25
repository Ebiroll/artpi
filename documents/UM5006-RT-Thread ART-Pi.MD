
# UM5006-RT-Thread ART-Pi data flash manual

SPI1??
W25Q64JV



## ART-Pi Data FLASH(W25Q128JV) Introduction

In addition to a W25Q64JV for storing programs, ART-Pi is also equipped with another W25Q128JV for storing WIFI firmware, Bluetooth firmware, and other data. The official recommendation is that the space of this W25Q128JV is divided as follows:

| Partition Name | Start Address | Partition Size | Purpose Description |
| ---------- | --------------- | -------- | ------------- ------ |
| wifi_image | 0               | 512KB    | save wifi firmware    |
| bt_image   | 512 * 1024      | 512KB    | save bt firmware |
| download   | 1 * 1024 * 1024 | 2048KB   | Firmware download partition |
| easyflash  | 3 * 1024 * 1024 | 1024KB   | easyflash parameter storage area |
| filesystem | 4 * 1024 * 1024 | 12MB     | file system partition |


ART-Pi has already flashed the corresponding data information in the data flash when it leaves the factory, but sometimes we may accidentally destroy or erase the above data, and then we need to rewrite the data. STM32CubeProgrammer loading and downloading algorithm can easily read and write data flash.

## STM32CubeProgrammer download algorithm loading

Copy the file ART-Pi SDK-->debug-->stldr-->ART-Pi_W25Q128JV.stldr to the external flash algorithm directory of STM32CubeProgrammer (eg: C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader)

![cubeprog](./figures/cubeprog.gif)

![cubeprog2](./figures/cubeprog2.gif)

## STM32CubeProgrammer erase and write data flash

This download algorithm can erase the whole block of flash at one time, or it can be erased separately according to the 64KB block size.

## STM32CubeProgrammer erase and write data flash

This download algorithm can erase the whole block of flash at one time, or it can be erased separately according to the 64KB block size.

![cubeprog3](./figures/cubeprog3.gif)

The following is an example of flashing the wifi firmware for demonstration (remember that the starting address of the wifi firmware for the flash partition division is 0, and the address of the data flash in the STM32CubeProgrammer is 0x70000000, so 0x70000000+0 (partition offset) is what we want to write. Start address), STM32CubeProgrammer can only open files with suffixes such as bin, so change the suffix of the wifi firmware file name to bin.

![cubeprog4](./figures/cubeprog4.gif)
