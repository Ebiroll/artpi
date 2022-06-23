(gdb) set debug serial 5
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
r +$01000000e9ffffffca86000040000000c986000046000000e8010020a4c1000800000000000000000000000000000000d000600040ff0120010000000a000000#DD
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
w +$ma,4#2e
r +$089cef3c#65
w +$m6,4#03
r +$475e25f1#03
w +$ma,4#2e
r +$089cef3c#65
w +$m6,4#03
r +$475e25f1#03
w +$ma,2#2c
r +$089c#04
w +$m8,2#03
r +$25f1#FE
w +$m6,2#01
r +$475e#05
w +$ma,2#2c
r +$089c#04
w +$m8,2#03
r +$25f1#FE
w +$m6,2#01
r +$475e#05
w +$ma,4#2e
r +$089cef3c#65
w +$m6,4#03
r +$475e25f1#03
w +$ma,4#2e
r +$089cef3c#65
w +$m6,4#03
r +$475e25f1#03
w +$ma,4#2e
r +$089cef3c#65
w +$ma,4#2e
r +$089cef3c#65
w +$ma,4#2e
r +$089cef3c#65
w +$p10#d1
r +$#00
w +]0x0000000a in ?? ()
[$qSymbol::#5b
r +$#00
w +](gdb) where
[$m2001ff40,40#f0
r +$d8010000e9ffffff4198000040000000d0006000d80100000a000000470700081c63000800000081000000000000000069960000e8010020b4ff012005000000#F1
w +$mffffffe9,4#cf
r <Timeout: 2 seconds>
w $mffffffe9,4#cf
r <Timeout: 2 seconds>
w $mffffffe9,4#cf
r <Timeout: 2 seconds>
w $mffffffe9,4#cf
r <Timeout: 2 seconds><Timeout: 2 seconds>
w -
r <Timeout: 2 seconds>
w -]^C[
r <Timeout: 2 seconds>
w -]Ignoring packet error, continuing...
[+]Error occurred computing Python errormessage.
Python Exception <class 'KeyboardInterrupt'> <class 'KeyboardInterrupt'>: 
warning: internal error: Unhandled Python exception
#0  0x0000000a in ?? ()
(gdb) Quit
(gdb) info regs
Undefined info command: "regs".  Try "help info".
(gdb) info registers
r0             0x1                 1
r1             0xffffffe9          -23
r2             0x86ca              34506
r3             0x40                64
r4             0x86c9              34505
r5             0x46                70
r6             0x200001e8          536871400
r7             0x800c1a4           134267300
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0x6000d0            6291664
sp             0x2001ff40          0x2001ff40
lr             0x1                 0x1
pc             0xa                 0xa
xpsr           <unavailable>
msp            <unavailable>
psp            <unavailable>
primask        <unavailable>
basepri        <unavailable>
faultmask      <unavailable>
control        <unavailable>
(gdb) 

On real target

info registers
r0             0x20002d58          536882520
r1             0x2001fe88          537001608
r2             0xffffffe9          -23
r3             0x4                 4
r4             0x20002d58          536882520
r5             0x8001b4d           134224717
r6             0x20001610          536876560
r7             0x2001fe90          537001616
r8             0x1                 1
r9             0x1                 1
r10            0x1                 1
r11            0x1                 1
r12            0xffffff9f          -97
sp             0x2001fe50          0x2001fe50
lr             0xfffffff1          -15
pc             0x80023bc           0x80023bc <HardFault_Handler>
xpsr           0x61000003          1627389955
msp            0x2001fe50          0x2001fe50
psp            0x0                 0x0
control        0x0                 0 '\000'
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




