/*
 * This heavily hacked, file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2017-2020 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de
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

/* This file implements STM32H7 target specific functions for detecting
 * the device, providing the XML memory map and Flash memory programming.
 *
 * Refereces:
 * ST doc - RM0433
 *   Reference manual - STM32H7x3 advanced ARM®-based 32-bit MCUs Rev.3
 */

/*
 * While the ST document (RM 0433) claims that the stm32h750 only has 1 bank
 * with 1 sector (128k) of user main memory flash (pages 151-152), we were able
 * to write and successfully verify into other regions in bank 1 and also into
 * bank 2 (0x0810 0000 as indicated for the other chips).
 */

#include "general.h"
#include "target.h"
#include "target_internal.h"
#include "cortexm.h"
#include "stm32h7_priv.h"
#include "stm32h7xx_hal.h"
#include "gdb_main.h"
#include "w25qxx.h"

///////////////////////////////////////////////////////////

#define FP_CTRL  (*(uint32_t*)0xE0002000)
#define FP_REMAP (*(uint32_t*)0xE0002004)
#define FP_COMP(n) (((uint32_t*)0xE0002008)[n])
#define FP_COMP0 (*(uint32_t*)0xE0002008)
#define FP_COMP1 (*(uint32_t*)0xE000200C)
#define FP_COMP2 (*(uint32_t*)0xE0002010)
#define FP_COMP3 (*(uint32_t*)0xE0002014)
#define FP_COMP4 (*(uint32_t*)0xE0002018)
#define FP_COMP5 (*(uint32_t*)0xE000201C)
#define FP_COMP6 (*(uint32_t*)0xE0002020)
#define FP_COMP7 (*(uint32_t*)0xE0002024)
#define FP_COMP_MASK  0x1FFFFFFC
#define FP_REMAP_MASK 0x1FFFFFF0
#define FP_REMAP_RMPSPT (1<<29)

#define FP_LAR_UNLOCK_KEY 0xC5ACCE55
#define FP_LAR   (*(unsigned int*) 0xE0000FB0)
#define FP_LSR   (*(unsigned int*) 0xE0000FB4)


///////////////////////////// Teensy debug functions ///////////////////////////

#define IRQ_SOFTWARE	10

// breakpoint handler pointer
void (*callback)() = NULL;

// Counter for debugging; counts number of breakpoint calls
int debugcount = 0;

// Debug system is enabled?
int debugenabled = 0;

// Restore registers before returning?
int debugrestore = 0;

// Pretty names for breakpoint and fault types
const char *hard_fault_debug_text[] = {
  "debug", "break", "nmi", "hard", "mem", "bus", "usage"
};

// The interrupt call
// 0 = breakpoint
// 1 = nmi
// 2 = hard fault, etc.
uint32_t debug_id = 0;

// Debug tracing - not used by code
int debug_trace = 0;

// Copy of the registers at breakpoint
struct save_registers_struct {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t xPSR;

  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t sp;
} save_registers;

GdbRegFile gdb_regfile;

// Structure of ISR stack
struct stack_isr {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t xPSR;
};
// Live pointer to stack of ISR. We use this to modify the
// return address and other things
struct stack_isr *stack;

/**
 * @brief Macros to save/restore registers from stack
 * 
 */
#define SAVE_STACK \
    "ldr r0, =stack \n" \
    "str sp, [r0] \n"

// Save registers within an interrupt. Changes R0 register
#define SAVE_REGISTERS \
    "ldr r0, =stack \n" \
    "ldr r1, [r0] \n " \
    "ldr r0, =save_registers \n" \
    "ldr r2, [r1, #0] \n" \
    "str r2, [r0, #0] \n" \
    "ldr r2, [r1, #4] \n" \
    "str r2, [r0, #4] \n" \
    "ldr r2, [r1, #8] \n" \
    "str r2, [r0, #8] \n" \
    "ldr r2, [r1, #12] \n" \
    "str r2, [r0, #12] \n" \
    "ldr r2, [r1, #16] \n" \
    "str r2, [r0, #16] \n" \
    \
    "ldr r2, [r1, #20] \n" \
    "str r2, [r0, #20] \n" \
    "ldr r2, [r1, #24] \n" \
    "str r2, [r0, #24] \n" \
    "ldr r2, [r1, #28] \n" \
    "str r2, [r0, #28] \n" \
    \
    "str r4, [r0, #32] \n" \
    "str r5, [r0, #36] \n" \
    "str r6, [r0, #40] \n" \
    "str r7, [r0, #44] \n" \
    "str r8, [r0, #48] \n" \
    "str r9, [r0, #52] \n" \
    "str r10, [r0, #56] \n" \
    "str r11, [r0, #60] \n" \
    "str r1, [r0, #64] \n"

// Restore all registers except SP
#define RESTORE_REGISTERS \
    "ldr r0, =stack \n" \
    "ldr r1, [r0] \n " \
    "ldr r0, =save_registers \n" \
    "ldr r2, [r0, #0] \n" \
    "str r2, [r1, #0] \n" \
    "ldr r2, [r0, #4] \n" \
    "str r2, [r1, #4] \n" \
    "ldr r2, [r0, #8] \n" \
    "str r2, [r1, #8] \n" \
    "ldr r2, [r0, #12] \n" \
    "str r2, [r1, #12] \n" \
    "ldr r2, [r0, #16] \n" \
    "str r2, [r1, #16] \n" \
    \
    "ldr r2, [r0, #20] \n" \
    "str r2, [r1, #20] \n" \
    "ldr r2, [r0, #24] \n" \
    "str r2, [r1, #24] \n" \
    "ldr r2, [r0, #28] \n" \
    "str r2, [r1, #28] \n" \
    \
    "ldr r4, [r0, #32] \n" \
    "ldr r5, [r0, #36] \n" \
    "ldr r6, [r0, #40] \n" \
    "ldr r7, [r0, #44] \n" \
    "ldr r8, [r0, #48] \n" \
    "ldr r9, [r0, #52] \n" \
    "ldr r10, [r0, #56] \n" \
    "ldr r11, [r0, #60] \n"

void (*original_software_isr)() = NULL;
void (*original_svc_isr)() = NULL;


/**
 * @brief Called by software interrupt. Perform chaining or
 * call handler.
 * 
 */
__attribute__((noinline, naked))
void debug_call_isr() {
  __disable_irq();
  asm volatile(SAVE_STACK);
  asm volatile(SAVE_REGISTERS);
  __enable_irq();
  asm volatile("push {lr}");
  NVIC_CLEAR_PENDING(IRQ_SOFTWARE);

  // Are we in debug mode? If not, just jump to original ISR
  if (debugenabled == 0) {
#if 1
    if (original_software_isr) {
      // asm volatile("ldr r0, =original_software_isr");
      // asm volatile("ldr r0, [r0]");
      // asm volatile("mov pc, r0");
      asm volatile("pop {lr}");
      asm volatile("mov pc, %0" : : "r" (original_software_isr));
    }
#endif
    return;
  }

  if (debugenabled == 2) { // halt permenantly
    while(1) { yield(); }
  }

  debug_monitor();              // process the debug event
  debugenabled = 0;
  // Serial.print("restore regs=");Serial.println(debugrestore);

  // restore registers if they have been changed by gdb
  if (debugrestore) {
    debugrestore = 0;
    asm volatile("pop {r12}");
    __disable_irq();
    asm volatile(RESTORE_REGISTERS);
    __enable_irq();
    asm volatile("mov lr, r12");
    asm volatile("bx lr");
  }
  else {
    asm volatile("pop {pc}");
  }
}

/**
 * @brief Called by SVC ISR to trigger software interrupt
 * 
 */
void debug_call_isr_setup() {
  debugcount++;
  debugenabled = 1;
  // process in lower priority so services can keep running
  NVIC_SET_PENDING(IRQ_SOFTWARE); 
}

#if 1
uint32_t lastpc;

int testOurSVC() {
  uint16_t *memory = (uint16_t*)(lastpc);
  if (((*memory) & 0xFFF0) == 0xdf10 || debug_isBreakpoint(memory)) {
    return 1;
  }
  return 0;
}
#endif

/**
 * @brief SVC handler. Save registers and handle breakpoint.
 * 
 */
__attribute__((noinline, naked))
void svcall_isr() {
#if 1
  // get the PC that triggered this
  // subtract width of svc instruction (which is 2)
  // is it one of our svcs?
  asm volatile(
    "ldr r0, [sp, #24] \n"
    "sub r0, #2 \n"
    "ldr r1, =lastpc \n"
    "str r0, [r1] \n"
    "push {lr}"
  );
  if (testOurSVC()) {
    debug_call_isr_setup();
    asm volatile("pop {pc}");
  }
  else {
    if (original_svc_isr) {
      asm volatile("pop {lr}");
      asm volatile("mov pc, %0" : : "r" (original_svc_isr));
    }
    asm volatile("pop {pc}");
  }
#else
  asm volatile("push {lr}");
  debug_call_isr_setup();
  asm volatile("pop {pc}");
#endif
}

/**
 * @brief Table used by FP_MAP to map memory to breakpoints. This will
 * get copied to RAM and serve as reference. Probably not really needed.
 * 
 */
__attribute__((naked))
void svc_call_table() {
  asm volatile(
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
    "svc #0x10 \n"
    "nop \n"
 );  
}

///////////////////////////// End teensy debug functions ///////////////////////////

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  // Send info 

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */

    __disable_irq();
	asm volatile(SAVE_STACK);
	asm volatile(SAVE_REGISTERS);
	__enable_irq();


  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}


typedef struct __attribute__((packed)) ContextStateFrame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t return_address;
  uint32_t xpsr;
} sContextStateFrame;


// DWTTRAP Indicates the a debug event was generated due to a configuration in the DWT.
// BKPT Indicates one or more breakpoint event took place (either via the FPB or a BKPT instruction).
// HALTED Indicates the core was halted due to a MON_STEP request.
void debug_monitor_handler_c(sContextStateFrame *frame) {
  volatile uint32_t *demcr = (uint32_t *)0xE000EDFC;

  volatile uint32_t *dfsr = (uint32_t *)0xE000ED30;
  const uint32_t dfsr_dwt_evt_bitmask = (1 << 2);
  const uint32_t dfsr_bkpt_evt_bitmask = (1 << 1);
  const uint32_t dfsr_halt_evt_bitmask = (1 << 0);
  const bool is_dwt_dbg_evt = (*dfsr & dfsr_dwt_evt_bitmask);
  const bool is_bkpt_dbg_evt = (*dfsr & dfsr_bkpt_evt_bitmask);
  const bool is_halt_dbg_evt = (*dfsr & dfsr_halt_evt_bitmask);
}

/**
  * @brief This function handles Debug monitor.
  */
__attribute__((naked))
void DebugMon_Handler(void) {
  __asm volatile(
      "tst lr, #4 \n"
      "ite eq \n"
      "mrseq r0, msp \n"
      "mrsne r0, psp \n"
      "b debug_monitor_handler_c \n");
}

// https://interrupt.memfault.com/blog/cortex-m-breakpoints#flash-patch--breakpoint-unit
// Flash patch 
bool fpb_set_breakpoint(size_t comp_id, uint32_t instr_addr) {
  if (instr_addr >= 0x20000000) {
    // for revision 1 only breakpoints in code can be installed :/
    return false;
  }
  // make sure the FPB is enabled
  FP_CTRL |= 0x3;

  const uint32_t replace = (instr_addr & 0x2) == 0 ? 1 : 2;
  const uint32_t fp_comp = (instr_addr & ~0x3) | 0x1 | (replace << 30);
  FP_COMP(comp_id) = fp_comp;
  return true;
}



//////////////////////////////////////////////////////////

bool debug_monitor_enable(target *t) {
  volatile uint32_t *dhcsr = (uint32_t*)0xE000EDF0;
  if ((*dhcsr & 0x1) != 0) {
    tc_printf(t,"Halting Debug Enabled - "
                "Can't Enable Monitor Mode Debug!");
    return false;
  }

  //  ARM CoreSight Architecture Specification.
  FP_LAR = FP_LAR_UNLOCK_KEY; // doesn't do anything, but might in some other processors



  volatile uint32_t *demcr = (uint32_t*)0xE000EDFC;
  const uint32_t mon_en_bit = 16;
  *demcr |= 1 << mon_en_bit;

  // Priority for DebugMonitor Exception is bits[7:0].
  // We will use the lowest priority so other ISRs can
  // fire while in the DebugMonitor Interrupt
  volatile uint32_t *shpr3 = (uint32_t *)0xE000ED20;
  *shpr3 = 0xff;

  //tc_printf(t,"Monitor Mode Debug Enabled!");
  return true;
}





void stm32h7_regs_read(target *t, void *data) {
	// Reorganize
	gdb_regfile.r0 = save_registers.r0;
	gdb_regfile.r1 = save_registers.r1;
	gdb_regfile.r2 = save_registers.r2;
	gdb_regfile.r3 = save_registers.r3;
	gdb_regfile.r4 = save_registers.r4;
	gdb_regfile.r5 = save_registers.r5;
	gdb_regfile.r6 = save_registers.r6;
	gdb_regfile.r7 = save_registers.r7;
	gdb_regfile.r8 = save_registers.r8;
	gdb_regfile.r9 = save_registers.r9;
	gdb_regfile.r10 = save_registers.r10;
	gdb_regfile.r11 = save_registers.r11;
	gdb_regfile.r12 = save_registers.r12;
	gdb_regfile.sp = save_registers.sp;
	gdb_regfile.lr = save_registers.lr;
	gdb_regfile.pc = save_registers.pc;
	//gdb_regfile.xpsr = save_registers.xPSR;
    //gdb_regfile.fpscr = 0xdeadbeef;

	int *p=(int*)&gdb_regfile.r0;
	memcpy(data,p,sizeof(GdbRegFile));

}

static bool stm32h7_cmd_erase_mass(target *t, int argc, const char **argv);
/* static bool stm32h7_cmd_option(target *t, int argc, char *argv[]); */
static bool stm32h7_uid(target *t, int argc, const char **argv);
static bool stm32h7_crc(target *t, int argc, const char **argv);
static bool stm32h7_cmd_psize(target *t, int argc, char *argv[]);
static bool stm32h7_cmd_rev(target *t, int argc, const char **argv);

// Not sure if this works, or is useful
void stm32h7_jump_boot_loader(void)
{
  uint32_t i=0;
  void (*SysMemBootJump)(void);
 
  /* Set the address of the entry point to bootloader */
     volatile uint32_t BootAddr = 0x1FF09800;
 
  /* Disable all interrupts */
     __disable_irq();

  /* Disable Systick timer */
     SysTick->CTRL = 0;
	 
  /* Set the clock to the default state */
     HAL_RCC_DeInit();

  /* Clear Interrupt Enable Register & Interrupt Pending Register */
     for (i=0;i<5;i++)
     {
	  NVIC->ICER[i]=0xFFFFFFFF;
	  NVIC->ICPR[i]=0xFFFFFFFF;
     }	
	 
  /* Re-enable all interrupts */
     __enable_irq();
	
  /* Set up the jump to booloader address + 4 */
     SysMemBootJump = (void (*)(void)) (*((uint32_t *) ((BootAddr + 4))));
 
  /* Set the main stack pointer to the bootloader stack */
     __set_MSP(*(uint32_t *)BootAddr);
 
  /* Call the function to jump to bootloader location */
     SysMemBootJump(); 
  
  /* Jump is done successfully */
     while (1)
     {
      /* Code should never reach this loop */
     }
}

extern  void  boot_uboot();

const struct command_s stm32h7_cmd_list[] = {
	{"erase_mass", (cmd_handler)stm32h7_cmd_erase_mass,
	 "Erase entire flash memory"},
	{"boot_loader", (cmd_handler)stm32h7_jump_boot_loader,
	 "Jump to bootloader"},
	{"boot_uboot", (cmd_handler)boot_uboot,
	 "Jump to uboot @0x90000000"},
/*	{"option", (cmd_handler)stm32h7_cmd_option,
	"Manipulate option bytes"},*/
	{"psize", (cmd_handler)stm32h7_cmd_psize,
	 "Configure flash write parallelism: (x8|x16|x32|x64(default))"},
	{"uid", (cmd_handler)stm32h7_uid, "Print unique device ID"},
	{"crc", (cmd_handler)stm32h7_crc, "Print CRC of both banks"},
	{"revision", (cmd_handler)stm32h7_cmd_rev,
	 "Returns the Device ID and Revision"},
	{NULL, NULL, NULL}
};


static const char tdesc_h7[] =
	"<?xml version=\"1.0\"?>"
	"<!DOCTYPE target SYSTEM \"gdb-target.dtd\">"
	"<target>"
	"  <architecture>arm</architecture>"
	"  <feature name=\"org.gnu.gdb.arm.m-profile\">"
	"    <reg name=\"r0\" bitsize=\"32\"/>"
	"    <reg name=\"r1\" bitsize=\"32\"/>"
	"    <reg name=\"r2\" bitsize=\"32\"/>"
	"    <reg name=\"r3\" bitsize=\"32\"/>"
	"    <reg name=\"r4\" bitsize=\"32\"/>"
	"    <reg name=\"r5\" bitsize=\"32\"/>"
	"    <reg name=\"r6\" bitsize=\"32\"/>"
	"    <reg name=\"r7\" bitsize=\"32\"/>"
	"    <reg name=\"r8\" bitsize=\"32\"/>"
	"    <reg name=\"r9\" bitsize=\"32\"/>"
	"    <reg name=\"r10\" bitsize=\"32\"/>"
	"    <reg name=\"r11\" bitsize=\"32\"/>"
	"    <reg name=\"r12\" bitsize=\"32\"/>"
	"    <reg name=\"sp\" bitsize=\"32\" type=\"data_ptr\"/>"
	"    <reg name=\"lr\" bitsize=\"32\" type=\"code_ptr\"/>"
	"    <reg name=\"pc\" bitsize=\"32\" type=\"code_ptr\"/>"
	"    <reg name=\"xpsr\" bitsize=\"32\"/>"
	"    <reg name=\"msp\" bitsize=\"32\" save-restore=\"no\" type=\"data_ptr\"/>"
	"    <reg name=\"psp\" bitsize=\"32\" save-restore=\"no\" type=\"data_ptr\"/>"
	"    <reg name=\"primask\" bitsize=\"8\" save-restore=\"no\"/>"
	"    <reg name=\"basepri\" bitsize=\"8\" save-restore=\"no\"/>"
	"    <reg name=\"faultmask\" bitsize=\"8\" save-restore=\"no\"/>"
	"    <reg name=\"control\" bitsize=\"8\" save-restore=\"no\"/>"
	"  </feature>"
	"</target>";

static const char h7_memory_map[] =
"<?xml version=\"1.0\"?>"
"<!DOCTYPE memory-map PUBLIC \"+//IDN gnu.org//DTD GDB Memory Map V1.0//EN\""
"     \"http://sourceware.org/gdb/gdb-memory-map.dtd\">"
"<memory-map>  <memory type=\"rom\" start=\"0x00000000\" length=\"0x10000\"/>"
"  <memory type=\"ram\" start=\"0x20000000\" length=\"0x20000\"/>"
"  <memory type=\"ram\" start=\"0x24000000\" length=\"0x80000\"/>"
"  <memory type=\"ram\" start=\"0x30000000\" length=\"0x48000\"/> "
"  <memory type=\"ram\" start=\"0x38000000\" length=\"0x10000\"/>  "
"  <memory type=\"ram\" start=\"0xC0000000\" length=\"0x2000000\"/>  "
"  <memory type=\"flash\" start=\"0x08000000\" length=\"0x20000\"> "
"    <property name=\"blocksize\">0x20000</property>  "
"  </memory>  "
"  <memory type=\"flash\" start=\"0x90000000\" length=\"0x600000\"> "
"    <property name=\"blocksize\">0x20000</property>  "
"  </memory>  "
"  <memory type=\"ram\" start=\"0x40000000\" length=\"0x1fffffff\"/> "
"  <memory type=\"ram\" start=\"0xe0000000\" length=\"0x1fffffff\"/> "
"  <memory type=\"rom\" start=\"0x1ff00000\" length=\"0x20000\"/>"
"</memory-map>";

/*
	target_add_ram(t, 0xC0000000,  0x2000000);    //  External SDRAM 32MB 
	// 16 Meg Flash W25Q64
	stm32h7_add_quadspi_flash(t,0x90000000, 0x600000, FLASH_SECTOR_SIZE);
*/

static int stm32h7_flash_erase(struct target_flash *f, target_addr addr,
							   size_t len);
static int stm32h7_flash_write(struct target_flash *f,
                               target_addr dest, const void *src, size_t len);

static const char stm32h7_driver_str[] = "STM32H7";

enum stm32h7_regs
{
	FLASH_ACR		= 0x00,
	FLASH_KEYR		= 0x04,
	FLASH_OPTKEYR	= 0x08,
	FLASH_CR		= 0x0c,
	FLASH_SR		= 0x10,
	FLASH_CCR		= 0x14,
	FLASH_OPTCR		= 0x18,
	FLASH_OPTSR_CUR = 0x1C,
	FLASH_OPTSR     = 0x20,
	FLASH_CRCCR		= 0x50,
	FLASH_CRCDATA	= 0x5C,
};

/* Flash Program and Erase Controller Register Map */
#define H7_IWDG_BASE        0x58004c00
#define FPEC1_BASE			0x52002000
#define FPEC2_BASE			0x52002100

// Also defiend in HAL file

#undef FLASH_SR_BSY	
#undef FLASH_SR_WBNE		
#undef FLASH_SR_QW			
#undef FLASH_SR_CRC_BUSY
#undef FLASH_SR_EOP		
#undef FLASH_SR_WRPERR		
#undef FLASH_SR_PGSERR		
#undef FLASH_SR_STRBERR
#undef FLASH_SR_INCERR		
#undef FLASH_SR_OPERR		
#undef FLASH_SR_OPERR		
#undef FLASH_SR_RDPERR		
#undef FLASH_SR_RDSERR		
#undef FLASH_SR_SNECCERR	
#undef FLASH_SR_DBERRERR	

#undef FLASH_CR_LOCK		
#undef FLASH_CR_PG			
#undef FLASH_CR_SER	
#undef FLASH_CR_BER		
#undef FLASH_CR_PSIZE8		
#undef FLASH_CR_PSIZE16	
#undef FLASH_CR_PSIZE32	
#undef FLASH_CR_PSIZE64	
#undef FLASH_CR_FW			
#undef FLASH_CR_START	
#undef FLASH_CR_SNB_1		
#undef FLASH_CR_SNB		
#undef FLASH_CR_CRC_EN		



#define FLASH_SR_BSY		(1 <<  0)
#define FLASH_SR_WBNE		(1 <<  1)
#define FLASH_SR_QW			(1 <<  2)
#define FLASH_SR_CRC_BUSY	(1 <<  3)
#define FLASH_SR_EOP		(1 << 16)
#define FLASH_SR_WRPERR		(1 << 17)
#define FLASH_SR_PGSERR		(1 << 18)
#define FLASH_SR_STRBERR	(1 << 19)
#define FLASH_SR_INCERR		(1 << 21)
#define FLASH_SR_OPERR		(1 << 22)
#define FLASH_SR_OPERR		(1 << 22)
#define FLASH_SR_RDPERR		(1 << 23)
#define FLASH_SR_RDSERR		(1 << 24)
#define FLASH_SR_SNECCERR	(1 << 25)
#define FLASH_SR_DBERRERR	(1 << 26)
#define FLASH_SR_ERROR_READ	(FLASH_SR_RDPERR   | FLASH_SR_RDSERR  |	\
							 FLASH_SR_SNECCERR |FLASH_SR_DBERRERR)
#define FLASH_SR_ERROR_MASK	(										\
		FLASH_SR_WRPERR  | FLASH_SR_PGSERR  | FLASH_SR_STRBERR |	\
		FLASH_SR_INCERR  | FLASH_SR_OPERR    | FLASH_SR_ERROR_READ)
#define FLASH_CR_LOCK		(1 << 0)
#define FLASH_CR_PG			(1 << 1)
#define FLASH_CR_SER		(1 << 2)
#define FLASH_CR_BER		(1 << 3)
#define FLASH_CR_PSIZE8		(0 << 4)
#define FLASH_CR_PSIZE16	(1 << 4)
#define FLASH_CR_PSIZE32	(2 << 4)
#define FLASH_CR_PSIZE64	(3 << 4)
#define FLASH_CR_FW			(1 << 6)
#define FLASH_CR_START		(1 << 7)
#define FLASH_CR_SNB_1		(1 << 8)
#define FLASH_CR_SNB		(3 << 8)
#define FLASH_CR_CRC_EN		(1 << 15)

#define FLASH_OPTCR_OPTLOCK	(1 << 0)
#define FLASH_OPTCR_OPTSTRT	(1 << 1)

#define FLASH_OPTSR_IWDG1_SW	(1 <<  4)


#define FLASH_CRCCR_ALL_BANK	(1 <<  7)
#define FLASH_CRCCR_START_CRC	(1 << 16)
#define FLASH_CRCCR_CLEAN_CRC	(1 << 17)
#define FLASH_CRCCR_CRC_BURST_3	(3 << 20)

#if 0

#endif


#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

#define OPTKEY1 0x08192A3B
#define OPTKEY2 0x4C5D6E7F




#define DBGMCU_IDCODE	0x5c001000
/* Access from processor address space.
 * Access via the APB-D is at 0xe00e1000 */
#define DBGMCU_IDC		(DBGMCU_IDCODE + 0)
#define DBGMCU_CR		(DBGMCU_IDCODE + 4)
#define DBGSLEEP_D1		(1 << 0)
#define DBGSTOP_D1		(1 << 1)
#define DBGSTBY_D1		(1 << 2)
#define DBGSTOP_D3		(1 << 7)
#define DBGSTBY_D3		(1 << 8)
#define D1DBGCKEN		(1 << 21)
#define D3DBGCKEN		(1 << 22)


#define BANK1_START 		0x08000000
#define NUM_SECTOR_PER_BANK 8
#define FLASH_SECTOR_SIZE 	0x20000
#define BANK2_START         0x08100000
enum ID_STM32H7 {
	ID_STM32H74x  = 0x450,      /* RM0433, RM0399 */
	ID_STM32H7Bx  = 0x480,      /* RM0455 */
	ID_STM32H72x  = 0x483,      /* RM0468 */
};

struct stm32h7_flash {
	struct target_flash f;
	enum align psize;
	uint32_t regbase;
};

struct stm32h7_priv_s {
	uint32_t dbg_cr;
};

static void stm32h7_add_flash(target *t,
                              uint32_t addr, size_t length, size_t blocksize)
{
	struct stm32h7_flash *sf = calloc(1, sizeof(*sf));
	struct target_flash *f;

	if (!sf) {			/* calloc failed: heap exhaustion */
		DEBUG_WARN("calloc: failed in %s\n", __func__);
		return;
	}

	f = &sf->f;
	f->start = addr;
	f->length = length;
	f->blocksize = blocksize;
	f->erase = stm32h7_flash_erase;
	f->write = stm32h7_flash_write;
	f->buf_size = 2048;
	f->erased = 0xff;
	sf->regbase = FPEC1_BASE;
	if (addr >= BANK2_START)
		sf->regbase = FPEC2_BASE;
	sf->psize = ALIGN_DWORD;
	target_add_flash(t, f);
}


static bool stm32h7_external_flash_unlock(target *t, uint32_t addr)
{
#ifdef NOT_NOW
	W25QXX_EnterQPIMode();
	W25QXX_WriteEnable(1);
#endif

#if 0
	uint32_t regbase = FPEC1_BASE;
	if (addr >= BANK2_START) {
		regbase = FPEC2_BASE;
	}

	while(target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_BSY) {
		if(target_check_error(t))
			return false;
	}
	uint32_t sr = target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_ERROR_MASK;
	if (sr) {
		DEBUG_WARN("%s error 0x%08" PRIx32, __func__, sr);
		target_mem_write32(t, regbase + FLASH_CCR, sr);
		return false;
	}
	if (target_mem_read32(t, regbase + FLASH_CR) & FLASH_CR_LOCK) {
		/* Enable FLASH controller access */
		target_mem_write32(t, regbase + FLASH_KEYR, KEY1);
		target_mem_write32(t, regbase + FLASH_KEYR, KEY2);
	}
	if (target_mem_read32(t, regbase + FLASH_CR) & FLASH_CR_LOCK)
		return false;
	else

#endif
		return true;
}



static int stm32h7_external_flash_erase(struct target_flash *f, target_addr addr,
							   size_t len)
{
	target *t = f->t;
	struct stm32h7_flash *sf = (struct stm32h7_flash *)f;
	if (stm32h7_external_flash_unlock(t, addr) == false)
		return -1;
	/* We come out of reset with HSI 64 MHz. Adapt FLASH_ACR.*/
	//target_mem_write32(t, sf->regbase + FLASH_ACR, 0);
	addr &= (NUM_SECTOR_PER_BANK * FLASH_SECTOR_SIZE) - 1;
	int start_sector =  addr / FLASH_SECTOR_SIZE;
	int end_sector   = (addr + len - 1) / FLASH_SECTOR_SIZE;

	enum align psize = ((struct stm32h7_flash *)f)->psize;
	uint32_t sr;
	while (start_sector <= end_sector) 
	{
	#ifdef NOT_NOW
		W25QXX_SectorErase(start_sector);
	#endif
		/*
		uint32_t cr = (psize * FLASH_CR_PSIZE16) | FLASH_CR_SER |
			(start_sector * FLASH_CR_SNB_1);
		target_mem_write32(t, sf->regbase + FLASH_CR, cr);
		cr |= FLASH_CR_START;
		target_mem_write32(t, sf->regbase + FLASH_CR, cr);
		DEBUG_INFO(" started cr %08" PRIx32 " sr %08" PRIx32 "\n",
				   target_mem_read32(t, sf->regbase + FLASH_CR),
				   target_mem_read32(t, sf->regbase + FLASH_SR));
		do {
			sr = target_mem_read32(t, sf->regbase + FLASH_SR);
			if (target_check_error(t)) {
				DEBUG_WARN("stm32h7_flash_erase: comm failed\n");
				return -1;
			}
//			target_mem_write32(t, H7_IWDG_BASE, 0x0000aaaa);
		}while (sr & (FLASH_SR_QW | FLASH_SR_BSY));
		if (sr & FLASH_SR_ERROR_MASK) {
			DEBUG_WARN("stm32h7_flash_erase: error, sr: %08" PRIx32 "\n", sr);
			return -1;
		}
		*/
		start_sector++;
	}
	return 0;
}

static int stm32h7_external_flash_write(struct target_flash *f, target_addr dest,
                               const void *src, size_t len)
{
	target *t = f->t;
	struct stm32h7_flash *sf = (struct stm32h7_flash *)f;
	enum align psize = sf->psize;
	if (stm32h7_external_flash_unlock(t, dest) == false)
		return -1;
	uint32_t cr = psize * FLASH_CR_PSIZE16;
	// Sector erase starts at 0.
	//W25QXX_Write(src, dest-f->start, len);
#ifdef NOT_NOW
	W25QXX_Write(src, dest, len);
#endif

/*	
	target_mem_write32(t, sf->regbase + FLASH_CR, cr);
	cr |= FLASH_CR_PG;
	target_mem_write32(t, sf->regbase + FLASH_CR, cr);
	// does H7 stall?
	uint32_t sr_reg = sf->regbase + FLASH_SR;
	uint32_t sr;
	target_mem_write(t, dest, src, len);
	while ((sr = target_mem_read32(t, sr_reg)) & FLASH_SR_BSY) {
		if(target_check_error(t)) {
			DEBUG_WARN("stm32h7_flash_write: BSY comm failed\n");
			return -1;
		}
	}
	if (sr & FLASH_SR_ERROR_MASK) {
		DEBUG_WARN("stm32h7_flash_write: error sr %08" PRIx32 "\n", sr);
		return -1;
	}
	// Close write windows.
	target_mem_write32(t, sf->regbase + FLASH_CR, 0);
*/
	return 0;
}


static void stm32h7_add_quadspi_flash(target *t,
                              uint32_t addr, size_t length, size_t blocksize)
{
	struct stm32h7_flash *sf = calloc(1, sizeof(*sf));
	struct target_flash *f;

	if (!sf) {			/* calloc failed: heap exhaustion */
		DEBUG_WARN("calloc: failed in %s\n", __func__);
		return;
	}

	f = &sf->f;
	f->start = addr;
	f->length = length;
	f->blocksize = blocksize;
	f->erase = stm32h7_external_flash_erase;
	f->write = stm32h7_external_flash_write;
	f->buf_size = 2048;
	f->erased = 0xff;
	sf->regbase = FPEC1_BASE;
	if (addr >= BANK2_START)
		sf->regbase = FPEC2_BASE;
	sf->psize = ALIGN_DWORD;
	target_add_flash(t, f);
}


static bool stm32h7_attach(target *t)
{
	if (!cortexm_attach(t))
		return false;
	/* If IWDG runs as HARDWARE watchdog (44.3.4) erase
	 * will be aborted by the Watchdog and erase fails!
	 * Setting IWDG_KR to 0xaaaa does not seem to help!*/
	uint32_t optsr = target_mem_read32(t, FPEC1_BASE + FLASH_OPTSR);
	if (!(optsr & FLASH_OPTSR_IWDG1_SW))
		tc_printf(t, "Hardware IWDG running. Expect failure. Set IWDG1_SW!");

	/* Free previously loaded memory map */
	target_mem_map_free(t);

	/* Add RAM to memory map */
	/* Table 7. Memory map and default device memory area attributes RM 0433, pg 130 */
	target_add_ram(t, 0x00000000, 0x10000); /* ITCM Ram,  64 k */
	target_add_ram(t, 0x20000000, 0x20000); /* DTCM Ram, 128 k */
	target_add_ram(t, 0x24000000, 0x80000); /* AXI Ram,  512 k */
	target_add_ram(t, 0x30000000, 0x20000); /* AHB SRAM1, 128 k */
	target_add_ram(t, 0x30020000, 0x20000); /* AHB SRAM2, 128 k */
	target_add_ram(t, 0x30040000, 0x08000); /* AHB SRAM3,  32 k */
	target_add_ram(t, 0x38000000, 0x10000); /* AHB SRAM4,  64 k */

	/* Add the flash to memory map. */
	stm32h7_add_flash(t, 0x8000000, 0x100000, FLASH_SECTOR_SIZE);
	stm32h7_add_flash(t, 0x8100000, 0x100000, FLASH_SECTOR_SIZE);

	// Specific to the ART-Pi board
	// 32Meg RAM
	target_add_ram(t, 0xC0000000,  0x2000000);    /* External SDRAM 32MB */

	// 16 Meg Flash W25Q64
	stm32h7_add_quadspi_flash(t,0x90000000, 16*0x100000, FLASH_SECTOR_SIZE);
	return true;
}

static void stm32h7_detach(target *t)
{
	struct stm32h7_priv_s *ps = (struct stm32h7_priv_s*)t->target_storage;

	target_mem_write32(t, DBGMCU_CR, ps->dbg_cr);
	cortexm_detach(t);
}

bool stm32h7_probe(target *t)
{
	cortexm_probe(t);
	uint32_t idcode = t->idcode;
	if (idcode == ID_STM32H74x || idcode == ID_STM32H7Bx || idcode == ID_STM32H72x) {
		t->driver = stm32h7_driver_str;
		t->attach = stm32h7_attach;
		t->detach = stm32h7_detach;
		target_add_commands(t, stm32h7_cmd_list, stm32h7_driver_str);
		/* Save private storage */
		struct stm32h7_priv_s *priv_storage = calloc(1, sizeof(*priv_storage));
		priv_storage->dbg_cr = target_mem_read32(t, DBGMCU_CR);
		t->target_storage = (void*)priv_storage;
		/* RM0433 Rev 4 is not really clear, what bits are needed in DBGMCU_CR.
		 * Maybe more flags needed?
		 */
		uint32_t dbgmcu_cr = DBGSLEEP_D1 | D1DBGCKEN;
		target_mem_write32(t, DBGMCU_CR,  dbgmcu_cr);
		return true;
	}
	return false;
}

void h7_mem_read(target *t, void *dest, target_addr src,size_t len) {
#if 0
	// Hocus pocus
   	//printf("esp32_mem_read %d\n",len);
	int * i = (int *) (src & (~3));

	// TODO: better address range check?
	//if (src < 0x20000000 || src >= 0x60000000) {
	//	return;
	//}

    int * dm = (int *) dest;
	for(int j=0; j<len/4; j++) {
	    *dm= *i >> ((src & 3) * 8);
		dm++;
		i++;
    }
#endif
    if (len==4) {
		uint32_t *source=(uint32_t *)src;
		uint32_t *destination=(uint32_t *)dest;
		*destination=*source;
	}
	else
	{
		unsigned char *source=(unsigned char *)src;
		unsigned char *destination=(unsigned char *)dest;

		for(int j=0; j<len; j++) 
		{
			*destination=*source;
			source++;
			destination++;
		}
	}
}

void h7_mem_write(target *t, target_addr dest,const void *src, size_t len) {
#if 0
  int * ip = (int *) (dest & (~3));
  int * sip = (int *) (dest & (~3));

  for(int i=0; i<len/4; i++) {
    ip[i] = sip[i];
  }
#endif
    if (len==4) {
		uint32_t *source=(uint32_t *)src;
		uint32_t *destination=(uint32_t *)dest;
		*destination=*source;
	}
	else
	{
		unsigned char *source=(unsigned char *)src;
		unsigned char *destination=(unsigned char *)dest;

		for(int j=0; j<len; j++) 
		{
			*destination=*source;
			source++;
			destination++;
		}
	}
}

static bool h7_check_error(target *t)
{
	return false;
}

static void h7_priv_free(void *priv)
{
	free(priv);
}

static void h7_halt_request(target *t)
{
	target_mem_write32(t, CORTEXM_DHCSR, CORTEXM_DHCSR_DBGKEY |
												CORTEXM_DHCSR_C_HALT |
												CORTEXM_DHCSR_C_DEBUGEN);
}


//static bool adi_check_error(ADIv5_AP_s *a)
//{
//	return false;
//}

static void h7_halt_resume(target *t, bool step)
{
	struct ADIv5_AP_s *priv = t->priv;
	uint32_t dhcsr = CORTEXM_DHCSR_DBGKEY | CORTEXM_DHCSR_C_DEBUGEN;

	if (step)
		dhcsr |= CORTEXM_DHCSR_C_STEP | CORTEXM_DHCSR_C_MASKINTS;

	/* Disable interrupts while single stepping... */
	if(step != priv->stepping) {
		target_mem_write32(t, CORTEXM_DHCSR, dhcsr | CORTEXM_DHCSR_C_HALT);
		priv->stepping = step;
	}

/*
	if (priv->on_bkpt) {
		uint32_t pc = cortexm_pc_read(t);
		if ((target_mem_read16(t, pc) & 0xFF00) == 0xBE00)
			cortexm_pc_write(t, pc + 2);
	}
*/

	if (priv->has_cache)
		target_mem_write32(t, CORTEXM_ICIALLU, 0);

	target_mem_write32(t, CORTEXM_DHCSR, dhcsr);
}

static void  h7_reset(target *t)
{

}

static void h7_regs_write(target *t, const void *data)
{
	const uint32_t *regs = data;


}

target *stm32h7_probe_with_controller(struct target_controller *controller)
{
	target *t;

	t = target_new();
	struct ADIv5_AP_s *priv = calloc(1, sizeof(*priv));
	// priv->error=adi_check_error;
	t->priv = priv;

	t->priv_free = h7_priv_free;
	t->check_error = h7_check_error;
	t->mem_read = h7_mem_read;
	t->mem_write = h7_mem_write;
	t->halt_request = h7_halt_request;

	// Force id for use with qemu
	t->idcode=0x450;

    t->driver = stm32h7_driver_str;

    target_check_error(t);
	stm32h7_probe(t);

	// Back to default
	t->mem_read = h7_mem_read;
	t->mem_write = h7_mem_write;
	t->regs_read=stm32h7_regs_read;
	t->tdesc = tdesc_h7;
	t->dyn_mem_map = h7_memory_map;
	t->halt_resume = h7_halt_resume;
	t->reset = h7_reset;
	t->regs_write = h7_regs_write;

// Test set registers


    target_attach(t,controller);

};



static bool stm32h7_flash_unlock(target *t, uint32_t addr)
{
	uint32_t regbase = FPEC1_BASE;
	if (addr >= BANK2_START) {
		regbase = FPEC2_BASE;
	}

	while(target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_BSY) {
		if(target_check_error(t))
			return false;
	}
	uint32_t sr = target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_ERROR_MASK;
	if (sr) {
		DEBUG_WARN("%s error 0x%08" PRIx32, __func__, sr);
		target_mem_write32(t, regbase + FLASH_CCR, sr);
		return false;
	}
	if (target_mem_read32(t, regbase + FLASH_CR) & FLASH_CR_LOCK) {
		/* Enable FLASH controller access */
		target_mem_write32(t, regbase + FLASH_KEYR, KEY1);
		target_mem_write32(t, regbase + FLASH_KEYR, KEY2);
	}
	if (target_mem_read32(t, regbase + FLASH_CR) & FLASH_CR_LOCK)
		return false;
	else
		return true;
}

static int stm32h7_flash_erase(struct target_flash *f, target_addr addr,
							   size_t len)
{
	target *t = f->t;
	struct stm32h7_flash *sf = (struct stm32h7_flash *)f;
	if (stm32h7_flash_unlock(t, addr) == false)
		return -1;
	/* We come out of reset with HSI 64 MHz. Adapt FLASH_ACR.*/
	target_mem_write32(t, sf->regbase + FLASH_ACR, 0);
	addr &= (NUM_SECTOR_PER_BANK * FLASH_SECTOR_SIZE) - 1;
	int start_sector =  addr / FLASH_SECTOR_SIZE;
	int end_sector   = (addr + len - 1) / FLASH_SECTOR_SIZE;

	enum align psize = ((struct stm32h7_flash *)f)->psize;
	uint32_t sr;
	while (start_sector <= end_sector) {
		uint32_t cr = (psize * FLASH_CR_PSIZE16) | FLASH_CR_SER |
			(start_sector * FLASH_CR_SNB_1);
		target_mem_write32(t, sf->regbase + FLASH_CR, cr);
		cr |= FLASH_CR_START;
		target_mem_write32(t, sf->regbase + FLASH_CR, cr);
		DEBUG_INFO(" started cr %08" PRIx32 " sr %08" PRIx32 "\n",
				   target_mem_read32(t, sf->regbase + FLASH_CR),
				   target_mem_read32(t, sf->regbase + FLASH_SR));
		do {
			sr = target_mem_read32(t, sf->regbase + FLASH_SR);
			if (target_check_error(t)) {
				DEBUG_WARN("stm32h7_flash_erase: comm failed\n");
				return -1;
			}
//			target_mem_write32(t, H7_IWDG_BASE, 0x0000aaaa);
		}while (sr & (FLASH_SR_QW | FLASH_SR_BSY));
		if (sr & FLASH_SR_ERROR_MASK) {
			DEBUG_WARN("stm32h7_flash_erase: error, sr: %08" PRIx32 "\n", sr);
			return -1;
		}
		start_sector++;
	}
	return 0;
}

static int stm32h7_flash_write(struct target_flash *f, target_addr dest,
                               const void *src, size_t len)
{
	target *t = f->t;
	struct stm32h7_flash *sf = (struct stm32h7_flash *)f;
	enum align psize = sf->psize;
	if (stm32h7_flash_unlock(t, dest) == false)
		return -1;
	uint32_t cr = psize * FLASH_CR_PSIZE16;
	target_mem_write32(t, sf->regbase + FLASH_CR, cr);
	cr |= FLASH_CR_PG;
	target_mem_write32(t, sf->regbase + FLASH_CR, cr);
	/* does H7 stall?*/
	uint32_t sr_reg = sf->regbase + FLASH_SR;
	uint32_t sr;
	target_mem_write(t, dest, src, len);
	while ((sr = target_mem_read32(t, sr_reg)) & FLASH_SR_BSY) {
		if(target_check_error(t)) {
			DEBUG_WARN("stm32h7_flash_write: BSY comm failed\n");
			return -1;
		}
	}
	if (sr & FLASH_SR_ERROR_MASK) {
		DEBUG_WARN("stm32h7_flash_write: error sr %08" PRIx32 "\n", sr);
		return -1;
	}
	/* Close write windows.*/
	target_mem_write32(t, sf->regbase + FLASH_CR, 0);
	return 0;
}

/* Both banks are erased in parallel.*/
static bool stm32h7_cmd_erase(target *t, int bank_mask)
{
	const char spinner[] = "|/-\\";
	int spinindex = 0;
	bool do_bank1 = bank_mask & 1, do_bank2 = bank_mask & 2;
	uint32_t cr;
	bool result = false;
	enum align psize = ALIGN_DWORD;
	for (struct target_flash *f = t->flash; f; f = f->next) {
		if (f->write == stm32h7_flash_write) {
			psize = ((struct stm32h7_flash *)f)->psize;
		}
	}
	cr = (psize * FLASH_CR_PSIZE16) | FLASH_CR_BER | FLASH_CR_START;
	/* Flash mass erase start instruction */
	if (do_bank1) {
		if (stm32h7_flash_unlock(t, BANK1_START) == false) {
			DEBUG_WARN("ME: Unlock bank1 failed\n");
			goto done;
		}
		uint32_t regbase = FPEC1_BASE;
		/* BER and start can be merged (3.3.10).*/
		target_mem_write32(t, regbase + FLASH_CR, cr);
		DEBUG_INFO("ME bank1 started\n");
	}
	if (do_bank2) {
		if (stm32h7_flash_unlock(t, BANK2_START) == false) {
			DEBUG_WARN("ME: Unlock bank2 failed\n");
			goto done;
		}
		uint32_t regbase = FPEC2_BASE;
		/* BER and start can be merged (3.3.10).*/
		target_mem_write32(t, regbase + FLASH_CR, cr);
		DEBUG_INFO("ME bank2 started\n");
	}

	/* Read FLASH_SR to poll for QW bit */
	if (do_bank1) {
		uint32_t regbase = FPEC1_BASE;
		while (target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_QW) {
//			target_mem_write32(t, H7_IWDG_BASE, 0x0000aaaa);
			tc_printf(t, "\b%c", spinner[spinindex++ % 4]);
			if(target_check_error(t)) {
				DEBUG_WARN("ME bank1: comm failed\n");
				goto done;
			}
		}
	}
	if (do_bank2) {
		uint32_t regbase = FPEC2_BASE;
		while (target_mem_read32(t, regbase + FLASH_SR) & FLASH_SR_QW) {
//			target_mem_write32(t, H7_IWDG_BASE 0x0000aaaa);
			tc_printf(t, "\b%c", spinner[spinindex++ % 4]);
			if(target_check_error(t)) {
				DEBUG_WARN("ME bank2: comm failed\n");
				goto done;
			}
		}
	}

	if (do_bank1) {
		/* Check for error */
		uint32_t regbase = FPEC1_BASE;
		uint32_t sr = target_mem_read32(t, regbase + FLASH_SR);
		if (sr & FLASH_SR_ERROR_MASK) {
			DEBUG_WARN("ME bank1, error sr %" PRIx32 "\n", sr);
			goto done;
		}
	}
	if (do_bank2) {
		/* Check for error */
		uint32_t regbase = FPEC2_BASE;
		uint32_t sr = target_mem_read32(t, regbase + FLASH_SR);
		if (sr & FLASH_SR_ERROR_MASK) {
			DEBUG_WARN("ME bank2, error: sr %" PRIx32 "\n", sr);
			goto done;
		}
	}
	result = true;
  done:
	tc_printf(t, "\n");
	return result;
}

static bool stm32h7_cmd_erase_mass(target *t, int argc, const char **argv)
{
	(void)argc;
	(void)argv;
	tc_printf(t, "Erasing flash... This may take a few seconds.  ");
	return stm32h7_cmd_erase(t, 3);
}

/* Print the Unique device ID.
 * Can be reused for other STM32 devices With uid as parameter.
 */
static bool stm32h7_uid(target *t, int argc, const char **argv)
{
	(void)argc;
	(void)argv;

	uint32_t uid = 0x1ff1e800;
	if (t->idcode == ID_STM32H7Bx) {
		uid = 0x08fff800;  /* 7B3/7A3/7B0 */
	}

	int i;
	tc_printf(t, "0x");
	for (i = 0; i < 12; i = i + 4) {
		uint32_t val = target_mem_read32(t, uid + i);
		tc_printf(t, "%02X", (val >> 24) & 0xff);
		tc_printf(t, "%02X", (val >> 16) & 0xff);
		tc_printf(t, "%02X", (val >>  8) & 0xff);
		tc_printf(t, "%02X", (val >>  0) & 0xff);
	}
	tc_printf(t, "\n");
	return true;
}
static int stm32h7_crc_bank(target *t, uint32_t bank)
{
	uint32_t regbase = FPEC1_BASE;
	if (bank >= BANK2_START)
		regbase = FPEC2_BASE;

	if (stm32h7_flash_unlock(t, bank) == false)
			return -1;
	uint32_t cr = FLASH_CR_CRC_EN;
	target_mem_write32(t, regbase + FLASH_CR, cr);
	uint32_t crccr= FLASH_CRCCR_CRC_BURST_3 |
		FLASH_CRCCR_CLEAN_CRC | FLASH_CRCCR_ALL_BANK;
	target_mem_write32(t, regbase + FLASH_CRCCR, crccr);
	target_mem_write32(t, regbase + FLASH_CRCCR, crccr | FLASH_CRCCR_START_CRC);
	uint32_t sr;
	while ((sr = target_mem_read32(t, regbase + FLASH_SR)) &
		   FLASH_SR_CRC_BUSY) {
		if(target_check_error(t)) {
			DEBUG_WARN("CRC bank %d: comm failed\n",
					   (bank < BANK2_START) ? 1 : 2);
			return -1;
		}
		if (sr & FLASH_SR_ERROR_READ) {
			DEBUG_WARN("CRC bank %d: error sr %08" PRIx32 "\n",
				  (bank < BANK2_START) ? 1 : 2, sr);
			return -1;
		}
	}
	return 0;
}

static bool stm32h7_crc(target *t, int argc, const char **argv)
{
	(void)argc;
	(void)argv;
	if (stm32h7_crc_bank(t, BANK1_START) ) return false;
	uint32_t crc1 = target_mem_read32(t, FPEC1_BASE + FLASH_CRCDATA);
	if (stm32h7_crc_bank(t, BANK2_START) ) return false;
	uint32_t crc2 = target_mem_read32(t, FPEC1_BASE + FLASH_CRCDATA);
	tc_printf(t, "CRC: bank1 0x%08lx, bank2 0x%08lx\n", crc1, crc2);
	return true;
}
static bool stm32h7_cmd_psize(target *t, int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	if (argc == 1) {
		enum align psize = ALIGN_DWORD;
		for (struct target_flash *f = t->flash; f; f = f->next) {
			if (f->write == stm32h7_flash_write) {
				psize = ((struct stm32h7_flash *)f)->psize;
			}
		}
		tc_printf(t, "Flash write parallelism: %s\n",
		          psize == ALIGN_DWORD ? "x64" :
		          psize == ALIGN_WORD  ? "x32" :
				  psize == ALIGN_HALFWORD ? "x16" : "x8");
	} else {
		enum align psize;
		if (!strcmp(argv[1], "x8")) {
			psize = ALIGN_BYTE;
		} else if (!strcmp(argv[1], "x16")) {
			psize = ALIGN_HALFWORD;
		} else if (!strcmp(argv[1], "x32")) {
			psize = ALIGN_WORD;
		} else if (!strcmp(argv[1], "x64")) {
			psize = ALIGN_DWORD;
		} else {
			tc_printf(t, "usage: monitor psize (x8|x16|x32|x64)\n");
			return false;
		}
		for (struct target_flash *f = t->flash; f; f = f->next) {
			if (f->write == stm32h7_flash_write) {
				((struct stm32h7_flash *)f)->psize = psize;
			}
		}
	}
	return true;
}

static const struct stm32h7xx_rev {
	uint32_t rev_id;
	char revision;
} stm32h7xx_revisions[] = {
	{ 0x1000, 'A' },
	{ 0x1001, 'Z' },
	{ 0x1003, 'Y' },
	{ 0x2001, 'X' },
	{ 0x2003, 'V' }
};
static bool stm32h7_cmd_rev(target *t, int argc, const char **argv)
{
	(void)argc;
	(void)argv;
	/* DBGMCU identity code register */
	uint32_t dbgmcu_idc = target_mem_read32(t, DBGMCU_IDC);
	uint16_t rev_id = (dbgmcu_idc >> 16) & 0xFFFF;
	uint16_t dev_id = dbgmcu_idc & 0xFFF;

	/* Print device */
	switch (dev_id) {
	case 0x450:
		tc_printf(t, "STM32H742/743/753/750\n");

		/* Print revision */
		char rev = '?';
		for (size_t i = 0;
			 i < sizeof(stm32h7xx_revisions)/sizeof(struct stm32h7xx_rev); i++) {
			/* Check for matching revision */
			if (stm32h7xx_revisions[i].rev_id == rev_id) {
				rev = stm32h7xx_revisions[i].revision;
			}
		}
		tc_printf(t, "Revision %c\n", rev);
		break;

	case 0x480:
		tc_printf(t, "STM32H7B3/7A3/7B0\n");
		break;
	case 0x483:
		tc_printf(t, "STM32H723/733/725/735/730\n");
		break;
	default:
		tc_printf(t, "Unknown STM32H7. This driver may not support it!\n");
	}

	return true;
}
