// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/******************************************************************************
 * Description: interface to gdb over wifi
 *******************************************************************************/

//#include "esp_gdbstub.h"
#include <string.h>
#include "gdb_if.h"
#include <stdint.h>
#include "usbd_cdc_if.h"

#define ATTR_GDBFN __attribute__((noinline))
static char chsum;						//Running checksum of the output packet


// Buffer to communicate with GDB
uint8_t gdb_buffer[512];
#define SEND_BUFF_LEN 256
uint8_t send_buffer[SEND_BUFF_LEN];

int read_pos=0;
int read_len=0;

int send_len=0;

int write_pos=0;

int ATTR_GDBFN gdbRecvChar() {
	int i;
    if (read_pos<read_len) {
        i = gdb_buffer[read_pos];
        read_pos++;
        return i;
    } else {
        return-1;
    }
	return i;
}

//Send a char to the uart.
void ATTR_GDBFN gdbSendChar(char c) {
    if (send_len<SEND_BUFF_LEN) {
        send_buffer[send_len] = c;
        send_len++;
    } else
    {
        CDC_Transmit_FS(send_buffer, send_len);
        send_len=0;
    }
}




//Send the start of a packet; reset checksum calculation.
void ATTR_GDBFN gdbPacketStart() {
	chsum=0;
	gdbSendChar('$');
}

//Send a char as part of a packet
void ATTR_GDBFN gdbPacketChar(char c) {
	if (c=='#' || c=='$' || c=='}' || c=='*') {
		gdbSendChar('}');
		gdbSendChar(c^0x20);
		chsum+=(c^0x20)+'}';
	} else {
		gdbSendChar(c);
		chsum+=c;
	}
}

//Send a string as part of a packet
void ATTR_GDBFN gdbPacketStr(char *c) {
	while (*c!=0) {
		gdbPacketChar(*c);
		c++;
	}
}

int gdb_if_init(void) 
{
	return 0;
}
unsigned char gdb_if_getchar(void) {
    int ret=-1;
    while(ret<0) {
        ret=gdbRecvChar();
        if (ret>0) {return ret;};
        platform_delay(10);
    }
    return 0;
}


extern uint32_t platform_time_ms(void);

unsigned char gdb_if_getchar_to(int timeout) {
    unsigned char ret=0;
    uint32_t now;
    uint32_t start=platform_time_ms();
    do {
        int val=gdbRecvChar();
        if (val!=-1) {
            ret=val;
            break;
        }
        now=platform_time_ms();
    } while((now-start)<timeout);
	return ret;
}



void gdb_if_putchar(unsigned char c, int flush)
{
    gdbSendChar(c);
    if (flush) {
        CDC_Transmit_FS(send_buffer, send_len);
        send_len=0;
    }
}


//Send a hex val as part of a packet. 'bits'/4 dictates the number of hex chars sent.
void ATTR_GDBFN gdbPacketHex(int val, int bits) {
	char hexChars[]="0123456789abcdef";
	int i;
	for (i=bits; i>0; i-=4) {
		gdbPacketChar(hexChars[(val>>(i-4))&0xf]);
	}
}

void gdbPacketFlush() {
    CDC_Transmit_FS(send_buffer, send_len);
    send_len=0;
}

//Finish sending a packet.
void ATTR_GDBFN gdbPacketEnd() {
	gdbSendChar('#');
	gdbPacketHex(chsum, 8);
	gdbPacketFlush();
}

int gdb_if_is_running(void) {
		return 1;
}





