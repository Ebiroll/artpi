(gdb) target remote /dev/ttyACM1
Remote debugging using /dev/ttyACM1
[
w +$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r $#00
w +
r $#00
w +
r $#00
w +
r <Timeout: 2 seconds>
w $qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r $#00
w +
r $#00
w +
r <Timeout: 2 seconds>
w $qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r $#00
w +
r $#00
w +
r <Timeout: 2 seconds>
w $qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a
r $#00
w +
r $#00
w +
r <Timeout: 2 seconds><Timeout: 2 seconds>
w -
r $#00
w +$vMustReplyEmpty#3a
r +$#00
w +$Hg0#df
r +$OK#9A
w +$qTStatus#49
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
w +$Hg0#df
r +$OK#9A
w +$g#67
r +$02000000e9ffffff00000840010000000200000002000000010000000000f81f0000000000000000000000000000000002000000f8fd012002000000801b0020797a0008#C7
w +$g#67
r +$02000000e9ffffff00000840010000000200000002000000010000000000f81fa822002000000000000000000000000002000000f8fd012002000000801b0020797a0008#06
w +$m80002ac,2#89
r +$9825#D8
w +$m80002ac,2#89
r +$9825#D8
w +$qfThreadInfo#bb
r +$m 1001,l#E7
w +$qsThreadInfo#c8
r +$#00
w +$qSymbol::#5b
r +$#00
w +$qTStatus#49
r +$T0;tnotrun:00#73
w +$qTfP#7b
r +$#00


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
w +$qTStatus#49
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
w +$qTStatus#49
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
w +$qTStatus#49
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
w +$qTStatus#49
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




