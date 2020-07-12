#include <mega128.h>
#include <delay.h>

unsigned char count = 0;

void Init_Timer0(void)
{
    TIMSK = (1<<OCIE0); // ① 오버플로우(TOV0) 인터럽트 선택
    TCCR0 = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); // ② CTC 모드, 1024분주비 사용
    OCR0 = 144; // ② 카운터 값 설정(1clk = 694ms) 
    DDRB = 0xff;
    SREG |= 0x80; // ③인터럽트 동작시작(전체인터럽트허가)
}

interrupt [TIM0_COMP] void timer0_overflow(void)
{
    count++;
    if(count == 100)
    {
        count = 0;
        PORTB = ~PORTB;
    }
}

void main()
{
    Init_Timer0();
    PORTB = 0xff;
    while(1);
}