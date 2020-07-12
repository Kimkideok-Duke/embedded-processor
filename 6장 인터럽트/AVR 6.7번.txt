#include <mega128.h>
#include <delay.h>

bit Dir;
unsigned char LED;

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    Dir = ~Dir; 
}

interrupt [EXT_INT1] void ext_int1_isr(void)
{
    PORTB = 0x00;
    delay_ms(50);
    PORTB = 0xff;
    delay_ms(50);
}

void Interrupt_init(void)
{
    EIMSK = 0x03;
    EICRA = 0x0a;
    DDRB = 0xff;
    DDRD = 0x00;
    SREG |= 0x80;
}          

void main(void)
{
    Interrupt_init();
    LED = 0xfe;
    Dir = 0;
    while(1)
    {
        if(Dir)
        {
            if(LED == 0xff) LED = 0x7f;
            PORTB = LED;
            LED = LED >> 1 | 0x80;
            delay_ms(500);
        }
        else
        {
            if(LED == 0xff) LED = 0xfe;
            PORTB = LED;
            LED = LED << 1 | 0x01;
            delay_ms(500);
        }
    }
} 