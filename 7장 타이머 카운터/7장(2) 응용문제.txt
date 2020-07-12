#include <mega128.h>
#include <delay.h>

unsigned int cnt2 = 0;

void Init_Timer2(void)
{
    TIMSK |= 1<<OCIE2;
    TCCR2 |= (1<<WGM20) | (1<<COM21) | (1 << CS21);
    OCR2 = 0x00;
}

void Init_Timer0(void)
{
    TIMSK |= (1 << OCIE0);
    TCCR0 |= (1 << WGM01) | (1 << CS01);
    OCR0 = 100;
}

interrupt [TIM0_COMP] void timer0_out_comp(void)
{
    cnt2++;
    if(cnt2 == 20000) 
    {
        cnt2 = 0;
        OCR2 ++;
        if(OCR0 == 255)
        {
            OCR2 = 0;
        }
    }
}

interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    #asm("nop");
}

void Init_TimerINT(void)
{
    Init_Timer0();
    Init_Timer2();
    SREG |= 0x80;
}

void main(void)
{
    cnt2 = 0;
    Init_TimerINT();
    DDRB = 0xff;
    while(1);
}