#include <mega128.h>
#include <delay.h>

#include "lcd.h"

void main(void)
{
    unsigned char str[] = "KIM KIDEOK";  
    
    unsigned char i;
    unsigned char line = 0;
    
    while(1)
    {   
        for(line=0;line<2;line++)
        {
            LCD_Init();
            LCD_Pos(line,1);
            LCD_Str(str);
        
            for(i=0;i<16;i++)
            {
                LCD_Display_Shift(LEFT);
                delay_ms(500);
            }
            LCD_Cursor_Home();
        }
    }
}