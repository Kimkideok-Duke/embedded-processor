#include <mega128.h>
#include <delay.h>
#define FND_DIG PORTE // FND 자리수
#define FND_DATA PORTB // FND 데이터
unsigned int count;
unsigned char milisec, sec;
bit onoff;
// FNT 각 자리수에 출력할 문자 모양(0, 1, 2, 3 .... F)
unsigned char FND_char[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x88,0x83,0xc4,0xa1,0x84,0x8e}; // 애노드 공통
unsigned int FND_dig[] ={0x1f,0x2f,0x4f,0x8f,0x0f}; // FNT 자리수 정의 1의 자리, 10의 자리, 100의 자리, 1000의 자리, All OFF
void Init_Port(void)
{
    DDRE = 0xf0; // FND 열선택
    DDRB = 0xff; // FND 데이터
}
void Init_Timer0(void)
{
    TIMSK = (1<<TOIE0); // ① 오버플로우(TOV0) 인터럽트 선택
    TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00); // ② 일반 모드, 1024분주비 사용
    TCNT0 = -5; // ② 카운터 값 설정(1clk = 694ms)
    SREG |= 0x80; // ③인터럽트 동작시작(전체인터럽트허가)
}
interrupt [TIM0_OVF] void timer0_overflow(void)
{
    TCNT0= -5; // 카운터 값 재설정(684ms)
    if(onoff == 1)
    {   
        count++;
        if(count == 29) //
        {
            milisec++;
            count = 0;
        }
        if(milisec == 100) 
        {
            milisec = 0;
            sec++;
        }
    }
}

void interrupt_init(void)
{
    EIMSK = 0x07;
    EICRA = (1<<ISC21)|(1<<ISC11)|(1<<ISC01);
    DDRD = 0x00;
    SREG |= 0x80;
}
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    onoff = 1;    
}

interrupt [EXT_INT1] void ext_int1_isr(void)
{
    count = 0;
    milisec = 0;
    sec = 0;
}

interrupt [EXT_INT2] void ext_int2_isr (void)
{
    onoff = 0;
}

void FND_Display(unsigned char Sec, unsigned char Milisec)
{
    FND_DIG = FND_dig[0]; // 10밀리초의 0의 자리
    FND_DATA = FND_char[Milisec%10];
    delay_ms(1);


    FND_DIG = FND_dig[1]; // 10밀리초의 10의 자리
    FND_DATA = FND_char[Milisec/10];
    delay_ms(1);
 
    if(Sec % 2) 
    {
        FND_DIG = FND_dig[2]; // 초의 1의 자리
        FND_DATA = FND_char[Sec%10] & 0x7F;
        delay_ms(1);
    }
    else 
    {
        FND_DIG = FND_dig[2]; // 초의 1의 자리
        FND_DATA = FND_char[Sec%10];
        delay_ms(1);
    }

    FND_DIG = FND_dig [3]; // 초의 10의 자리
    FND_DATA = FND_char[Sec/10];
    delay_ms(1);
}
void main(void)
{
    count = 0;
    milisec = 0;
    sec = 0;
    onoff = 0;
    Init_Port();
    interrupt_init();
    Init_Timer0(); // 타이머/카운터0의 초기화
    while(1) // 무한 루프
    {
        FND_Display(sec, milisec);
    }
}