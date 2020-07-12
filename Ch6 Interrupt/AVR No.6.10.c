#include <mega128.h>
#include <delay.h>

char Count;
unsigned char ReadE;

interrupt [EXT_INT7] void ext_int7_isr(void)
{
    ReadE = PINE;
    if(ReadE & (1<<PORTE6)) Count++;  
    else Count--;                            
}

void Interrupt_init(void)
{
    EIMSK |= (1<<INT7);             
    EICRB = (1<<ISC71);  
    DDRB = 0xff;
    DDRE = 0x00;
    SREG |= 0x80;
}          

void main(void)
{
    char prev_dt = 0;
    unsigned char LED = 0xfe;

    Interrupt_init();
    Count = 0;
    PORTB = 0xff;
    while(1)
    {
        if(prev_dt > Count)
        {
            //Dir = Left;
            prev_dt = Count;
            
            if(LED == 0xff)LED = 0x7f;
            PORTB = LED;
            LED = LED >> 1 | 0x80;
            delay_ms(1);    
    }
        else if(prev_dt < Count)
        {
            //Dir = Right;
            prev_dt = Count;
            
            if(LED == 0xff) LED = 0xfe;
            PORTB = LED;
            LED = LED << 1 |0x01;
            delay_ms(1);
        }
        else
        { 
            
        }
    }
} 