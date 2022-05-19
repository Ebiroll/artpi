# Gdbstub
Work in progress.
The Idea was to access memory to make dump of the flash

https://github.com/ftrias/TeensyDebug


https://www-zeuthen.desy.de/dv/documentation/unixguide/infohtml/gdb/Thread-List-Format.html


For the future, another UART can be used when running programs from the external flash.
This way we do not interfer with the debug UART4.


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

Not so useful
(gdb) set debug target 1
``` 
