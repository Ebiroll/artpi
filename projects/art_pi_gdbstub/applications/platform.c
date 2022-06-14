/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "general.h"
#include "gdb_if.h"
//#include "version.h"

#include "gdb_packet.h"
#include "gdb_main.h"
#include "target.h"
//#include "exception.h"
#include "gdb_packet.h"
//#include "morse.h"
#include "stm32h7xx_hal.h"


void platform_init(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	/*
	gpio_set_iomux_function(3, IOMUX_GPIO3_FUNC_GPIO);
	gpio_set_iomux_function(2, IOMUX_GPIO2_FUNC_GPIO);

	gpio_clear(_, SWCLK_PIN);
	gpio_clear(_, SWDIO_PIN);

	gpio_enable(SWCLK_PIN, GPIO_OUTPUT);
	gpio_enable(SWDIO_PIN, GPIO_OUTPUT);
	*/

	gdb_if_init();
}

void platform_srst_set_val(bool assert)
{
	(void)assert;
}

bool platform_srst_get_val(void) { return false; }

const char *platform_target_voltage(void)
{
	return "not supported";
}

uint32_t platform_time_ms(void)
{
	int64_t time_milli=HAL_GetTick();
	return((uint32_t)time_milli);
}

//#define vTaskDelayMs(ms)	vTaskDelay((ms)/portTICK_RATE_MS)

void platform_delay(uint32_t ms)
{
    uint32_t tickstart;

    tickstart = HAL_GetTick();

	while (((HAL_GetTick() - tickstart) < ms && (ms != 0U)))
	{
	 __ASM volatile ("nop");
	 //__asm("nop");
	}

	//vTaskDelayMs(ms);
}

int platform_hwversion(void)
{
	return 0;
}

/* This is a transplanted main() from main.c */
void main_task(void *parameters)
{
	(void) parameters;

	platform_init(0, NULL);

	while (true) {

               //volatile struct exception e;
		       //TRY_CATCH(e, EXCEPTION_ALL) {
		       gdb_main();
		       //}
		       //if (e.type) {
		       //gdb_putpacketz("EFF");
		       //target_list_free();
		       //morse("TARGET LOST.", 1);
		       //}
	}

	/* Should never get here */
}

void user_init(void)
{
#if 0
	uart_set_baud(0, 460800);
	printf("SDK version:%s\n", sdk_system_get_sdk_version());

#endif
//	xTaskCreate(&main_task, (const char *)"main", 4*256, NULL, 2, NULL);
}
