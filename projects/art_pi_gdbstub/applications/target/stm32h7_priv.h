/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2015 Gareth McMullin <gareth@blacksphere.co.nz>
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
#ifndef __STM32H7PRIV_H
#define __STM32H7PRIV_H

#include "target.h"
//#include "adiv5.h"

typedef struct ADIv5_AP_s ADIv5_AP_t;

typedef struct ADIv5_DP_s {
	int refcnt;

	uint32_t idcode;
	uint32_t targetid;  /* Contains IDCODE for DPv2 devices.*/

	void (*seq_out)(uint32_t MS, int ticks);
	void (*seq_out_parity)(uint32_t MS, int ticks);
	uint32_t (*seq_in)(int ticks);
	bool (*seq_in_parity)(uint32_t *ret, int ticks);
	/* dp_low_write returns true if no OK resonse, but ignores errors */
	bool (*dp_low_write)(struct ADIv5_DP_s *dp, uint16_t addr,
						 const uint32_t data);
	uint32_t (*dp_read)(struct ADIv5_DP_s *dp, uint16_t addr);
	uint32_t (*error)(struct ADIv5_DP_s *dp);
	uint32_t (*low_access)(struct ADIv5_DP_s *dp, uint8_t RnW,
                               uint16_t addr, uint32_t value);
	void (*abort)(struct ADIv5_DP_s *dp, uint32_t abort);

	uint32_t (*ap_read)(ADIv5_AP_t *ap, uint16_t addr);
	void (*ap_write)(ADIv5_AP_t *ap, uint16_t addr, uint32_t value);

	void (*mem_read)(ADIv5_AP_t *ap, void *dest, uint32_t src, size_t len);
	void (*mem_write_sized)(ADIv5_AP_t *ap, uint32_t dest, const void *src,
							size_t len, enum align align);
	uint8_t dp_jd_index;
	uint8_t fault;
} ADIv5_DP_t;

struct ADIv5_AP_s {
	int refcnt;

	ADIv5_DP_t *dp;
	uint8_t apsel;

	bool stepping;
	bool on_bkpt;
	bool has_cache;

	uint32_t idr;
	uint32_t base;
	uint32_t csw;
	uint32_t ap_cortexm_demcr; /* Copy of demcr when starting */
	uint32_t ap_storage; /* E.g to hold STM32F7 initial DBGMCU_CR value.*/
	uint16_t ap_designer;
	uint16_t ap_partno;
};

#endif

