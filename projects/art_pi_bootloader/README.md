#BootLoader routine

## Introduction

The main function of this routine is to make the program jump from `0x08000000` to `0x90000000`.
The size of the on-chip ROM of STM32H750 is 128K, which cannot meet the general program development, and must use XIP to run the program. So you need to use BootLoader to jump to the XIP(0x90000000) address.
This routine can also serve as a base project for you to develop more advanced bootloader functions.

## Hardware description

<img src="./figures\qspi_flash.png" alt="qspi_flash" style="zoom:50%;" />

As shown in the figure above, QSPI_FLASH is connected to the QSPI peripheral pin of the microcontroller.

## Software Description

The source code is located in `/projects/art_pi_bootloader/applications/main.c`.

In the main function, the jump is implemented.

```
int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    W25QXX_Init();

    W25Q_Memory_Mapped_Enable();

    SCB_DisableICache();
    SCB_DisableDCache();

    SysTick->CTRL = 0;

    JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    JumpToApplication();

    return RT_EOK;
}
```


## run
### Compile & Download

After compiling, connect the ST-Link USB port of the development board to the PC, and then download the firmware to the development board.

### running result

After power-on, the bootloader program will be executed, and then it will jump to execute the executable program in QSPI FLASH

## Precautions

1. If there is no executable program in the QSPI FLASH, the MCU will stop running after executing the jump instruction.