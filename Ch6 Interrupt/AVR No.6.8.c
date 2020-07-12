#include <mega128.h>
#include <delay.h>

unsigned char count;
unsigned char LED;
int i = 0;

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    count++; 
}

void Interrupt_init(void)
{
    EIMSK = 0x01;
    EICRA = 0x02;
    DDRB = 0xff;
    DDRD = 0x00;
    SREG |= 0x80;
}          

void main(void)
{
    Interrupt_init();
    PORTB = 0xff;
    while(1)
    {
        if(count == 1)
        {   
            LED = 0x7f;
            while(i<=7)
            {
                PORTB = LED;
                LED = LED >> 1 | 0x80;
                delay_ms(100);
                i++;
            }
            i = 0;
        }
        else if(count == 2)
        {
            LED = 0xfe;
            while(i<=7)
            {
                PORTB = LED;
                LED = LED << 1 | 0x01;
                delay_ms(100);
                i++;
            }
            i = 0; 
        }
        else if(count == 3)
        {
            PORTB = 0x00;
            delay_ms(500);
            PORTB = 0xff;
            delay_ms(500);
        }
        else if(count == 4)
        {
            PORTB = 0xf0;
            delay_ms(500);
            PORTB = 0x0f;
            delay_ms(500);
        }
        else if(count >= 5)
        {
            PORTB = 0xff;
            count = 1;
        }
    }
} 
