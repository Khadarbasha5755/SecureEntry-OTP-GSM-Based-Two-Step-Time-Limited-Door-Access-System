//lcd.c

#include<LPC21xx.h>
#include "types.h"
#include "lcd_defines.h"
#include "defines.h"
#include "delay.h"
void WriteLCD(u8 byte)
{
  //write byte on pins
  WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//select write operation to write data onto lcd contoller register(DDRAM)
	//for selecting write operation we apply logic 0 onto RW register
	IOCLR0=1<<LCD_RW;
	//apply high to low pulse through enable pin for store data from pins to DDRAM
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}
void CmdLCD(u8 cmd)
{
	//select command register for controlling the lcd
	//for selecting cmd register we apply logic 0 onto RS register
	IOCLR0=1<<LCD_RS;
	//write cmd to lcd
	WriteLCD(cmd);
}
void InitLCD(void)
{
	//cfg p0.8 to p0.15,p0.16,p0.17,p0.18 as gpio output pins
	IODIR0|=((0xFF<<LCD_DATA)|(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	//wait for 15ms to stabilize the lcd internal power supply and oscillator
	delay_ms(15);
	//0x30 is a function set cmd in lcd.it tells the lcd to intialize in 8 bit interface mode.
	//that's why we give 0x30 again and again to tell the lcd work in 8 bit mode
	CmdLCD(0x30);
	//then again wait 4.1 ms
	delay_ms(4);
	delay_us(100);
	CmdLCD(0x30);
	//again wait 100 ms
	delay_us(100);
	CmdLCD(0x30);
	//give which mode you want and how many lines you wnat
	CmdLCD(MODE_8BIT_2LINE);
	//diplay on cursor blinking
	CmdLCD(DSP_ON_CUR_OFF);
	//clear the LCD screen
	CmdLCD(CLEAR_LCD);
	//shift the cursor to right
	CmdLCD(SHIFT_CUR_RIGHT);
}
void CharLCD(u8 ch)
{
	//select the data register to diaplay the character on the LCD
	//for selecting data register we apply logic 1 onto RS register
	IOSET0=1<<LCD_RS;
	//write character to lcd through ddram
	WriteLCD(ch);
}
void StrLCD(s8 *str)
{
	while(*str)
	{
		CharLCD(*str);
		str++;
	}
}
void U32LCD(u32 n)
{
	u8 a[20];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			a[i]=(n%10)+48;
			n=n/10;
			i++;
		}
		for(--i;i>=0;i--)
		{
			CharLCD(a[i]);
		}
	}
}
void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);
}
void F32LCD(f32 fn,u8 nDp)
{
	u32 n;
	s32 i;
	if(fn<0.0)
	{
		CharLCD('-');
		fn=-fn;
	}
	n=fn;
	U32LCD(n);
	CharLCD('.');
	for(i=0;i<nDp;i++)
	{
		fn=(fn-n)*10;
		n=fn;
		CharLCD(n+48);
	}
}
void BuildCGRAM(u8 *p,u8 nBytes)
{
	s32 i;
	//start the cgram
	CmdLCD(GOTO_CGRAM_START);
	//write into cgram
	//select the data register for selecting apply logic 1 onto RS register
	IOSET0=1<<LCD_RS;
	for(i=0;i<nBytes;i++)
	{
		WriteLCD(p[i]);
	}
	CmdLCD(GOTO_LINE1_POS0);
}
void LCD_GotoXY(u8 row,u8 col)
{
    if(row==0)
        CmdLCD(0x80+col);
    else
        CmdLCD(0xC0+col);
}

void LCD_StringXY(u8 row,u8 col,s8 *str)
{
    LCD_GotoXY(row,col);
    StrLCD(str);
}
void LCD_CharXY(u8 row, u8 col, u8 ch)
{
    if(row == 0)
        CmdLCD(0x80 + col);   // Line 1
    else
        CmdLCD(0xC0 + col);   // Line 2

    CharLCD(ch);
}
