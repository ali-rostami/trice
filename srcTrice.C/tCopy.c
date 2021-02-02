/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(65254), "rd_:triceCheck.c" );
//#define TRICE_CODE NO_CODE // enable this line to disable trice code generation in this file object
#include "trice.h"

#include "main.h" // __get_PRIMASK

///////////////////////////////////////////////////////////////////////////////
// trice time measurement
//

#ifndef TRICE_S
#define TRICE_S( i, p, s )
#endif


//! triceRuntimeGeneratedStringUnbound can transfer runtime generated strings if TRICES_1 is not available.
TRICE_INLINE void triceRuntimeGeneratedStringUnbound( const char* s ){
    size_t len = strlen( s );
    char c1, c2, c3, c4, c5, c6, c7, c8;
    while( len ){
        switch( len ){
            case  0: return;
            case  1: c1=*s++;
                TRICE8_1( Id(65329), "%c", c1 ); return;
            case  2: c1=*s++; c2=*s++;
                TRICE8_2( Id(65279), "%c%c", c1, c2 ); return;
            case  3: c1=*s++; c2=*s++; c3=*s++;
                TRICE8_3( Id(65057), "%c%c%c", c1, c2, c3 ); return;
            case  4: c1=*s++; c2=*s++; c3=*s++; c4=*s++;
                TRICE8_4( Id(65052), "%c%c%c%c", c1, c2, c3, c4 ); return;
            case  5: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++;
                TRICE8_5( Id(65088), "%c%c%c%c%c", c1, c2, c3, c4, c5 ); return;
            case  6: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++;
                TRICE8_6( Id(65473), "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6 ); return;
            case  7: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++; c7=*s++;
                TRICE8_7( Id(65121), "%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7); return;
            case  8:
            default: c1 = *s++; c2 = *s++; c3 = *s++; c4 = *s++; c5 = *s++; c6 = *s++; c7 = *s++; c8 = *s++;
                TRICE8_8( Id(65468), "%c%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7, c8 );
                len -= 8;
        }
    }
    return;
}

//! trice runtime string
#define TRICE_RTS(dynString) do{ triceRuntimeGeneratedStringUnbound(dynString); }while(0)

// nextRuntimeString returns a in length changing string from 0 to 250 bytes.
char* nextRuntimeString( int length ){
    static char rts[300] = {0};
    for( int i = 0; i < length; i++ ){
       char c = 0x7f & (0x20 + i);
        c = c < 0x20 ? c + 0x20 : c; // exclude control characters
        c = '`' == c ? 0x20 : c; // exclude back tick for easy testTable generation
        c = 127 == c ? 0x20 : c; // exclude DEL control character
        rts[i] = c;
    }
    rts[length] = 0;
    return rts;
}

// triceRuntimeStrings sends n stings to the trice port.
void triceRuntimeStrings( int from, int limit){
    for( int i = from; i < limit; i++ ){
        char* rts =  nextRuntimeString(i);
        TRICE_S( Id(65213), "%s\n", rts );
    }
}

//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSetTime(int index) {
    switch (index) {
        case 0:
            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE16_1(Id(65168), "dbg:12345 as 16bit is %#016b\n", 12345);
            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE0(Id(65010), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE0(Id(65304), "--------------------------------------------------\n");
            break;
        case 1:
            TRICE16_1(Id(65236), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65388), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65105), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65312), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65439), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65309), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65211), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65438), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65044), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65463), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65364), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65099), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 2:
            TRICE16_1(Id(65274), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65274), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65274), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(65274), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            TRICE32_1(Id(65287), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(65287), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(65287), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(65287), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 4:
            TRICE8_4( Id(65492), "tst:TRICE8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id(65331), "tst:TRICE8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id(65103), "tst:TRICE8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE16_4(Id(65055), "tst:TRICE16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(65077), "tst:TRICE16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(65060), "tst:TRICE16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE32_4(Id(65510), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id(65066), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE64_1(Id(65396), "att:TRICE64_1 %#b\n", 0x1122334455667788);
            break;
        case 5:
            TRICE8_1(Id(65409), "tst:TRICE8_1 %d\n", -111);
            TRICE8_2(Id(65308), "tst:TRICE8_2 %d %d\n", -111, -222);
            TRICE8_3(Id(65454), "tst:TRICE8_3 %d %d %d\n", -111, -222, -3);
            TRICE8_4(Id(65013), "tst:TRICE8_4 %d %d %d %d\n", -111, -222, -3, -4);
            TRICE8_5(Id(65239), "tst:TRICE8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            TRICE8_6(Id(65372), "tst:TRICE8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            TRICE8_7(Id(65246), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            TRICE8_8(Id(65264), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 7:
            TRICE16_1(Id(65416), "tst:TRICE16_1 %d\n", -111); // 1
            TRICE16_2(Id(65412), "tst:TRICE16_2 %d %d\n", -111, -222); // 2
            TRICE16_3(Id(65017), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333); // 3
            TRICE16_4(Id(65117), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 8:
            TRICE32_1(Id(65048), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_1(Id(65529), "tst:TRICE32_1 %d\n", -111); // 2
            TRICE32_2(Id(65318), "tst:TRICE32_2 %x %x\n", -111, -222); // 4
            TRICE32_2(Id(65021), "tst:TRICE32_2 %d %d\n", -111, -222); // 4
            TRICE32_3(Id(65073), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333); // 4
            TRICE32_3(Id(65132), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333); // 4
            TRICE32_4(Id(65283), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444); // 4
            TRICE32_4(Id(65450), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 9:
            TRICE64_1(Id(65528), "tst:TRICE64_1 %d\n", -111); // 2
            TRICE64_2(Id(65391), "tst:TRICE64_2 %d %d\n", -111, -222); // 4
            break;
        case 10:
            TRICE0(Id(65184), "e:A");
            TRICE0(Id(65042), "w:B");
            TRICE0(Id(65167), "a:c");
            TRICE0(Id(65443), "wr:d");
            TRICE0(Id(65367), "rd:e\n");
            TRICE0(Id(65090), "diag:f");
            TRICE0(Id(65201), "d:G");
            TRICE0(Id(65517), "t:H");
            TRICE0(Id(65041), "time:i");
            TRICE0(Id(65344), "message:J");
            TRICE0(Id(65314), "dbg:k\n");
            break;
        case 11:
            TRICE0(Id(65228), "1");
            TRICE0(Id(65161), "2");
            TRICE0(Id(65109), "3");
            TRICE0(Id(65219), "4");
            TRICE0(Id(65046), "e:7");
            TRICE0(Id(65005), "m:12");
            TRICE0(Id(65385), "m:123\n");
            break;
        case 12:
            TRICE8_1(Id(65409), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(65308), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(65454), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(65013), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(65239), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(65372), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(65246), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(65264), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            TRICE_RTS( "an_example_string\n" );
            TRICE_RTS( "" );
            TRICE_RTS( "\n" );
            TRICE_RTS( "a\n" );
            TRICE_RTS( "an\n" );
            TRICE_RTS( "an_\n" );
            TRICE_RTS( "an_e\n" );
            TRICE_RTS( "an_ex\n" );
            TRICE_RTS( "an_exa\n" );
            TRICE_RTS( "an_exam\n" );
            TRICE_RTS( "an_examp\n" );
            TRICE_RTS( "an_exampl\n" );
            TRICE_RTS( "an_example\n" );
            TRICE_RTS( "an_example_\n" );
            TRICE_RTS( "an_example_s\n" );
            TRICE_RTS( "an_example_st\n" );
            TRICE_RTS( "an_example_str\n" );
            TRICE_RTS( "an_example_stri\n" );
            TRICE_RTS( "an_example_strin\n" );
            TRICE_RTS( "an_example_string\n" );
            break;
        case 14:
            triceRuntimeStrings(0, 20);
            break;
       case 15: 
			triceRuntimeStrings(30, 35 );
           break;
       case 16: 
			triceRuntimeStrings(126, 130);
           break;
       case 17: 
			triceRuntimeStrings(254, 256);
           break;
        case 18:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id(65083), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(65054), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
            encrypt(b);
            TRICE16_1( Id(65330), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(65484), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id(65526), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id(65061), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(65405), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
            decrypt(b);
            TRICE16_1( Id(65051), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(65281), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id(65029), "--------------------------------------------------\n\n" );
#endif
        break;
    default:
        break;
    }
}

#if TRICE_PACK2_ENCODING == TRICE_ENCODING || TRICE_BARE_ENCODING == TRICE_ENCODING
//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSetSpace(int index) {
    switch (index) {
        case 0:
            trice0(Id(65304), "--------------------------------------------------\n");
            trice0(Id(65304), "--------------------------------------------------\n");
            trice16_1(Id(65168), "dbg:12345 as 16bit is %#016b\n", 12345);
            trice0(Id(65304), "--------------------------------------------------\n");
            trice0(Id(65010), "sig:This ASSERT error is just a demo and no real error:\n");
            trice0(Id(65304), "--------------------------------------------------\n");
            break;
        case 1:
            trice16_1(Id(65236), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65388), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65105), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65312), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65439), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65309), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65211), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65438), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65044), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65463), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65364), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65099), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 2:
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(65370), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 4:
            trice8_4( Id(65493), "tst:trice8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(65400), "tst:trice8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(65137), "tst:trice8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            trice16_4(Id(65447), "tst:trice16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(65498), "tst:trice16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(65418), "tst:trice16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            trice32_4(Id(65507), "tst:trice32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice32_4(Id(65305), "tst:trice32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice64_1(Id(65251), "att:trice64_1 %#b\n", 0x1122334455667788);
            break;
        case 5:
            trice8_1(Id(65112), "tst:trice8_1 %d\n", -111);
            trice8_2(Id(65442), "tst:trice8_2 %d %d\n", -111, -222);
            trice8_3(Id(65115), "tst:trice8_3 %d %d %d\n", -111, -222, -3);
            trice8_4(Id(65159), "tst:trice8_4 %d %d %d %d\n", -111, -222, -3, -4);
            trice8_5(Id(65503), "tst:trice8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            trice8_6(Id(65495), "tst:trice8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            trice8_7(Id(65031), "tst:trice8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            trice8_8(Id(65523), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 7:
            trice16_1(Id(65424), "tst:trice16_1 %d\n", -111); // 1
            trice16_2(Id(65089), "tst:trice16_2 %d %d\n", -111, -222); // 2
            trice16_3(Id(65437), "tst:trice16_3 %d %d %d\n", -111, -222, -333); // 3
            trice16_4(Id(65144), "tst:trice16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 8:
            trice32_1(Id(65300), "tst:trice32_1 %d\n", -111); // 2
            trice32_2(Id(65485), "tst:trice32_2 %d %d\n", -111, -222); // 4
            trice32_3(Id(65533), "tst:trice32_3 %d %d %d\n", -111, -222, -333); // 4
            trice32_4(Id(65369), "tst:trice32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            trice32_1(Id(65094), "tst:trice32_1 %08x\n", 0x0123cafe); // 2
            trice32_2(Id(65525), "tst:trice32_2 %x %x\n", 0xfeedcafe, 0xcafe0123); // 4
            trice32_3(Id(65428), "tst:trice32_3 %x %x %x\n", 0xaa001122, 0xbb112233, 0xcc223344); // 4
            trice32_4(Id(65143), "tst:trice32_4 %x %x %x %x\n", 0x12345678, 0x9abcdef0, 0xfedcba98, 0x87654321); // 4
            break;
        case 9:
            trice64_1(Id(65299), "tst:trice64_1 %d\n", -111); // 2
            trice64_2(Id(65406), "tst:trice64_2 %d %d\n", -111, -222); // 4
            trice64_1(Id(65003), "tst:trice64_1 %x\n", 0x8765432112345678); // 2
            trice64_2(Id(65262), "tst:trice64_2 %x %x\n", 0x13579bdffdb97531, 0xeca8642002468ace); // 4
            break;
        case 10:
            trice0(Id(65184), "e:A");
            trice0(Id(65042), "w:B");
            trice0(Id(65167), "a:c");
            trice0(Id(65443), "wr:d");
            trice0(Id(65367), "rd:e\n");
            trice0(Id(65090), "diag:f");
            trice0(Id(65201), "d:G");
            trice0(Id(65517), "t:H");
            trice0(Id(65041), "time:i");
            trice0(Id(65344), "message:J");
            trice0(Id(65314), "dbg:k\n");
            break;
        case 11:
            trice0(Id(65228), "1");
            trice0(Id(65161), "2");
            trice0(Id(65109), "3");
            trice0(Id(65219), "4");
            trice0(Id(65046), "e:7");
            trice0(Id(65005), "m:12");
            trice0(Id(65385), "m:123\n");
            break;
        case 12:
            trice8_1(Id(65112), "tst:trice8_1 %d\n", 1);
            trice8_2(Id(65442), "tst:trice8_2 %d %d\n", 1, 2);
            trice8_3(Id(65115), "tst:trice8_3 %d %d %d\n", 1, 2, 3);
            trice8_4(Id(65159), "tst:trice8_4 %d %d %d %d\n", 1, 2, 3, 4);
            trice8_5(Id(65503), "tst:trice8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            trice8_6(Id(65495), "tst:trice8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            trice8_7(Id(65031), "tst:trice8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            trice8_8(Id(65523), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            TRICE_RTS( "an_example_string\n" );
            TRICE_RTS( "" );
            TRICE_RTS( "\n" );
            TRICE_RTS( "a\n" );
            TRICE_RTS( "an\n" );
            TRICE_RTS( "an_\n" );
            TRICE_RTS( "an_e\n" );
            TRICE_RTS( "an_ex\n" );
            TRICE_RTS( "an_exa\n" );
            TRICE_RTS( "an_exam\n" );
            TRICE_RTS( "an_examp\n" );
            TRICE_RTS( "an_exampl\n" );
            TRICE_RTS( "an_example\n" );
            TRICE_RTS( "an_example_\n" );
            TRICE_RTS( "an_example_s\n" );
            TRICE_RTS( "an_example_st\n" );
            TRICE_RTS( "an_example_str\n" );
            TRICE_RTS( "an_example_stri\n" );
            TRICE_RTS( "an_example_strin\n" );
            TRICE_RTS( "an_example_string\n" );
            break;
        case 14:
            triceRuntimeStrings(0, 20);
            break;
       case 15: 
			triceRuntimeStrings(30, 35 );
           break;
       case 16: 
			triceRuntimeStrings(126, 130);
           break;
       case 17: 
			triceRuntimeStrings(254, 256);
           break;
        case 18:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id(65083), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(65054), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
            encrypt(b);
            TRICE16_1( Id(65330), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(65484), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id(65526), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id(65061), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(65405), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
            decrypt(b);
            TRICE16_1( Id(65051), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(65281), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id(65029), "--------------------------------------------------\n\n" );
#endif
        break;
    default:
        break;
    }
}
#endif // #if TRICE_PACK2_ENCODING == TRICE_ENCODING
