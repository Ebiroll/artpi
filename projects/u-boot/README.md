

# u-boot
Running u-boot on the ART-Pi board and in qemu.

# building


   ARCH=arm CROSS_COMPILE=arm-none-eabi- make stm32h750-art-pi_defconfig
   make menuconfig

  (0xc1800000) Address in memory to use by default
  0xC0000000 is the base memory adress for the SD-RAM


  ARCH=arm CROSS_COMPILE=arm-none-eabi- make 



# Starting,

You can use the gdb_stub project as bootloader


# Start qemu

 build/arm-softmmu/qemu-system-arm -M artpi -d 'unimp' -bios ../artpi/projects/art_pi_gdbstub/board/CubeMX_Config/
build/artpi_gdbstub_boot.bin  -s  -S  -serial tcp::5551,server,nowait   -serial tcp::5552,server,nowait  -serial tcp::5553,server,nowait  -serial tcp::555
4,server,nowait    -serial tcp::5555,server,nowait

 gdb-multiarch u-boot  -ex 'target remote:1234'

 (gdb) restore u-boot.bin binary 0x90000000 0

Restoring binary file u-boot.bin into memory (0x90000000 to 0x9004857c)


Start netcat on UART4 
  nc localhost 5554

'''
U-Boot 2021.04-rc3-00160-g3727ffa (Mar 22 2021 - 11:50:15 +0800)

Model: RT-Thread STM32H750i-Art-Pi board
DRAM:  32 MiB
MMC:   STM32 SD/MMC: 0
In:    serial@40004c00
Out:   serial@40004c00
Err:   serial@40004c00
Hit SPACE in 3 seconds to stop autoboot.
Wrong Image Format for bootm command
ERROR: can't get kernel image!
'''

'''
U-Boot 2022.07-rc5-00017-gbfa9306e14 (Jun 29 2022 - 10:19:09 +0200)

Model: RT-Thread STM32H750i-ART-PI board
DRAM:  32 MiB
Core:  31 devices, 16 uclasses, devicetree: separate
MMC:   STM32 SD/MMC: 0
Loading Environment from nowhere... OK
In:    serial@40004c00
Out:   serial@40004c00
Err:   serial@40004c00
Hit SPACE in 3 seconds to stop autoboot.
Wrong Image Format for bootm command
ERROR: can't get kernel image!

U-Boot > ?
?
?         - alias for 'help'
base      - print or set address offset
bdinfo    - print Board Info structure
blkcache  - block cache diagnostics and control
boot      - boot default, i.e., run 'bootcmd'
bootd     - boot default, i.e., run 'bootcmd'
bootelf   - Boot from an ELF image in memory
bootflow  - Boot flows
bootm     - boot application image from memory
bootvx    - Boot vxWorks from an ELF image
bootz     - boot Linux zImage image from memory
cmp       - memory compare
coninfo   - print console devices and information
cp        - memory copy
crc32     - checksum calculation
dcache    - enable or disable data cache
dm        - Driver model low level access
echo      - echo args to console
editenv   - edit environment variable
env       - environment handling commands
exit      - exit script
ext2load  - load binary file from a Ext2 filesystem
ext2ls    - list files in a directory (default /)
ext4load  - load binary file from a Ext4 filesystem
ext4ls    - list files in a directory (default /)
ext4size  - determine a file's size
ext4write - create a file in the root directory
false     - do nothing, unsuccessfully
fatinfo   - print information about filesystem
fatload   - load binary file from a dos filesystem
fatls     - list files in a directory (default /)
fatsize   - determine a file's size
fdt       - flattened device tree utility commands
fstype    - Look up a filesystem type
fstypes   - List supported filesystem types
go        - start application at address 'addr'
gpt       - GUID Partition Table
help      - print command description/usage
icache    - enable or disable instruction cache
iminfo    - print header information for application image
imxtract  - extract a part of a multi-image
itest     - return true/false on integer compare
ln        - Create a symbolic link
load      - load binary file from a filesystem
loadb     - load binary file over serial line (kermit mode)
loads     - load S-Record file over serial line
loadx     - load binary file over serial line (xmodem mode)
loady     - load binary file over serial line (ymodem mode)
loop      - infinite loop on address range
ls        - list files in a directory (default /)
md        - memory display
mm        - memory modify (auto-incrementing address)
mmc       - MMC sub system
mmcinfo   - display MMC info
mw        - memory write (fill)
nm        - memory modify (constant address)
panic     - Panic with optional message
part      - disk partition related commands
pinmux    - show pin-controller muxing
printenv  - print environment variables
random    - fill memory with random pattern
reset     - Perform RESET of the CPU
run       - run commands in an environment variable
save      - save file to a filesystem
setenv    - set environment variables
showvar   - print local hushshell variables
size      - determine a file's size
sleep     - delay execution for some time
source    - run script from memory
sysboot   - command to get and boot from syslinux files
test      - minimal test like /bin/sh
timer     - access the system timer
true      - do nothing, successfully
version   - print monitor, compiler and linker version

U-Boot > bdinfo
bdinfo
boot_params = 0x00000000
DRAM bank   = 0x00000000
-> start    = 0xc0000000
-> size     = 0x02000000
flashstart  = 0x00000000
flashsize   = 0x00000000
flashoffset = 0x00000000
baudrate    = 2000000 bps
relocaddr   = 0xc1fb2000
reloc off   = 0x31fb2000
Build       = 32-bit
fdt_blob    = 0xc1eaa0e0
new_fdt     = 0xc1eaa0e0
fdt_size    = 0x00005dc0
lmb_dump_all:
 memory.cnt  = 0x1
 memory[0]      [0xc0000000-0xc1ffffff], 0x02000000 bytes flags: 0
 reserved.cnt  = 0x1
 reserved[0]    [0xc1ea5eb0-0xc1ffffff], 0x0015a150 bytes flags: 0
devicetree  = separate
arch_number = 0x00000000
TLB addr    = 0xc1ff0000
irq_sp      = 0xc1eaa0d0
sp start    = 0xc1eaa0c0
Early malloc usage: ccc / f00
U-Boot >

'''


## artpi memory map

Internal on the stm32h750 chip

Flash 1 
 0x08000000,LENGTH = 0x100000 (8192k/2)
Flash 2 
 0x8100000, LENGTH = 0x100000 (8192k/2)

	stm32h7_add_flash(t, 0x8000000, 0x100000, FLASH_SECTOR_SIZE);


	target_add_ram(t, 0x00000000, 0x10000); /* ITCM Ram,  64 k */
	target_add_ram(t, 0x20000000, 0x20000); /* DTCM Ram, 128 k */
	target_add_ram(t, 0x24000000, 0x80000); /* AXI Ram,  512 k */
	target_add_ram(t, 0x30000000, 0x20000); /* AHB SRAM1, 128 k */
	target_add_ram(t, 0x30020000, 0x20000); /* AHB SRAM2, 128 k */
	target_add_ram(t, 0x30040000, 0x08000); /* AHB SRAM3,  32 k */
	target_add_ram(t, 0x38000000, 0x10000); /* AHB SRAM4,  64 k */

## same info but different

 DTCMRAM:         128 KB 
 RAM_D1:          512 KB 
 RAM_D2:          288 KB 
 RAM_D3:          64 KB  
 ITCMRAM:         64 KB  


   8MiB QSPI flash (Boot flash)
   16MiB SPI flash (W25Q64)              ORIGIN =0x90000000,LENGTH =8192k
   32MiB SDRAM                           ORIGIN =0xC0000000,LENGTH =32M



- Debug serial port to serial port 4 mapping instructions

    PI9 ------> USART4_RX

    PA0 ------> USART4_TX


   On the art-pi board uart4 is connected to the Stlink

## Some notes from running u-boot


When looking at the sources and reverse engineering the u-boot binary in
/projects/art_pi_kboot/demo
It seems like the Device tree blob should be located at,
   fdt_blob = 0x9004_0044

Other boot-time parameters,

   kernel_addr_r=0xC000_8000
   fdtfile=stm32h750i-art-pi.dtb
   fdt_addr_r=0xC0408000
   scriptaddr=0xC0418000
   pxefile_addr_r=0xC0428000
   ramdisk_addr_r=0xC0438000
   boot_scripts=boot.scr.uimg boot.scr

global_data is a structure used by u-boot.
   struct global_data *gd_ptr;
   r9 + 0x70


   (gdb) p *gd_ptr             
    $13 = {bd = 0x0, flags = 0, baudrate = 0, cpu_clk = 0, bus_clk = 0, pci_clk = 0, mem_clk = 0, have_console = 0,
    env_addr = 0,	env_valid = 0, env_has_init = 0, env_load_prio = 0, ram_base = 0, ram_top = 0, relocaddr = 0,
    ram_size = 0,	mon_len = 241896, irq_sp = 0, start_addr_sp = 0, reloc_off = 0,	new_gd = 0x0, dm_root = 0x0,
    dm_root_f = 0x0, uclass_root_s = {next = 0x0,	prev = 0x0}, uclass_root = 0x0,	timer = 0x0, 
    fdt_blob = 0x90040044, new_fdt = 0x0,	fdt_size = 0, fdt_src = FDTSRC_SEPARATE, jt = 0x0,
    env_buf = '\000' * 31 , timebase_h = 0, timebase_l = 0, malloc_base = 604238080,	malloc_limit = 0,
    malloc_ptr = 0, cur_serial_dev = 0x0, arch = {timer_rate_hz = 0, tbu = 0, tbl = 0, lastinc = 0,
     timer_reset_value = 0, tlb_addr = 0, tlb_size = 0}, dmtag_list = {next = 0x0, prev = 0x0}}

 p/x *gd
$9 = {bd = 0x0, flags = 0x0, baudrate = 0x1e8480, cpu_clk = 0x0, bus_clk = 0x0, pci_clk = 0x0, mem_clk = 0x0, have_console = 0x0, env_addr = 0x9002a004, env_valid = 0x1,
   env_has_init = 0x1000, env_load_prio = 0x0, ram_base = 0x0, ram_top = 0x0, relocaddr = 0x0, ram_size = 0x0, mon_len = 0x39d68, irq_sp = 0x0, start_addr_sp = 0x0, reloc_off = 0x0,
   new_gd = 0x0, dm_root = 0x2403f118, dm_root_f = 0x0, uclass_root_s = {next = 0x2403f110, prev = 0x2403f110}, uclass_root = 0x2403f07c, timer = 0x0, fdt_blob = 0x9003ea7c, 
   new_fdt = 0x0, fdt_size = 0x0, fdt_src = 0x0, jt = 0x0, env_buf = {0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0 <repeats 25 times>}, timebase_h = 0x0, timebase_l = 0x0,
   malloc_base = 0x2403f100, malloc_limit = 0xf00, malloc_ptr = 0x6c, cur_serial_dev = 0x0, arch = {timer_rate_hz = 0x0, tbu = 0x0, tbl = 0x0, lastinc = 0x0, timer_reset_value = 0x0,
     tlb_addr = 0x0, tlb_size = 0x0}, dmtag_list = {next = 0x2403f0f8, prev = 0x2403f0f8}}



When using qemu we can load u-boot and kernel

   (gdb) restore org-u-boot binary 0x90000000 0 275089
   (gdb) restore u-boot.bin binary 0x90000000 0 
   (gdb) restore art.itb binary 0x90080000 0 
   (gdb) restore u-boot-dtb.bin binary 0x90040044 0

   > If you compiled u-boot manually
   restore u-boot.bin binary 0x90000000 0
   restore art.itb binary 0x90080000 0
   restore u-boot-dtb.bin binary 0x90040044 0


   (gdb) p/x *0x90000004
   (gdb) s	$pc=0x900003fd


     p init_sequence_f
     $1 =  <setup_mon_len>
      <fdtdec_setup>, 
      <initf_malloc>, 
      <log_init>,  
      <initf_bootstage>, 
      <event_init>, 
      <setup_spl_handoff>,
      <arch_cpu_init>, 
      <mach_cpu_init>, 
      <initf_dm>,
     <board_early_init_f>, 
      <timer_init>,
       <env_init>, 
      <init_baud_rate>,

      /* Runs up to here */
      <serial_init>,       serial_find_console_or_panic

       {driver = 0x90037c5c <_u_boot_list_2_driver_2_serial_stm32>, name = 0x90040604 "serial@40004c00", plat_ = 0x2403fa98, parent_plat_ = 0x0, uclass_plat_ = 0x0, driver_data = 2416144538,    parent = 0x2403f118, priv_ = 0x0, uclass = 0x2403fa2c, uclass_priv_ = 0x0, parent_priv_ = 0x0, uclass_node = {next = 0x2403fa34, prev = 0x2403fa34}, child_head = {next = 0x2403fa78, prev = 0x2403fa78},
  sibling_node = {next = 0x2403f14c, prev = 0x2403f9ec}, flags_ = 66, seq_ = 0, node_ = {np = 0x80c, of_offset = 2060}, dma_offset = 0}

       pinctrl_select_state

    configure_clocks()
      drivers/clk/clk_stm32h7.c

     /* set HPRE (/2) DI clk --> 125MHz */                                                                                          
     clrsetbits_le32(&regs->d1cfgr RCC_D1CFGR_HPRE_MASK,                                                                                                                         
                                RCC_D1CFGR_HPRE_DIV2)                                                                                                                                      
                                                                                                                                                                                           
         /*  select PLL1 as system clock source (sys_ck)*/                                                                                                                               
         clrsetbits_le32(&regs->cfgr, RCC_CFGR_SW_MASK, RCC_CFGR_SW_PLL1);                                                                
      while ((readl(&regs->cfgr) & RCC_CFGR_SW_MASK) != RCC_CFGR_SW_PLL1)
                        ;                                                                                                                                                                  

      s32h7xx_powermgt_read: Unknown offset 0x00000010
      stm32h7xx_powermgt_read: Unknown offset 0x00000010
      stm32h7xx_powermgt_read: Unknown offset 0x00000010
      stm32h7xx_powermgt_read: Unknown offset 0x00000010

                                                                                                                                                                                        ???
      /* sdram: use pll1_q as fmc_k clk */
      clrsetbits_le32(&regs->d1ccipr, RCC_D1CCIPR_FMCSRC_MASK,         FMCSRC_PLL1_Q_CK);                                                                                              




      <console_init_f>,
      <display_options>, 
      <display_text_info>, 
      <checkcpu>, 
      <show_board_info>,
      <misc_init_f>,
      <announce_dram_init>, 
      <dram_init>,
      <setup_dest_addr>, 
      <reserve_round_4k>, 
      <arch_reserve_mmu>, 
      <reserve_video>, 
      <reserve_trace>,
      <reserve_uboot>, 
      <reserve_malloc>,
      <reserve_board>, 
      <reserve_global_data>, 
      <reserve_fdt>, 
      <reserve_bootstage>, 
      <reserve_bloblist>,
      <reserve_arch>, 
      <reserve_stacks>, 
      <dram_init_banksize>, 
      <show_dram_config>, 
      <setup_bdinfo>, 
      <display_new_sp>, 
      <reloc_fdt>,
      <reloc_bootstage>, 
      <reloc_bloblist>, 
      <setup_reloc>, 
       <clear_bss>,


    u-boot Init call sequence

      board_init_f():
         - purpose: set up the machine ready for running board_init_r():
            i.e. SDRAM and serial UART
         - global_data is available
         - stack is in SRAM
         - BSS is not available, so you cannot use global/static variables,
            only stack variables and global_data

         Non-SPL-specific notes:
         - dram_init() is called to set up DRAM. If already done in SPL this
            can do nothing

         SPL-specific notes:
         - you can override the entire board_init_f() function with your own
            version as needed.
         - preloader_console_init() can be called here in extremis
         - should set up SDRAM, and anything needed to make the UART work
         - there is no need to clear BSS, it will be done by crt0.S
         - for specific scenarios on certain architectures an early BSS *can*
         be made available (via CONFIG_SPL_EARLY_BSS by moving the clearing
         of BSS prior to entering board_init_f()) but doing so is discouraged.
         Instead it is strongly recommended to architect any code changes
         or additions such to not depend on the availability of BSS during
         board_init_f() as indicated in other sections of this README to
         maintain compatibility and consistency across the entire code base.
         - must return normally from this function (don't call board_init_r()
            directly)

      Here the BSS is cleared. For SPL, if CONFIG_SPL_STACK_R is defined, then at
      this point the stack and global_data are relocated to below
      CONFIG_SPL_STACK_R_ADDR. For non-SPL, U-Boot is relocated to run at the top of
      memory.


    board_init_r():
      - purpose: main execution, common code
      - global_data is available
      - SDRAM is available
      - BSS is available, all static/global variables can be used
      - execution eventually continues to main_loop()

      Non-SPL-specific notes:
      - U-Boot is relocated to the top of memory and is now running from
         there.

      SPL-specific notes:
      - stack is optionally in SDRAM, if CONFIG_SPL_STACK_R is defined and
         CONFIG_SPL_STACK_R_ADDR points into SDRAM
      - preloader_console_init() can be called here - typically this is
         done by selecting CONFIG_SPL_BOARD_INIT and then supplying a
         spl_board_init() function containing this call
      - loads U-Boot or (in falcon mode) Linux



setup_mon_len()
fdtdec_setup () 
Set breakpoint here to see if OK
fdt_check_header()

/* Should not be weak! */
fdtdec_board_setup()

initf_malloc()
arch_cpu_init()

puts("No serial driver found")
                                                                                               

b puts()

Breakpoint 2, puts (s=0x2403edbc "pinctrl_stm32 pin-controller@58020000: hwspinlock_get_by_index may have failed (-38)\n") at common/console.c:691
0x2403ee5c "pinctrl_stm32 pin-controller@58020000: periph->name = serial@40004c00\n") at common/console.c:691
puts (s=0x2403ec2c "gpio_stm32 gpio@58020000: addr = 0x58020000 bank_name = GPIOA gpio_count = 16 gpio_range = 0xffff\n") at common/console.c:69



clk_get_rate(&clk)

stm32_get_PLL1_rate()
"Can't find clk clk-hsi (-19)") at common/console.c:691

stm32h7xx_powermgt_read: Unknown offset 0x000000e8
stm32h7xx_powermgt_write: Unknown offset 0x000000e8
stm32h7xx_powermgt_read: Unknown offset 0x00000028


                                                                                               
                                                                                               
                                                                                                                                                                                                      { 0x90000000, REGION_1, XN_DIS,  PRIV_RW_USR_RW,  SHARED_WRITE_BUFFERED, REGION_256MB }
           { 0xC0000000, REGION_0, XN_DIS,PRIV_RW_USR_RW,    O_I_WB_RD_WR_ALLOC, REGION_512MB }  


{start_addr = 0x90000000, region_no = 0x1, xn = 0x0, ap = 0x3, mr_attr = 0x1, reg_size = 0x1b}
{start_addr = 0xc0000000, region_no = 0x0, xn = 0x0, ap = 0x3, mr_attr = 0x5, reg_size = 0x1c}

dm_init_and_scan()


# Qemu u-boot log 

      U-Boot 2022.04-00992-g9bb99fa958 (May 17 2022 - 08:45:08 +0200)

      Model: RT-Thread STM32H750i-ART-PI board
      DRAM:  stm32_fmc fmc@52004000: can't find syscon device (-2)
      stm32_fmc fmc@52004000: no of banks = 1
      pinctrl_stm32 pin-controller@58020000: periph->name = fmc@52004000
      gpio_stm32 gpio@58020c00: addr = 0x58020c00 bank_name = GPIOD gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 33
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=33 gate offset=0xe0 bit_index=3 name=gpiod
      gpio_stm32 gpio@58020c00: clock enabled
      gpio_stm32 gpio@58021000: addr = 0x58021000 bank_name = GPIOE gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 32
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=32 gate offset=0xe0 bit_index=4 name=gpioe
      gpio_stm32 gpio@58021000: clock enabled
      gpio_stm32 gpio@58021400: addr = 0x58021400 bank_name = GPIOF gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 31
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=31 gate offset=0xe0 bit_index=5 name=gpiof
      gpio_stm32 gpio@58021400: clock enabled
      gpio_stm32 gpio@58021800: addr = 0x58021800 bank_name = GPIOG gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 30
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=30 gate offset=0xe0 bit_index=6 name=gpiog
      gpio_stm32 gpio@58021800: clock enabled
      gpio_stm32 gpio@58021c00: addr = 0x58021c00 bank_name = GPIOH gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 29
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=29 gate offset=0xe0 bit_index=7 name=gpioh
      gpio_stm32 gpio@58021c00: clock enabled
      gpio_stm32 gpio@58020800: addr = 0x58020800 bank_name = GPIOC gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 34
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=34 gate offset=0xe0 bit_index=2 name=gpioc
      gpio_stm32 gpio@58020800: clock enabled
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 64
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=64 gate offset=0xd4 bit_index=12 name=fmc
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 45
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: source = 3 freq = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id=45 gate_offset=0xe8 sysclk=6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: freq after APB1 prescaler = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 45
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=45 gate offset=0xe8 bit_index=3 name=tim5
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: source = 3 freq = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id=45 gate_offset=0xe8 sysclk=6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: freq after APB1 prescaler = 6250000
      32 MiB
      stm32-rcc reset-clock-controller@58024400: RCC bind
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020800
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020800
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020c00
      pinctrl_stm32 pin-controller@58020000: bind gpio@58020c00
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021800
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021800
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021c00
      pinctrl_stm32 pin-controller@58020000: bind gpio@58021c00
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022000
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022400
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022800
      pinctrl_stm32 pin-controller@58020000: bind gpio@58022800
      pinctrl_stm32 pin-controller@58020000: bind i2c1-0
      pinctrl_stm32 pin-controller@58020000: bind rmii-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc1-b4-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc1-b4-od-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc1-b4-sleep-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc1-dir-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc1-dir-sleep-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc2-b4-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc2-b4-od-0
      pinctrl_stm32 pin-controller@58020000: bind sdmmc2-b4-sleep-0
      pinctrl_stm32 pin-controller@58020000: bind spi1-0
      pinctrl_stm32 pin-controller@58020000: bind uart4-0
      pinctrl_stm32 pin-controller@58020000: bind usart1-0
      pinctrl_stm32 pin-controller@58020000: bind usart2-0
      pinctrl_stm32 pin-controller@58020000: bind usart3-0
      pinctrl_stm32 pin-controller@58020000: bind usbotg-hs-0
      pinctrl_stm32 pin-controller@58020000: bind fmc@0
      pinctrl_stm32 pin-controller@58020000: hwspinlock_get_by_index may have failed (-38)
      pinctrl_stm32 pin-controller@58020000: periph->name = serial@40004c00
      gpio_stm32 gpio@58020000: addr = 0x58020000 bank_name = GPIOA gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 36
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=36 gate offset=0xe0 bit_index=0 name=gpioa
      gpio_stm32 gpio@58020000: clock enabled
      gpio_stm32 gpio@58022000: addr = 0x58022000 bank_name = GPIOI gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 28
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=28 gate offset=0xe0 bit_index=8 name=gpioi
      gpio_stm32 gpio@58022000: clock enabled
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 80
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=80 gate offset=0xe8 bit_index=19 name=uart4
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: source = 3 freq = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id=80 gate_offset=0xe8 sysclk=6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: freq after APB1 prescaler = 6250000
      Core:  28 devices, 13 uclasses, devicetree: separate
      MMC:   pinctrl_stm32 pin-controller@58020000: periph->name = sdmmc@52007000
      gpio_stm32 gpio@58020800: addr = 0x58020800 bank_name = GPIOC gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 34
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=34 gate offset=0xe0 bit_index=2 name=gpioc
      gpio_stm32 gpio@58020800: clock enabled
      gpio_stm32 gpio@58020c00: addr = 0x58020c00 bank_name = GPIOD gpio_count = 16 gpio_range = 0xffff
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 33
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=33 gate offset=0xe0 bit_index=3 name=gpiod
      gpio_stm32 gpio@58020c00: clock enabled
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 62
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=62 gate offset=0xd4 bit_index=16 name=sdmmc1
      stm32_rcc_reset reset-clock-controller@58024400: reset id = 1008 bank = 124 offset = 16)
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 45
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: source = 3 freq = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id=45 gate_offset=0xe8 sysclk=6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: freq after APB1 prescaler = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id 45
      stm32h7_rcc_clock reset-clock-controller@58024400: clkid=45 gate offset=0xe8 bit_index=3 name=tim5
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: source = 3 freq = 6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: clk->id=45 gate_offset=0xe8 sysclk=6250000
      stm32h7_rcc_clock reset-clock-controller@58024400: system clock: freq after APB1 prescaler = 6250000
      stm32_rcc_reset reset-clock-controller@58024400: reset id = 1008 bank = 124 offset = 16)
      STM32 SD/MMC: 0
      Loading Environment from nowhere... OK
      In:    serial@40004c00
      Out:   serial@40004c00
      Err:   serial@40004c00
      Hit SPACE in 3 seconds to stop autoboot.
      ## Loading kernel from FIT Image at 90080000 ...
         Using 'conf-1' configuration
         Trying 'kernel' kernel subimage
         Description:  Vanilla Linux kernel
         Type:         Kernel Image
         Compression:  uncompressed
         Data Start:   0x900800e8
         Data Size:    4804648 Bytes = 4.6 MiB
         Architecture: ARM
         OS:           Linux
         Load Address: 0xc0008000
         Entry Point:  0xc0008000
         Hash algo:    crc32
         Hash value:   beeeab64
         Hash algo:    sha1
         Hash value:   b04fc82a06a6c386fa493a36cdd9b131f24c3a25
         Verifying Hash Integrity ... crc32+ sha1+ OK
      ## Loading fdt from FIT Image at 90080000 ...
         Using 'conf-1' configuration
         Trying 'fdt-1' fdt subimage
         Description:  Flattened Device Tree blob
         Type:         Flat Device Tree
         Compression:  uncompressed
         Data Start:   0x90515238
         Data Size:    16423 Bytes = 16 KiB
         Architecture: ARM
         Hash algo:    crc32
         Hash value:   7426cdac
         Hash algo:    sha1
         Hash value:   865934e6bed5f8e611e6d14f7226c628ab343fa7
         Verifying Hash Integrity ... crc32+ sha1+ OK
         Booting using the fdt blob at 0x90515238
         Loading Kernel Image
         Loading Device Tree to c17f8000, end c17ff026 ... OK




# Devices

   8MiB QSPI flash (Boot flash)
   16MiB SPI flash (W25Q64)              ORIGIN =0x90000000,LENGTH =8192k
   32MiB SDRAM
   AP6212 wifi,bt,fm comb


https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi


Most of the examples use the following adress mapping,
```

norflash0 | addr: 0x00000000 | len: 0x01000000 | blk_size: 0x00001000 |initialized finish.
norflash1 | addr: 0x00000000 | len: 0x00800000 | blk_size: 0x00001000 |initialized finish.

[I/FAL] ==================== FAL partition table ====================
[I/FAL] | name       | flash_dev |   offset   |    length  |
[I/FAL] -------------------------------------------------------------
[I/FAL] | wifi_image | norflash0 | 0x00000000 | 0x00080000 |
[I/FAL] | bt_image   | norflash0 | 0x00080000 | 0x00080000 |
[I/FAL] | download   | norflash0 | 0x00100000 | 0x00200000 |
[I/FAL] | easyflash  | norflash0 | 0x00300000 | 0x00100000 |
[I/FAL] | filesystem | norflash0 | 0x00400000 | 0x00a00000 |
[I/FAL] | factory    | norflash0 | 0x00e00000 | 0x00200000 |
[I/FAL] | app        | norflash1 | 0x00000000 | 0x00800000 |


Linker,
https://stackoverflow.com/questions/51323589/are-all-ram-sections-used-in-this-linkerscript-of-a-stm32h743-microcontroller-c

Internal memory,

 DTCMRAM:         128 KB 
 RAM_D1:          512 KB 
 RAM_D2:          288 KB 
 RAM_D3:          64 KB  
 ITCMRAM:         64 KB  


From this mapping

ROM (rx) : ORIGIN =0x90000000,LENGTH =8192k
RAM (rw) : ORIGIN =0x24000000,LENGTH =512k
RxDecripSection (rw) : ORIGIN =0x30040000,LENGTH =32k
TxDecripSection (rw) : ORIGIN =0x30040060,LENGTH =32k
RxArraySection (rw) : ORIGIN =0x30040200,LENGTH =32k
ROM (rx)    : ORIGIN =0x08000000,LENGTH =8192k

To this to run in qemu

QFLASH (rx) : ORIGIN =0x90000000,LENGTH =8192k
RAM (rw)    : ORIGIN =0x24000000,LENGTH =512k
ROM (rx) : ORIGIN =0x08000000,LENGTH =128k
RAM (rw) : ORIGIN =0x24000000,LENGTH =512k


   .section  .vectors
ENTRY(_start)
	.long	CONFIG_SYS_INIT_SP_ADDR		@ 0 - Reset stack pointer
	.long	reset				@ 1 - Reset
	.long	__invalid_entry			@ 2 - NMI
	.long	__hard_fault_entry		@ 3 - HardFault
	.long	__mm_fault_entry		@ 4 - MemManage
	.long	__bus_fault_entry		@ 5 - BusFault
	.long	__usage_fault_entry		@ 6 - UsageFault
	.long	__invalid_entry			@ 7 - Reserved
	.long	__invalid_entry			@ 8 - Reserved
	.long	__invalid_entry			@ 9 - Reserved
	.long	__invalid_entry			@ 10 - Reserved
	.long	__invalid_entry			@ 11 - SVCall
	.long	__invalid_entry			@ 12 - Debug Monitor
	.long	__invalid_entry			@ 13 - Reserved
	.long	__invalid_entry			@ 14 - PendSV
	.long	__invalid_entry			@ 15 - SysTick
	.rept	255 - 16
	.long	__invalid_entry			@ 16..255 - External Interrupts
	.endr


```



AP6212 is a low-power and high-performance WiFi+BT4.2 module launched by AMPAK. This module conforms to 802.11b/g/n. The WiFi function adopts SDIO interface, and the Bluetooth adopts UART/I2S/PCM interface, with StationMode, SoftAP, P2P function, etc. The hardware circuit connection of the chip is shown in the figure.
![WIFI-OTA](./figures/wifi_hardware.png)

(Broadcom BCM43438 A1 chip inside)  https://blog.quarkslab.com/reverse-engineering-broadcom-wireless-chipsets.html


# Linux for the st32m_h8
One of the few MMU-less arm boards supported in the kernel source tree

You can learn more about 
https://linux-kernel-labs.github.io/refs/heads/master/so2/lab11-arm-kernel-development.html



sw context:
- as stm32h750 just has 128k bytes internal flash, so running a fw on
  internal flash to download u-boot/kernel to qspi flash, boot
  u-boot/kernel from qspi flash. this fw is based on rt-thread.
- kernel can be xip on qspi flash or load to sdram
- root filesystem is jffs2(created by buildroot), stored on spi flash


to support the boad, add following changes.
- fix r0-r3, r12 register restore failed after svc call,
- add dts binding
- update yaml doc


## Build procedure

On an ubuntu LTS system

   apt install git make gcc device-tree-compiler bison flex libssl-dev libncurses-dev python3-ply python3-git libgmp3-dev libmpc-dev

   sudo apt-get install libc6-armel-cross gcc-arm-none-eabi libc6-dev-armel-cross binutils-arm-none-eabi libncurses5-dev build-essential bison flex libssl-dev bc

   sudo apt install gdb-multiarch

## U-boot

   git clone https://github.com/u-boot/u-boot



      * _main execution sequence is:
   
    1. Set up initial environment for calling board_init_f().
       This environment only provides a stack and a place to store
       the GD ('global data') structure, both located in some readily
       available RAM (SRAM, locked cache...). In this context, VARIABLE
       global data, initialized or not (BSS), are UNAVAILABLE; only
       CONSTANT initialized data are available. GD should be zeroed
       before board_init_f() is called.
     Note that register r9 points to gd.
   
    2. Call board_init_f(). This function prepares the hardware for
       execution from system RAM (DRAM, DDR...) As system RAM may not
       be available yet, , board_init_f() must use the current GD to
       store any data which must be passed on to later stages. These
       data include the relocation destination, the future stack, and
       the future GD location.
   
    3. Set up intermediate environment where the stack and GD are the
       ones allocated by board_init_f() in system RAM, but BSS and
       initialized non-const data are still not available.
   
    4a.For U-Boot proper (not SPL), call relocate_code(). This function
       relocates U-Boot from its current location into the relocation
       destination computed by board_init_f().
   
    4b.For SPL, board_init_f() just returns (to crt0). There is no
       code relocation in SPL.
   
    5. Set up final environment for calling board_init_r(). This
       environment has BSS (initialized to 0), initialized non-const
       data (initialized to their intended value), and stack in system
       RAM (for SPL moving the stack and GD into RAM is optional - see
       CONFIG_SPL_STACK_R). GD has retained values set by board_init_f().
   
    6. For U-Boot proper (not SPL), some CPUs have some work left to do
       at this point regarding memory, so call c_runtime_cpu_setup.
   
    7. Branch to board_init_r().
   
    For more information see 'Board Initialisation Flow in README.
   


   ARCH=arm CROSS_COMPILE=arm-none-eabi- make stm32h750-art-pi_defconfig

   ARCH=arm CROSS_COMPILE=arm-none-eabi- make 


   ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M  -bios ../artpi/libraries/qemu/bootloader.bin -nographic  -s -S

   ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M -kernel u-boot  -nographic  -s -S

   gdb --args ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M  -bios  bootloader.bin -nographic  -s -S -kernel u-boot

### Hardfloat use this toolchain

$ sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf # for arm32
$ sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu     # for arm64

### Linaro toolchain differences
   https://wiki-archive.linaro.org/WorkingGroups/ToolChain/FAQ#What_is_the_differences_between_.2BIBw-arm-none-eabi-.2BIB0_and_.2BIBw-arm-linux-gnueabihf.2BIB0.3F_Can_I_use_.2BIBw-arm-linux-gnueabihf.2BIB0_tool_chain_in_bare-metal_environment.3F_How_do_you_know_which_toolchain_binary_to_use_where.3F


   wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz

### Optional is to install Linaro arm cross compiler
   Optional

   wget http://releases.linaro.org/components/toolchain/gcc-linaro/7.5-2019.12/gcc-linaro-7.5-2019.12.tar.xz
   sudo tar xf gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf.tar.xz -C /opt

   git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

   sudo tar xf gcc-linaro-7.5-2019.12.tar.xz -C /opt


### Get kernel and compile

   git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

Or get latest version 

   wget ttps://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tag/?h=v5.18-rc4


    
# compile the kernel


    cd linux
    export ARCH=arm
    export CROSS_COMPILE=arm-linux-gnueabi-

    make stm32_defconfig
      then
    make menuconfig
      Add debug configuration and remove MMU support

    make
    make dtbs

    ls arch/arm/boot/dts/stm32h750*

    ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make -j8
    ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make -j8
    file vmlinux
    vmlinux: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), statically linked, BuildID[sha1]=304e52462758033e520112825260e36c8589e18b, with debug_info, not stripped


# Run qemu

   You need qemu 7 with patches to emulate the artpi board

   qemu-img create -f raw sd.bin 64M
   if [ ! -f "sd.bin" ]; then
     dd if=/dev/zero of=sd.bin bs=1024 count=65536
   fi

   qemu-system-arm -M artpi -kernel rtthread.elf -serial stdio -sd sd.bin -S -s



   Get a minimal qemu-arm filesystem


   wget http://downloads.yoctoproject.org/releases/yocto/yocto-2.5/machines/qemu/qemuarm/core-image-minimal-qemuarm.ext4


    Start qemu 

    ../qemu-7.0.0-rc4/build/arm-softmmu/qemu-system-arm -M artpi -cpu cortex-m7 -d 'in_asm,int,exec,cpu,guest_errors,unimp' -m 32M -kernel arch/arm/boot/zImage -nographic  -dtb arch/arm/boot/dts/stm32h750i-art-pi.dtb  -append "root=/dev/mmcblk0 rw console=ttymxc0 loglevel=8 earlycon printk" -serial stdio -sd core-image-minimal-qemuarm.ext4 -S


    Start debugger in other window
    file vmlinux


    killall -9 qemu-system-arm
   
    gdb-multiarch vmlinux
    (gdb) set arch arm
    (gdb) target remote:1234



Note that qemu does not use the device tree to create all devices, it just passes the device tree to the kernel



### Kernel tree art-pi highlights

    Documentation/arm/stm32/stm32h750-overview.rst
    Documentation/arm/index.rst                        |   1 +
    Documentation/arm/stm32/stm32h750-overview.rst     |  33 ++
    .../devicetree/bindings/arm/stm32/stm32.yaml       |   4 +
    .../bindings/pinctrl/st,stm32-pinctrl.yaml         |   1 +
    arch/arm/boot/dts/Makefile                         |   1 +
    arch/arm/boot/dts/stm32h7-pinctrl.dtsi             | 392 +++++++++++++++++++++
    arch/arm/boot/dts/stm32h743-pinctrl.dtsi           | 307 +---------------
    arch/arm/boot/dts/stm32h743.dtsi                   |  30 ++
    arch/arm/boot/dts/stm32h750-pinctrl.dtsi           |  11 +
    arch/arm/boot/dts/stm32h750.dtsi                   |   5 +
    arch/arm/boot/dts/stm32h750i-art-pi.dts            | 228 ++++++++++++
    arch/arm/mach-stm32/board-dt.c                     |   1 +
    drivers/pinctrl/stm32/Kconfig                      |   2 +-
    drivers/pinctrl/stm32/pinctrl-stm32h743.c          |   3 +
    Documentation/arm/stm32/stm32h750-overview.rst
    arch/arm/boot/dts/stm32h7-pinctrl.dtsi
    arch/arm/boot/dts/stm32h750-pinctrl.dtsi
    arch/arm/boot/dts/stm32h750.dtsi
    arch/arm/boot/dts/stm32h750i-art-pi.dts


###  Other

https://medium.com/@jan_75582/setup-arm-web-application-development-environment-with-qemu-for-stm32-500f5650a489


https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt

I do suggest that you try to include your rootfs in the zImage because you do not need to point to the disk, just the init. qemu-system-arm -M versatileab -m 128M -kernel zImage -append "rdinit=/bin/sh" (I think, its been a couple of months) ??? 
look at the section "What is initramfs?" here for information, but note that I had to point my kernel to the _install directory from busybox NOT the cpio file I created as instructed here. This is more of a what is the difference between ramDisk and initramfs than a correct how to. Here is another one


### Arm kernel options
https://www.kernel.org/doc/html/latest/arm/index.html

# Links

https://balau82.wordpress.com/2010/04/12/booting-linux-with-u-boot-on-qemu-arm/

# Bare metal projects
https://github.com/chcbaram/ART-Pi

https://wiki.st.com/stm32mpu/wiki/Debugging_the_Linux_kernel_using_the_GDB

https://www.st.com/resource/en/reference_manual/dm00314099-stm32h742-stm32h743-753-and-stm32h750-value-line-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf


https://www.emcraft.com/stm32f769i-discovery-board/installing-uboot-to-stm32f7-discovery-board


ART-Pi setup guide.
https://www.programmersought.com/article/60736526194/

QSPI support
https://wiki.segger.com/STM32H7#STM32H753xx.2F50xx.2F43xx_QSPI_support


# Somewhat related

https://hackaday.com/2021/05/19/raspberry-pi-rp2040-hands-on-experiences-from-an-stm32-perspective/

Cross compile
https://jensd.be/1126/linux/cross-compiling-for-arm-or-aarch64-on-debian-or-ubuntu

Interesting crazyflie
https://lup.lub.lu.se/luur/download?func=downloadFile&recordOId=9052405&fileOId=9052409

Gfx lib
https://support.touchgfx.com/4.19/docs/introduction/welcome

More qemu
https://www.youtube.com/watch?v=CdH-cad8DSw

Trusted platfom mode with qemu
https://www.linaro.org/blog/how-to-emulate-trusted-platform-module-in-qemu-with-u-boot/

Qemu for stm32
https://medium.com/@jan_75582/setup-arm-web-application-development-environment-with-qemu-for-stm32-500f5650a489


# qemu options
Not all are needed here, but is saved for reference

    -smp cores=4 -m 1024 -device sdhci-pci -device sd-card,drive=mydrive -drive id=mydrive,if=none,format=raw,file=image.bin


    -drive file=flash_image.bin,if=mtd,format=raw

    -global driver=timer.esp32.timg,property=wdt_disable,value=true

    -drive file=sd_image.bin,if=sd,format=raw


# Jump to built in bootloader on h7

In STM32H7, the base address of system memory is different from the entry point of the bootloader. 

Thus, in order to jump to the bootloader, address "0x1FF09800" should be used instead of "0x1FFF0000". 

Below, you find a sample code permitting the jump to the bootloader in STM32H7 devices:

```
void JumpToBootloader(void)
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

```


gdb-multiarch u-boot -ex 'target remote:1234'

(gdb) s $pc=0x90000f46+1

Possibility to run in qemu.
https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi/commit/244051251dddd944cccfa478a84e67e812cfbc6b

nvalid read at addr 0x9003EAE4, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 4, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFE, size 2, region '(null)', reason: rejected
Invalid read at addr 0xFFFFFFFC, size 2, region '(null)', reason: rejected

