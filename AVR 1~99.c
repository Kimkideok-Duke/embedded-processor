#include <mega128.h>
#include <delay.h>

unsigned char num_data[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f};

void PORT_Init(void)
{
    DDRB = 0xff;
    DDRE = 0xff;
}

void main()
{
    unsigned char i, j; 
    unsigned int k=0;
    PORT_Init();
    
    while(1)
    {
        for(i=0; i<10; i++)
        {
            
            for(j=0; j<10; j++)
            {
                while(1)
                {   
                    k++; 
                    PORTE = Port_fnd[1];
                    PORTB = num_data[i];
                    delay_ms(10);
                    PORTE = Port_fnd[0];
                    PORTB = num_data[j];
                    delay_ms(10);
                    
                    if(k>50)
                    {
                        break;
                    } 
                }  
                k=0;
            }      
        }
    }
}