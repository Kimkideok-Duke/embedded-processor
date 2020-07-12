#include <mega128.h>
#include <delay.h>
#define FND_DIG PORTE // FND 자리수
#define FND_DATA PORTB // FND 데이터
unsigned int count;
unsigned char num;
// FNT 각 자리수에 출력할 문자 모양(0, 1, 2, 3 .... F)
unsigned char FND_char[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x88,0x83,0xc4,0xa1,0x84,0x8e}; // 애노드 공통
unsigned int FND_dig[] ={0x1f,0x2f,0x4f,0x8f,0x0f}; // FND 자리수 정의 1의 자리, 10의 자리, 100의 자리, 1000의 자리, All OFF
void Init_Port(void)
{
    DDRE = 0xf0; // FND 열선택
    DDRB = 0xff; // FND 데이터
}
void Init_Timer0(void)
{
    TIMSK = (1<<OCIE0); // ① 오버플로우(TOV0) 인터럽트 선택
    TCCR0 = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); // ② CTC 모드, 1024분주비 사용
    OCR0 = 144; // ② 카운터 값 설정(1clk = 694ms)
    SREG |= 0x80; // ③인터럽트 동작시작(전체인터럽트허가)
}
interrupt [TIM0_COMP] void timer0_overflow(void)
{
    count++;
    if(count == 100)
    {
        num++;
        count = 0;
    }
    if(num == 101)
    {
        num = 0;
    }
}
void FND_Display(unsigned char num)
{
    unsigned char one, two, three;
    one = 0;
    two = 0;
    three = 0;
    
    one = (num % 100) % 10;
    two = (num % 100) / 10;
    three = num - (num % 100);
    if(num == 100)
    {
        three = 1;
    }
    
    FND_DIG = FND_dig[0]; // 1의 자리
    FND_DATA = FND_char[one];
    delay_ms(1);

    FND_DIG = FND_dig[1]; // 10의 자리
    FND_DATA = FND_char[two];
    delay_ms(1);

    FND_DIG = FND_dig [2]; // 100의 자리
    FND_DATA = FND_char[three];
    delay_ms(1);
}
void main(void)
{
    count = 0;
    num = 0;
    Init_Port();
    Init_Timer0(); // 타이머/카운터0의 초기화
    while(1) // 무한 루프
    {
        FND_Display(num);
    }
}