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
