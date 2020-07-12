#include <mega128.h>
#include <delay.h>

unsigned char count = 0;

void Init_Timer0(void)
{
    TIMSK = (1<<TOIE0); // �� �����÷ο�(TOV0) ���ͷ�Ʈ ����
    TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00); // �� �Ϲ� ���, 1024���ֺ� ���
    TCNT0 = -144; // �� ī���� �� ����(1clk = 694ms) 
    DDRB = 0xff;
    SREG |= 0x80; // �����ͷ�Ʈ ���۽���(��ü���ͷ�Ʈ�㰡)
}

interrupt [TIM0_OVF] void timer0_overflow(void)
{
    TCNT0= -144; // ī���� �� �缳��(684ms)
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