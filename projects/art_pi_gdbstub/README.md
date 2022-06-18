# Gdbstub
Work in progress.
The Idea was to access memory to make dump of the flash and use the gdb protocol.
Then I saw this program and got a bit carried away.

If it had worked you could dump memory with,
   (gdb) target remote /dev/ttyACM1

   (gdb) dump binary memory eflash.bin 0x90000000 0x90080000

   (gdb) dump binary memory boot_flash.bin 0x08000000 0x08001000


I would not trust the results yet, but the code can be consider educational.
More attention must be given to the
h7_mem_read() function.
It works well for 32-bits len=4 and aligned addresses 


https://github.com/ftrias/TeensyDebug


https://www-zeuthen.desy.de/dv/documentation/unixguide/infohtml/gdb/Thread-List-Format.html

As the project uses blackmagic codebase, some commands will just lock up the board.
blackmagic assumes you access the DP via JTAG or SWD and writes to the DAP (Debug access port).
Currently, generic cortexm target commands are used as a starting point of the project but will be replaced by the Debug monitor ISR commands.


# TIP: Disabling Halting Debug from GDB
If you are using GDB over ST-link, Halting Mode debug can be disabled manually by clearing C_DEBUGEN and setting the appropriate value for DBGKEY (0xA05F):

   (gdb) set *(uint32_t*)0xE000EDF0=(0xA05F<<16)

This can also be a useful way to keep an active GDB session open and probe its state while the system is running!

# More reasonable approach

Access the Debug Access Port (DAP) via JTAG or SWD.
https://interrupt.memfault.com/blog/a-deep-dive-into-arm-cortex-m-debug-interfaces
# Interesting article for x86

https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/

# Debug monitor.

https://interrupt.memfault.com/blog/cortex-m-debug-monitor

https://github.com/adamgreen/mri


# gdb protocol

Basic gdb-stub frame
``` 
Start with $ then single letter command more argumets # and checksum
If checksum is OK answer with + otherwise -
-> $q(extra)#cs
<- +
``` 


Interesting gdb commands
``` 
(gdb) info threads
(gdb) set debug remote 1
(gdb) set remotetimeout 30 
(gdb) set debug xtensa 1

(gdb) target remote:2345
(gdb) target remote 192.168.1.139:2345


(gdb) info threads
(gdb) monitor help
(gdb) set debug protocol 5
(gdb) set debug serial 5

Not so useful
(gdb) set debug target 1
``` 

``` 
st-link
(gdb) info threads
w $qL1200000000000000000#50
r +$#00
w +]  Id   Target Id         Frame 
* 1    Thread <main>     0x080063ba in HAL_GetTick () at Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c:340
``` 


``` 
(gdb) info threads
-> remote->log_command (...)
<- remote->log_command (info threads)
-> remote->update_thread_list (...)
-> remote->always_non_stop_p (...)
<- remote->always_non_stop_p () = false
-> remote->is_async_p (...)
<- remote->is_async_p () = false
[
w $qfThreadInfo#bb
r +$m 01,02,03,04,l#33
w +]-> remote->always_non_stop_p (...)
<- remote->always_non_stop_p () = false
-> remote->is_async_p (...)
<- remote->is_async_p () = false
[$qsThreadInfo#c8
r +$#00
w +]<- remote->update_thread_list ()
No threads.
``` 



``` 
Virtual com port
[21844.504150] usb 2-1.6: USB disconnect, device number 22
[21847.237349] usb 2-1.6: new full-speed USB device number 23 using ehci-pci
[21847.318563] usb 2-1.6: New USB device found, idVendor=0483, idProduct=5740, bcdDevice= 2.00
[21847.318575] usb 2-1.6: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[21847.318581] usb 2-1.6: Product: STM32 Virtual ComPort
[21847.318586] usb 2-1.6: Manufacturer: STMicroelectronics
[21847.318590] usb 2-1.6: SerialNumber: 356C375A3030
[21847.319453] cdc_acm 2-1.6:1.0: ttyACM1: USB ACM device
``` 

