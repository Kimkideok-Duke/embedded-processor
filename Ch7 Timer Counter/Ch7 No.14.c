#include <mega128.h>
#include <delay.h>

unsigned char count = 0;
unsigned char sec = 0;

void Init_Timer2(void)
{
    TIMSK = (1<<TOIE2); // ① 오버플로우(TOV0) 인터럽트 선택
    TCCR2 = (1<<CS22)|(1<<CS20); // ② 일반 모드, 1024분주비 사용
    TCNT2 = -15625; // ② 카운터 값 설정 
    DDRB = 0xff;
    SREG |= 0x80; // ③인터럽트 동작시작(전체인터럽트허가)
}

interrupt [TIM2_OVF] void timer2_overflow(void)
{
    TCNT0= -15625; // 카운터 값 재설정
    count++;
    if(count == 50)
    {   
        sec++;
        count = 0;
        PORTB = 0x00;
        delay_ms(10);
    }
    if(sec==60)
    {
        sec = 0;
    }
    PORTB = 0xff;
}

void main()
{
    Init_Timer2();
    PORTB = 0xff;
    while(1);
}