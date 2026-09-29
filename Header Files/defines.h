//defines.h

/* Write an 8-bit value to a specific position in a register. */
#define WRITEBYTE(WORD,STARTBIT,BYTE) \
        WORD=((WORD&(u32)~0xFF<<STARTBIT)|(BYTE<<STARTBIT))


/* Write a single bit value (0 or 1) to a specific bit position. */
#define WRITEBIT(WORD,BIT,BITVAL) \
        WORD=((WORD&~(1<<BIT))|(BITVAL<<BIT))


/* Write a 4-bit value to a specific position in a register. */
#define WRITENIBBLE(WORD,STARTBIT,NIBBLE) \
        WORD=((WORD&(u32)~0xF<<STARTBIT)|(NIBBLE<<STARTBIT))


/* Read the value of a single bit from a register. */
#define READBIT(WORD,BIT) \
        ((WORD>>BIT)&1)


/* Read 4 bits (nibble) starting from the given bit position. */
#define READNIBBLE(WORD,SBIT) \
        ((WORD>>SBIT)&0xF)


/* Copy one bit from one position to another position in the same register. */
#define READWRITEBIT(WORD,DBIT,SBIT) \
        WORD=((WORD&~(1<<DBIT))|(((WORD>>SBIT)&1)<<DBIT))


/* Copy one bit from a source register to a destination register. */
#define READWRITENIT2(DWORD,DBIT,SWORD,SBIT) \
        DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT))
