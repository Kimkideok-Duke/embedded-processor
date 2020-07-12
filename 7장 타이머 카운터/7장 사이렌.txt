#include <mega128.h>
#include <delay.h>

unsigned int freq = 300;
unsigned int cnt1 = 0;

void Init_Port(void)
{
    DDRG = 0x10;
}

void Init_Timer0(void)
{
    TIMSK = (1<< OCIE0);
    TCCR0 = ((1<<WGM01) | (1<<CS01) | (1<<CS00)); // CTC 모드, 32분주
    OCR0 = 1;
    SREG |= 0x80;
}

interrupt [TIM0_COMP] void timer0_out_comp(void)
{
    cnt1++;
    if(cnt1 > freq)
    {
        PORTG = ~PORTG; 
        cnt1 = 0;
    } 
}

void main(void)
{
    Init_Timer0();
    Init_Port();
    PORTG = 0x10;
    freq = 300;
    while(1)
    {   
        while(freq<750)
        {
            freq++;
            delay_ms(1); 
        }  
        while(freq>=300)
        {
            freq--;  
            delay_ms(1);
        } 
    }
}