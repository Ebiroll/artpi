
#include <stdint.h>
#include <stddef.h> 
#include <stdbool.h>
#include "target/stm32h7_priv.h"

void adiv5_ap_ref(ADIv5_AP_t *ap)
{
	if (ap->refcnt == 0)
		ap->dp->refcnt++;
	ap->refcnt++;
}

static void adiv5_dp_unref(ADIv5_DP_t *dp)
{
	if (--(dp->refcnt) == 0)
		free(dp);
}

void adiv5_ap_unref(ADIv5_AP_t *ap)
{
	if (--(ap->refcnt) == 0) {
		adiv5_dp_unref(ap->dp);
		free(ap);
	}
}


void adiv5_ap_write(ADIv5_AP_t *ap, uint16_t addr, uint32_t value)
{
	//return ap->dp->ap_write(ap, addr, value);
}

void adiv5_mem_read(ADIv5_AP_t *ap, void *dest, uint32_t src,
								  size_t len)
{
	//return ap->dp->mem_read(ap, dest, src, len);
}

void adiv5_mem_write(ADIv5_AP_t *ap, uint32_t dest, const void *src, size_t len)
{
	//enum align align = MIN(ALIGNOF(dest), ALIGNOF(len));
	//adiv5_mem_write_sized(ap, dest, src, len, align);
}

uint32_t adiv5_dp_error(ADIv5_DP_t *dp)
{
	return dp->error(dp);
}

