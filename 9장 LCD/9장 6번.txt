#include <mega128.h>
#include <delay.h>

#include "lcd.h"

unsigned int num = 48;
unsigned int i, line;

void main(void)
{
    while(1)
    {
        while(num<59)
        {
            LCD_Init();
            for(line=0;line<2;line++)
            {
                for(i=0;i<16;i++)
                {
                    LCD_Pos(line,i);
                    LCD_Char(num);
                }
            }
            delay_ms(1000);
            num++; 
            if(num == 58)num = 48;
        }
    }        
}