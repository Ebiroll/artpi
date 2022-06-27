
#include <stdint.h>

extern int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len);


void setNextBuffer() {
#ifdef UNIT_TEST
    static int ix=0;
    uint32_t len = 0;


    uint8_t Test2[] = "$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;";
    uint8_t Test3[] = "exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a";

    switch (ix) {
        case 0:
            len=strlen("+$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;");
            CDC_Receive_FS((uint8_t*)"+$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+;fork-events+;vfork-events+;", &len);
            break;
        case 1:
            len=strlen("exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a");
            CDC_Receive_FS((uint8_t*)"exec-events+;vContSupported+;QThreadEvents+;no-resumed+;xmlRegisters=i386#6a", &len);
        case 2:
            len=strlen(Test2);
            CDC_Receive_FS((uint8_t*)Test2, &len);
            break;
        case 3:
            len=strlen(Test3);
            CDC_Receive_FS((uint8_t*)Test3, &len);
            break;
        case 4:
            len=strlen("+");
            CDC_Receive_FS((uint8_t*)"+", &len);
            break;
        case 5:
            len=strlen("+$vMustReplyEmpty#3a");
            CDC_Receive_FS((uint8_t*)"+$vMustReplyEmpty#3a", &len);
            break;
        case 6:
            len=strlen("+$Hg0#df");
            CDC_Receive_FS((uint8_t*)"+$Hg0#df", &len);
            break;
        case 7:
            len=strlen("+$m8000000,4#25");
            CDC_Receive_FS((uint8_t*)"$m8000000,4#25", &len);
            CDC_Receive_FS((uint8_t*)"$m8000000,4#25", &len);
            break;

        // Read single byte
        case 8:
            len=strlen("+$m8000000,1#22");
            CDC_Receive_FS((uint8_t*)"$m8000000,1#22", &len);
            break;
        case 9:
            len=strlen("+$m8000001,1#23");
            CDC_Receive_FS((uint8_t*)"$m8000000,1#23", &len);
            break;
        case 10:
            len=strlen("+$m8000002,1#24");
            CDC_Receive_FS((uint8_t*)"$m8000002,1#24", &len);
            break;
        case 11:
            len=strlen("+$m8000003,1#25");
            CDC_Receive_FS((uint8_t*)"$m8000003,1#25", &len);
            break;
        case 12:
            len=strlen("+$m8000004,1#26");
            CDC_Receive_FS((uint8_t*)"$m8000004,1#26", &len);
            break;



/*
w 
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
*/


    }


    ix++;

#endif
}