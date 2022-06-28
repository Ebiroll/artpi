Debug load build/artpi_gdbstub_boot.elf


w $][v][F][l][a][s][h][E][r][a][s][e][:][0][8][0][0][0][0][0][0][,][0][0][0][2][0][0][0][0][#][c][4][
r +]





(gdb) target remote /dev/ttyACM1
Remote debugging using /dev/ttyACM1
[
w +$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r +$PacketSize=5DC;qXfer:memory-map:read+;qXfer:features:read+;#82
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qXfer:features:read:target.xml:0,5d7#1b
r +$m<?xml version="1.0"?><!DOCTYPE target SYSTEM "gdb-target.dtd"><target>  <architecture>arm</architecture>  <feature name="org.gnu.gdb.arm.m-profile">    <reg name="r0" bitsize="32"/>    <reg name="r1" bitsize="32"/>    <reg name="r2" bitsize="32"/>    <reg name="r3" bitsize="32"/>    <reg name="r4" bitsize="32"/>    <reg name="r5" bitsize="32"/>    <reg name="r6" bitsize="32"/>    <reg name="r7" bitsize="32"/>    <reg name="r8" bitsize="32"/>    <reg name="r9" bitsize="32"/>    <reg name="r10" bitsize="32"/>    <reg name="r11" bitsize="32"/>    <reg name="r12" bitsize="32"/>    <reg name="sp" bitsize="32" type="data_ptr"/>    <reg name="lr" bitsize="32" type="code_ptr"/>    <reg name="pc" bitsize="32" type="code_ptr"/>    <reg name="xpsr" bitsize="32"/>    <reg name="msp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="psp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="primask" bitsize="8" save-restore="no"/>    <reg name="basepri" bitsize="8" save-restore="no"/>    <reg name="faultmask" bitsize="8" save-restore="no"/>    <reg name="control" bitsize="8" save-restore="no"/>  </feature></target>#A5
w +$qXfer:features:read:target.xml:475,5d7#8b
r +$l#6C
w +$qTStatus#49
r +$#00
w +$?#3f
r +$T05#B9
w +$qfThreadInfo#bb
r +$#00
w +$qL1200000000000000000#50
r +$#00
w +$Hc-1#09
r +$OK#9A
w +$qC#b4
r +$#00
w +$qAttached#8f
r +$1#31
w +$qOffsets#4b
r +$#00
w +$g#67
r +$582d0020083000204830002020000000ccff012000000000e801002058c10008000000000000000000000000000000000000000048ff01209d22000820070008#98
w +$qXfer:memory-map:read::0,5d7#ba
r +$m<?xml version="1.0"?><!DOCTYPE memory-map PUBLIC "+//IDN gnu.org//DTD GDB Memory Map V1.0//EN"     "http://sourceware.org/gdb/gdb-memory-map.dtd"><memory-map>  <memory type="rom" start="0x00000000" length="0x10000"/>  <memory type="ram" start="0x20000000" length="0x20000"/>  <memory type="ram" start="0x24000000" length="0x80000"/>  <memory type="ram" start="0x30000000" length="0x48000"/>   <memory type="ram" start="0x38000000" length="0x10000"/>    <memory type="flash" start="0x08000000" length="0x20000">     <property name="blocksize">0x20000</property>    </memory>    <memory type="ram" start="0x40000000" length="0x1fffffff"/>   <memory type="ram" start="0xe0000000" length="0x1fffffff"/>   <memory type="rom" start="0x1ff00000" length="0x20000"/></memory-map>#FF
w +$qXfer:memory-map:read::302,5d7#1f
r +$l#6C
w +$m80002ac,2#89
r +$b0e8#2F
w +$m80002ac,4#8b
r +$b0e8f05f#60
w +$qL1200000000000000000#50
r +$#00
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m800071c,4#60
r +$25ff10df#5E
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m800071c,4#60
r +$25ff10df#5E
w +$m8000720,2#2c
r +$00bf#28
w +$m800071e,2#60
r +$10df#2B
w +$m800071c,2#5e
r +$25ff#33
w +$m8000720,2#2c
r +$00bf#28
w +$m800071e,2#60
r +$10df#2B
w +$m800071c,2#5e
r +$25ff#33
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m800071c,4#60
r +$25ff10df#5E
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m800071c,4#60
r +$25ff10df#5E
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m8000720,4#2e
r +$00bf2a46#25
w +$m8000720,4#2e
r +$00bf2a46#25
w +]0x08000720 in main () at Core/Src/main.c:216
warning: Source file is more recent than executable.
216	  asm volatile(
[$qSymbol::#5b
r +$#00
w +](gdb) where
[$m2001ff80,40#f4
r +$add8010000000000000000005f6b00080500000000000000b9d60100f2000000e801002058c10008000000003f0700083234320d0a00166e456e746572206764#D5
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,2#fb
r +$ad7a#5D
w +$m0,2#fb
r +$ad7a#5D
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,4#fd
r +$ad7a370c#5A
w +$m0,4#fd
r +$ad7a370c#5A
w +$p10#d1
r +$#00
w +]Python Exception <class 'gdb.error'> Register 25 is not available: 
#0  0x08000720 in main () at Core/Src/main.c:216
(gdb) info registers
r0             0x20002d58          536882520
r1             0x20003008          536883208
r2             0x20003048          536883272
r3             0x20                32
r4             0x2001ffcc          537001932
r5             0x0                 0
r6             0x200001e8          536871400
r7             0x800c158           134267224
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0x0                 0
sp             0x2001ff48          0x2001ff48
lr             0x800229d           0x800229d <stm32h7_add_flash+76>
pc             0x8000720           0x8000720 <main+124>
xpsr           <unavailable>
msp            <unavailable>
psp            <unavailable>
primask        <unavailable>
basepri        <unavailable>
faultmask      <unavailable>
control        <unavailable>

(gdb) b 228
(gdb) b 228
[
w $m8000680,40#63
r +$4023059306934ff480620792089304210deb010004f0ecfa1db030bd0048025800040058f0b58fb005f0d8fdfff7a0fffff7c6fefff776ff00f05af906f0c8fb#7B
w +$m8000700,40#5c
r +$0fcf0fc497e8030044f8040b21700d23152202a9304606f0c6f900f025ff10df00bf2a461149684609f092fd6846fff7cffd052382b269460a4806f0b4f90135#A2
w +$m800073e,2#62
r +$0135#C9
w +$m800073e,2#62
r +$0135#C9
w +]Breakpoint 1 at 0x800073e: file Core/Src/main.c, line 228.
(gdb) c
Continuing.
Note: automatically using hardware breakpoints for read-only addresses.
[$m800073e,2#62
r +$0135#C9
w +$Z1,800073e,2#ac
r +$#00
w +]Warning:
Cannot insert hardware breakpoint 1.
Could not insert hardware breakpoints:
You may have requested too many hardware breakpoints/watchpoints.

Command aborted.



On real hardware,
(gdb) 

r0             0x1775              6005
r1             0x40                64
r2             0x6000d0            6291664
r3             0x1f4               500
r4             0x1581              5505
r5             0xc                 12
r6             0x200001e8          536871400
r7             0x800c158           134267224
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0xa                 10
sp             0x2001ffb0          0x2001ffb0
lr             0x800074b           134219595
pc             0x8000722           0x8000722 <main+126>
xpsr           0x61000000          1627389952
msp            0x2001ffb0          0x2001ffb0
psp            0x0                 0x0
control        0x4                 4 '\004'
faultmask      0x0                 0 '\000'
basepri        0x0                 0 '\000'
primask        0x0                 0 '\000'
fpscr          0x0                 0



========================= cur_target=NULL

w +$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r $<Timeout: 2 seconds>
w +
r $#00
w +
r <Timeout: 2 seconds>
w $qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r +$PacketSize=100;qXfer:memory-map:read+;qXfer:features:read+;qXfer:threads:read+#83
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qXfer:features:read:target.xml:0,fb#13
r +$E01#A6
w +$#49
r +$T0;tnotrun:00#73
w +$qTfV#81
r +$l#6C
w +$?#3f
r +$W00#B7

=========================

w $qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r +$PacketSize=100;qXfer:memory-map:read+;qXfer:features:read+;qXfer:threads:read+#83
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qXfer:features:read:target.xml:0,fb#13
r +$l#6C


=================================

w +$qXfer:features:read:target.xml:0,fb#13
r +$m<?xml version="1.0"?><!DOCTYPE target SYSTEM "gdb-target.dtd"><target>  <architecture>arm</architecture>  <feature name="org.gnu.gdb.arm.m-profile">    <reg name="r0" bitsize="32"/>    <reg name="r1" bitsize="32"/>    <reg name="r2" bitsize="32"/>    #3]


==================================

[+$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r +$PacketSize=5DC;qXfer:memory-map:read+;qXfer:features:read+;qXfer:threads:read+#AE
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qXfer:features:read:target.xml:0,5d7#1b
r +$m<?xml version="1.0"?><!DOCTYPE target SYSTEM "gdb-target.dtd"><target>  <architecture>arm</architecture>  <feature name="org.gnu.gdb.arm.m-profile">    <reg name="r0" bitsize="32"/>    <reg name="r1" bitsize="32"/>    <reg name="r2" bitsize="32"/>    <reg name="r3" bitsize="32"/>    <reg name="r4" bitsize="32"/>    <reg name="r5" bitsize="32"/>    <reg name="r6" bitsize="32"/>    <reg name="r7" bitsize="32"/>    <reg name="r8" bitsize="32"/>    <reg name="r9" bitsize="32"/>    <reg name="r10" bitsize="32"/>    <reg name="r11" bitsize="32"/>    <reg name="r12" bitsize="32"/>    <reg name="sp" bitsize="32" type="data_ptr"/>    <reg name="lr" bitsize="32" type="code_ptr"/>    <reg name="pc" bitsize="32" type="code_ptr"/>    <reg name="xpsr" bitsize="32"/>    <reg name="msp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="psp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="primask" bitsize="8" save-restore="no"/>    <reg name="basepri" bitsize="8" save-restore="no"/>    <reg name="faultmask" bitsize="8" save-restore="no"/>    <reg name="control" bitsize="8" save-restore="no"/>  </feature></target>#A5
w +$qXfer:features:read:target.xml:475,5d7#8b
r +$l#6C
w +$#49
r +$T0;tnotrun:00#73
w +$qTfV#81
r +$l#6C
w +$?#3f
r +$T05#B9
w +$qXfer:threads:read::0,5d7#a1
r $</threads>#94
w +
r 001" core="0" <Timeout: 2 seconds>
w $qXfer:threads:read::0,5d7#a1
r $</threads>#94
w +
r 001" core="0" <Timeout: 2 seconds>
w $qXfer:threads:read::0,5d7#a1
r $</threads>#94
w +
r 001" core="0" ]^CQuit


===========================================

w +$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r +$PacketSize=5DC;qXfer:memory-map:read+;qXfer:features:read+;#82
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qXfer:features:read:target.xml:0,5d7#1b
r +$m<?xml version="1.0"?><!DOCTYPE target SYSTEM "gdb-target.dtd"><target>  <architecture>arm</architecture>  <feature name="org.gnu.gdb.arm.m-profile">    <reg name="r0" bitsize="32"/>    <reg name="r1" bitsize="32"/>    <reg name="r2" bitsize="32"/>    <reg name="r3" bitsize="32"/>    <reg name="r4" bitsize="32"/>    <reg name="r5" bitsize="32"/>    <reg name="r6" bitsize="32"/>    <reg name="r7" bitsize="32"/>    <reg name="r8" bitsize="32"/>    <reg name="r9" bitsize="32"/>    <reg name="r10" bitsize="32"/>    <reg name="r11" bitsize="32"/>    <reg name="r12" bitsize="32"/>    <reg name="sp" bitsize="32" type="data_ptr"/>    <reg name="lr" bitsize="32" type="code_ptr"/>    <reg name="pc" bitsize="32" type="code_ptr"/>    <reg name="xpsr" bitsize="32"/>    <reg name="msp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="psp" bitsize="32" save-restore="no" type="data_ptr"/>    <reg name="primask" bitsize="8" save-restore="no"/>    <reg name="basepri" bitsize="8" save-restore="no"/>    <reg name="faultmask" bitsize="8" save-restore="no"/>    <reg name="control" bitsize="8" save-restore="no"/>  </feature></target>#A5
w +$qXfer:features:read:target.xml:475,5d7#8b
r +$l#6C
w +$#49
r +$T0;tnotrun:00#73
w +$qTfV#81
r +$l#6C
w +$?#3f
r +$T05#B9
w +$qfThreadInfo#bb
r +$m 1001,l#E7
w +$qsThreadInfo#c8
r +$#00
w +$Hc-1#09
r +$OK#9A
w +$qC#b4
r +$#00
w +$qAttached#8f
r +$1#31
w +$qOffsets#4b
r +$#00
w +$g#67
r +$3f000000e9ffffffcd130000400000008e1300000b000000e801002088c1000800000000000000000000000000000000d000600040ff01203f0000000a00000047070008#DA
+$010000000000000002000000dc070020020000000000000000000840000000000000000000000000000000000000000098ff012058ff0120bf620008c0620008#38
w +$m80002ac,2#89
r +$b0e8#2F
w +$m80002ac,4#8b
r +$b0e8f05f#60
w +$qfThreadInfo#bb
r +$m 1001,l#E7
w +$qsThreadInfo#c8
r +$#00
w +$qSymbol::#5b
r +$#00
w +$#49
r +$T0;tnotrun:00#73
w +$qTfP#7b
r +$#00
w +](gdb) where
The current thread has terminated
(gdb) info threads
[$qfThreadInfo#bb
r +$m 1001,l#E7
w +$qsThreadInfo#c8
r +$#00




