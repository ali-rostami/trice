//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_RING_BUFFER

static int TriceSingleDeferredOut(uint32_t* addr);

//! TriceRingBuffer is a kind of heap for trice messages.
uint32_t TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2] = {0};

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = TriceRingBuffer; 

//! triceBufferWriteLimit is the first address behind TriceRingBuffer. 
uint32_t* const triceRingBufferLimit = &TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2]; 

//! singleTricesRingCount holds the readable trices count inside TriceRingBuffer.
int singleTricesRingCount = 0; 

#pragma push
#pragma diag_suppress=170 //warning:  #170-D: pointer points outside of underlying object
//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
//! This is first the TRICE_DATA_OFFSET byte space followedy the trice data.
//! Initally this value is set to TriceRingBuffer minus TRICE_DATA_OFFSET byte space
//! to ga correct value for the very first call of triceNextRingBufferRead
uint32_t* TriceRingBufferReadPosition = TriceRingBuffer - (TRICE_DATA_OFFSET>>2); //lint !e428 Warning 428: negative subscript (-4) in operator 'ptr-int'
#pragma  pop

//! triceNextRingBufferRead returns a single trice data buffer address. The trice are data starting at byte offset TRICE_DATA_OFFSET.
//! Implicit assumed is singleTricesRingCount > 0.
//! \param lastWordCount is the uint32 count of the last read trice including padding bytes.
//! The value lastWordCount is needed to increment TriceRingBufferReadPosition accordingly.
//! \retval is the address of the next trice data buffer.
static uint32_t* triceNextRingBufferRead( int lastWordCount ){
    TriceRingBufferReadPosition += (TRICE_DATA_OFFSET>>2) + lastWordCount;
    if( (TriceRingBufferReadPosition + (TRICE_BUFFER_SIZE>>2)) > triceRingBufferLimit ){
        TriceRingBufferReadPosition = TriceRingBuffer;
    }
    return TriceRingBufferReadPosition; 
}

//! TriceTransfer needs to be called cyclically to read out the Ring Buffer.
void TriceTransfer( void ){
    if( singleTricesRingCount == 0 ){ // no data
        return;
    }
    if( TriceOutDepth() ){ // last transmission not finished
        return;
    }
    singleTricesRingCount--;
    static int lastWordCount = 0;
    uint32_t* addr = triceNextRingBufferRead( lastWordCount );
    lastWordCount = TriceSingleDeferredOut(addr);
}

//! TriceSingleDeferredOut expects a single trice at addr with byte offset TRICE_DATA_OFFSET and returns the wordCount of this trice which includes 1-3 padding bytes.
//! \param addr points to TRICE_DATA_OFFSET bytes usble space followed by the begin of a single trice.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_DATA_OFFSET/4)+(TRICE_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
static int TriceSingleDeferredOut(uint32_t* addr){
    uint32_t* pData = addr + (TRICE_DATA_OFFSET>>2);
    uint8_t* pEnc = (uint8_t*)addr;
    int wordCount;
    uint8_t* pStart;
    size_t Length;
    int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    size_t encLen = TriceDeferredEncode( pEnc, pStart, Length);
    TriceNonBlockingWrite( triceID, pEnc, encLen );
    return wordCount;
}

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER