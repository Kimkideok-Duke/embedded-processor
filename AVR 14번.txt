#include <mega128.h>
#include <delay.h>

void NUM12(void)
{
    int led1 = 0xaf;
    PORTB = led1;   
    delay_ms(1000);
    led1 = (led1 << 1)|0x10;
    PORTB = led1;
    delay_ms(1000);
}

void NUM34(void)
{
    int led2 = 0x6f;
    PORTB = led2;
    delay_ms(1000);
    led2 = (~led2)|0x0f;
    PORTB = led2;
    delay_ms(1000);
}

void NUM56(void)
{
    int led3 = 0x0f;
    PORTB = led3;
    delay_ms(1000);
    led3 = led3 | 0x70;
    PORTB = led3;
    delay_ms(1000);
} 

void main()
{
    DDRB = 0xf0;
    while(1)
    {
        NUM12();
        NUM34();
        NUM56();
    }
}
