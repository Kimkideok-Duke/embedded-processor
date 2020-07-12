#include <mega128.h>
#include <delay.h>

unsigned char LED = 0x7f;
int i = 0;

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    if(0 <= i <= 6)
    {
        LED = LED >> 1 | 0x80;
        delay_ms(500);
        i--;   
    }
    else
    {
        LED = 0x7f;
        delay_ms(500);
        i = 0;
    }
}

interrupt [EXT_INT7] void ext_int7_isr(void)
{
    if(0<= i <= 6)
    {
        LED = LED << 1 | 0x01;
        delay_ms(500);
        i++;   
    }
    else
    {
        LED = 0xfe;
        delay_ms(500);
        i = 6;
    }   
}

void Interrupt_init(void)
{
    EIMSK = 0x81;
    EICRA = 0x02;
    EICRB = 0x80;
    DDRB = 0xff;
    DDRD = 0x00;
    SREG |= 0x80;
}          

void main(void)
{
    Interrupt_init();
    PORTB = LED;
    while(1)
    {
        PORTB = LED;
    }
} 