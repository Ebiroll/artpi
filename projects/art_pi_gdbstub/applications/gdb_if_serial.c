// Copyright 2022 Olof Åstrand
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
 * Description: interface to gdb over serial
 *******************************************************************************/

#include "gdb_if.h"
#include "stm32h7xx_hal.h"

#define USE_SERIAL 1

extern UART_HandleTypeDef huart4;


//Length of buffer used to reserve GDB commands. Has to be at least able to fit the G command, which
//implies a minimum size of about 320 bytes.
#define PBUFLEN 512
static char chsum;						//Running checksum of the output packet

#define ATTR_GDBFN


#define IN_BUFFER_LEN 512
static int in_buf_received = 0;

uint8_t gdb_buffer[IN_BUFFER_LEN];
int read_pos;
int read_len;



#if defined(USE_SERIAL) 


int ATTR_GDBFN gdbRecvChar() {
	HAL_StatusTypeDef ret=HAL_UART_Receive (&huart4, gdb_buffer, 1, 1000);
	if (ret==HAL_OK) {
		//read_pos++;
		return gdb_buffer[0];
	} else {
		return -1;
	}
}

//Send a char to the uart.
void ATTR_GDBFN gdbSendChar(char c) {
	HAL_UART_Transmit (&huart4, &c, 1, 0);
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
	return gdbRecvChar();
}


unsigned char gdb_if_getchar_to(int timeout) {
	/*
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
	*/

}



void gdb_if_putchar(unsigned char c, int flush)
{
    gdbSendChar(c);
}

// See also
//gdbSendChar(c);
//gdbPacketChar(c);

//Send a hex val as part of a packet. 'bits'/4 dictates the number of hex chars sent.
void ATTR_GDBFN gdbPacketHex(int val, int bits) {
	char hexChars[]="0123456789abcdef";
	int i;
	for (i=bits; i>0; i-=4) {
		gdbPacketChar(hexChars[(val>>(i-4))&0xf]);
	}
}

void gdbPacketFlush() {
    //int  nwrote;
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

void gdb_if_close(void) {
}

#endif