#include <mega128.h>
#include <delay.h>

#include "lcd.h"

unsigned char num[] = "2015146003";
unsigned char class[] = "Electronics Eng";
unsigned char name[] = "KIM KIDEOK";
unsigned char lab[] = "Embedded LAB";
unsigned char SW;

void main(void)
{   
    while(1)
    {
        SW = PIND;
        if(SW != 0xff)
        {
            if(SW == 0xfe)
            {   
                LCD_Init();
                LCD_Pos(0,0);
                LCD_Str(num);
            }
            else if(SW == 0xfd)
            {   
                LCD_Init();
                LCD_Pos(0,0);
                LCD_Str(class);
            }
            else if(SW == 0xfb)
            {   
                LCD_Init();
                LCD_Pos(0,0);
                LCD_Str(name);
            }
            else if(SW == 0xf7)
            {   
                LCD_Init();
                LCD_Pos(0,0);
                LCD_Str(lab);
            }
        }
    }             
}