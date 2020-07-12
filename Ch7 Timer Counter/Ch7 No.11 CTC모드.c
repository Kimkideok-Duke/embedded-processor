#include <mega128.h>
#include <delay.h>

unsigned char count = 0;

void Init_Timer0(void)
{
    TIMSK = (1<<OCIE0); // �� �����÷ο�(TOV0) ���ͷ�Ʈ ����
    TCCR0 = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); // �� CTC ���, 1024���ֺ� ���
    OCR0 = 144; // �� ī���� �� ����(1clk = 694ms) 
    DDRB = 0xff;
    SREG |= 0x80; // �����ͷ�Ʈ ���۽���(��ü���ͷ�Ʈ�㰡)
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