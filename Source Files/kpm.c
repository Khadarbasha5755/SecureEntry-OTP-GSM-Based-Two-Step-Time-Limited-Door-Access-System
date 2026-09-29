//kpm.c

// 4x4 Matrix Keypad driver.
// This file contains functions to:
//     1. Initialize the keypad GPIO pins.
//     2. Detect whether any key is pressed.
//     3. Identify the pressed row.
//     4. Identify the pressed column.
//     5. Find the actual key value.
//     6. Read a complete number from the keypad.

// Keypad arrangement:
//
//        C0   C1   C2   C3
//     R0  7    8    9    B
//     R1  4    5    6    /
//     R2  1    2    3    -
//     R3  C    0    E    +

//==============================================================

#include <LPC21xx.h>
#include "types.h"
#include "kpm_defines.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"

/*==============================================================
                    EXTERNAL INTERRUPT FLAG
  =============================================================*/

extern volatile u8 edit_request;


/*==============================================================
                         KEYPAD LUT
  =============================================================*/

u8 kpmLUT[4][4] =
{
    /* Row 0 */
    {'1','2','3','A'},

    /* Row 1 */
    {'4','5','6','B'},

    /* Row 2 */
    {'7','8','9','C'},

    /* Row 3 */
    {'*','0','#','D'}
};


/*==============================================================
                          Init_KPM()
  =============================================================*/

void Init_KPM(void)
{
    /*
       Configure four row pins as output pins.
    */

    WRITENIBBLE(IODIR1,ROW0,15);
}


/*==============================================================
                           colscan()
  =============================================================*/

u32 colscan(void)
{
    /*
       0 -> Key pressed
       1 -> No key pressed
    */

    if(READNIBBLE(IOPIN1,COL0) < 15)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/*==============================================================
                           rowcheck()
  =============================================================*/

u32 rowcheck(void)
{
    u32 rno;

    for(rno=0; rno<4; rno++)
    {
        WRITENIBBLE(IOPIN1,ROW0,(~(1<<rno)));

        if(colscan() == 0)
        {
            break;
        }
    }

    IOCLR1 = 15 << ROW0;

    return rno;
}


/*==============================================================
                           colcheck()
  =============================================================*/

u32 colcheck(void)
{
    u32 cno;

    for(cno=0; cno<4; cno++)
    {
        if(READBIT(IOPIN1,(cno+COL0)) == 0)
        {
            break;
        }
    }

    return cno;
}


/*==============================================================
                           keyscan()
  =============================================================*/

u32 keyscan(void)
{
    u32 row;
    u32 col;
    u32 key;

    /*----------------------------------------------------------
                       WAIT FOR KEY PRESS
      ----------------------------------------------------------*/

    while(colscan());


    /*----------------------------------------------------------
                          FIND ROW
      ----------------------------------------------------------*/

    row = rowcheck();


    /*----------------------------------------------------------
                         FIND COLUMN
      ----------------------------------------------------------*/

    col = colcheck();


    /*----------------------------------------------------------
                         FIND KEY VALUE
      ----------------------------------------------------------*/

    key = kpmLUT[row][col];


    /*----------------------------------------------------------
                       WAIT FOR KEY RELEASE
      ----------------------------------------------------------*/

    while(!colscan());


    return key;
}


/*==============================================================
                           readnum()
  =============================================================*/

u32 readnum(void)
{
    u32 num = 0;
    u8 key;

    while(1)
    {
        key = keyscan();

        if(key >= '0' && key <= '9')
        {
            num = (num * 10) + (key - 48);
        }
        else
        {
            break;
        }
    }

    return num;
}


/*==============================================================
                         ReadNumLCD()
  =============================================================*/

u32 ReadNumLCD(void)
{
    u32 num = 0;
    u8 key;
    u8 digits = 0;
    u8 pos = 0;

    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        key = keyscan();


        /*======================================================
                         NUMBER KEY
          ======================================================*/

        if(key >= '0' && key <= '9')
        {
            num = (num * 10) + (key - '0');

            LCD_CharXY(1,pos,key);

            pos++;
            digits++;
        }


        /*======================================================
                          BACKSPACE
          ======================================================*/

        else if(key == KEY_BACKSPACE)
        {
            if(digits > 0)
            {
                num = num / 10;

                digits--;
                pos--;

                LCD_CharXY(1,pos,' ');

                LCD_GotoXY(1,pos);
            }
        }


        /*======================================================
                            CLEAR
          ======================================================*/

        else if(key == KEY_CLEAR)
        {
            num = 0;
            digits = 0;
            pos = 0;

            CmdLCD(GOTO_LINE2_POS0);

            StrLCD("                ");
        }


        /*======================================================
                            ENTER
          ======================================================*/

        else if(key == KEY_ENTER)
        {
            break;
        }
    }

    return num;
}


/*==============================================================
                         keyscan_nb()
  =============================================================*/

/*
   Non-blocking keypad scan.

   Return:
       0 -> No key pressed
       key value -> Key pressed
*/

u32 keyscan_nb(void)
{
    u32 row;
    u32 col;
    u32 key;

    /*-----------------------------------------------
                    NO KEY PRESSED
      ------------------------------------------------*/

    if(colscan())
    {
        return 0;
    }

    /*-----------------------------------------------
                    KEY PRESSED
      ------------------------------------------------*/

    /* Small debounce delay */
    delay_ms(20);

    /* Check again after debounce */
    if(colscan())
    {
        return 0;
    }

    row = rowcheck();
    col = colcheck();

    key = kpmLUT[row][col];

    /*-----------------------------------------------
                    WAIT FOR KEY RELEASE
      ------------------------------------------------*/

    while(!colscan());

    /*-----------------------------------------------
              WAIT FOR RELEASE TO STABILIZE
      ------------------------------------------------*/

    delay_ms(20);

    /* Make sure key is really released */
    while(!colscan());

    return key;
}
