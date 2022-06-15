
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
    	int * i = (int *) (src & (~3));

	// TODO: better address range check?
	if (src < 0x20000000 || src >= 0x60000000) {
		return;
	}

    int * dm = (int *) dest;
    *dm= *i >> ((src & 3) * 8);

	//return ap->dp->mem_read(ap, dest, src, len);
}

void adiv5_mem_write(ADIv5_AP_t *ap, uint32_t dest, const void *src, size_t len)
{
	//enum align align = MIN(ALIGNOF(dest), ALIGNOF(len));
	//adiv5_mem_write_sized(ap, dest, src, len, align);

  int * ip = (int *) (dest & (~3));
  int * sip = (int *) (dest & (~3));

  for(int i=0; i<len/4; i++) {
    ip[i] = sip[i];
  }

}

uint32_t adiv5_dp_error(ADIv5_DP_t *dp)
{
	return dp->error(dp);
}

