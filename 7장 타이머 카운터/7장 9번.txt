#include <mega128.h>
#include <delay.h>

void main()
{
    DDRB = 0xff;
    PORTB.2 = 0;
    while(1)
    {
        delay_ms(5);
        PORTB.2 = ~PORTB.2;
    }
}