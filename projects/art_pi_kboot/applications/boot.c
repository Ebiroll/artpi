/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-25     armink       the first version
 */

//#include <rthw.h>
//#include <rtthread.h>
#include "w25qxx.h"


void * memset(void * blk, int c, size_t n)
{
    unsigned char * dst = blk;

    while (n-- > 0)
        *dst++ = (unsigned char)c;

    return blk;
}

void rt_hw_board_init(void);
int my_startup(void);

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    rtthread_startup();
    return 0;
}
#elif defined(__ICCARM__)
extern int main(void);
/* __low_level_init will auto called by IAR cstartup */
extern void __iar_data_init3(void);
int __low_level_init(void)
{
    // call IAR table copy function.
    __iar_data_init3();
    rtthread_startup();
    return 0;
}
#elif defined(__GNUC__)
/* Add -eentry to arm-none-eabi-gcc argument */
int entry(void)
{
    my_startup();
    return 0;
}
#endif



int my_startup(void)
{
    rt_hw_interrupt_disable();

    /* board level initialization
     * NOTE: please initialize heap inside board initialization.
     */
    rt_hw_board_init();

    MX_QUADSPI_Init();

    W25QXX_ExitQPIMode();
    W25QXX_Reset();
    
    main();
    /* never reach here */
    return 0;
}
