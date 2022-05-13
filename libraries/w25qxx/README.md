## W25QXX SPI FLASH Library for STM32
* http://www.github.com/NimaLTD   
* https://www.instagram.com/github.nimaltd/   
* https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw   

* Enable SPI and a Gpio as output(CS pin).Connect WP and HOLD to VCC.
* Select software CS pin.
* Config `w25qxxConf.h`.
* Call `W25qxx_Init()`. 
* After init, you can watch `w25qxx` struct.(Chip ID,page size,sector size and ...)
* In Read/Write Function, you can put 0 to `NumByteToRead/NumByteToWrite` parameter to maximum.
* Dont forget to erase page/sector/block before write.



    At present, the following commands are mainly used ：

    #define WRITE_ENABLE_CMD      0x06         /*  Write enabling instructions  */  
    #define READ_ID_CMD2          0x90         /*  Read ID command  */  
    #define READ_ID_CMD2          0x9F         /*  Read ID 2 command  */  
    #define READ_STATUS_REG_CMD   0x05         /*  Read status command  */ 
    #define BULK_ERASE_CMD        0xC7         /*  The entire chip erase command  */ 
    #define SUBSECTOR_ERASE_4_BYTE_ADDR_CMD      0x21    /* 32bit Address sector erase instruction , 4KB */
    #define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x34    /* 32bit Address of the 4 Line quick write command  */
    #define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xEC    /* 32bit Address of the 4 Line q
