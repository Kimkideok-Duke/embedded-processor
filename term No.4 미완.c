#include <mega128.h>
#include <delay.h>
#include <math.h>
#include <string.h>

#include "lcd.h"           

#define FND_Null        0
#define FND_Star        14
#define FND_Sharp       15

#define  Do     1908
#define  Re     1700
#define  Mi     1515
#define  Fa     1432
#define  Sol     1275     

bit i = 0;
unsigned int j = 1;
bit sec1 = 0;
bit sec3 = 0;
bit y =0;
unsigned int select = 0;
unsigned int k =0;

unsigned char New_key_data=0, key_Num=0;                          
unsigned int buf_seg[5] = {FND_Null, FND_Null, FND_Null, FND_Null, '\0'};
bit Key_off_flag=0;
 
unsigned int count = 1000;
unsigned int sec, sec_escape = 0, cnt = 0;
bit go_tim0, go_tim2;
unsigned char one, two, three, four;

unsigned char rand_num[5];
unsigned char entrand_num[5] = {32, 32, 32, 32, '\0'};

unsigned char game[] = "MEMORY GAME!!";
unsigned char push[] = "PUSH Botton!!";
unsigned char mode[] = "GAME MODE";
unsigned char easy[] = "EASY";
unsigned char normal[] = "NORMAL";
unsigned char hard[] = "HARD";
unsigned char answer[] = "ENTER ANSWER";
unsigned char correct[] = "CORRECT!!";
unsigned char wrong[] = "WRONG!";
unsigned char fail[] = "FAIL!!";

void init_reg(void)
{
    DDRE = 0xf0;            //FND Sel
    DDRB = 0xff;            //FND Data Line
    DDRC = 0x0f;            //���� 4bit Col(�Է�), ���� 4bit Row(���)
    PORTC = 0x0f;           //Port �ʱ�ȭ          
}

unsigned char KeyScan(void)                     // 4X4 Ű�е� ��ĵ �Լ�, ��� ���� 10���� 1~16  {
{
    unsigned int Key_Scan_Line_Sel = 0xf7;      // Init_data ���� �Ϻ��� ����� ����  
    // ���� �Ϻ�(4bit)�� ����Ī(���������� ���ư��鼭)�ϸ鼭 ���    
    unsigned char Key_Scan_sel=0, key_scan_num=0;         
    unsigned char Get_Key_Data=0;            // ���� Ű ������        
      
    //Ű��ĵ �κ�  
    for(Key_Scan_sel=0; Key_Scan_sel<4; Key_Scan_sel++)     
    {           
          // �ʱ�ȭ 
          PORTC = Key_Scan_Line_Sel;               
          delay_us(10);                                
            
          //���� �κ�
          Get_Key_Data = (PINC & 0xf0);   // 74LS14�� ������ ���      
            
          if(Get_Key_Data != 0x00)
          {                  
                switch(Get_Key_Data)        // C��Ʈ ������ ���� �Ϻ�(4bit)�� ����            
                {
                      case 0x10:            // 0001�� ������ ���� count���� 4�� ������                                        
                                            //  1�� ���ϰ� key_scan_num������ ����
                            key_scan_num = Key_Scan_sel*4 + 1;                
                            break;                
                      case 0x20:            // 0010�� ������ ���� count���� 4�� ������
                                            //  2�� ���ϰ� key_scan_num������ ����
                            key_scan_num = Key_Scan_sel*4 + 2;                
                            break;                
                      case 0x40:            // 0100�� ������ ���� count���� 4�� ������ 
                                            //  3�� ���ϰ� key_scan_num������ ����           
                            key_scan_num = Key_Scan_sel*4 + 3;                
                            break;                
                      case 0x80:            // 1000�� ������ ���� count���� 4�� ������                                                      
                                            //  4�� ���ϰ� key_scan_num������ ����                                         
                            key_scan_num = Key_Scan_sel*4 + 4;                 
                            break;
                      default :
                            key_scan_num = FND_Null; 
                            break;                
                }           
                return key_scan_num;       
          }               
          Key_Scan_Line_Sel = (Key_Scan_Line_Sel>>1);
        //Init_data�� ����Ʈ ��, Key_Scan_Line �̵�     
    } 
    return key_scan_num;         
}

unsigned char keypad_in()               // ä�͸� ���� ���� 
{
      if(Key_off_flag)                   // Key_off_flag�� 1�� ���, �� Ű�� ������ ���
      {
            buf_seg[3] = buf_seg[2];        // ���׸�Ʈ �� ����Ʈ  ����� <- �����
            buf_seg[2] = buf_seg[1];        // ���׸�Ʈ �� ����Ʈ  ����� <- �����
            buf_seg[1] = buf_seg[0];        // ���׸�Ʈ �� ����Ʈ  ����� <- ����� 
            Key_off_flag = ~Key_off_flag;    // Ű �Է��� �ѹ��� �ޱ� ���� Key_off_flag ����
      }
      else;
           buf_seg[0] = key_Num;           // ���� Ű���� ���׸�Ʈ�� ǥ�� ����� 
      return key_Num;                       // �������� key_Num ����
}

void Keymain()
{
    New_key_data = KeyScan();
           if(New_key_data)
           {
                if(New_key_data%4 != 0)
                {
                    key_Num = (New_key_data/4)*3+(New_key_data%4);
                    if(key_Num >= 10)
                    {
                        switch(key_Num)
                        {
                            case 10 :
                                key_Num = FND_Star;
                                break;
                            case 11 :
                                key_Num = 0;
                                break; 
                            case 12 :
                                key_Num = FND_Sharp;
                                break;
                            default :
                                break;
                        }
                    }
                    else;   
                }
                else
                    key_Num = (New_key_data/4)+9;
                    keypad_in();    
           }
           else
           Key_off_flag = 1;                   
}

void rst_buf()
{    
    int a;
    for(a=0;a<10;a++)
    {
        buf_seg[a] = 0;
    }
}

void myDelay_us(unsigned int delay)
{
    int a;
    for(a=0; a<delay; a++)
    {
        delay_us(1);
    }
}

void SSound(int time)
{
    int b, tim;
    tim = 50000/time;
    for(b=0; b<tim; b++)
    {
        PORTG |= 1<<PORTG4;
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4);
        myDelay_us(time);
    }
}

void PassSound()
{
    DDRG =0x10;
    SSound(Do);
    delay_ms(10);
    SSound(Re);
    delay_ms(10);
    SSound(Mi);
    delay_ms(10);
    SSound(Fa);
    delay_ms(10);
    SSound(Sol);
    delay_ms(10);
    LCD_Init();
    LCD_Pos(0,3);
    LCD_Str(correct);
    delay_ms(2000);    
}   

void WrongSound()
{
    DDRG =0x10;
    SSound(Fa);
    delay_ms(10);
    SSound(Fa);
    delay_ms(10);
}

void Init_Timer2(void)
{
    TIMSK = (1<<TOIE2); 
    TCCR2 = (1<<CS20);
    TCNT2 = -1;
    SREG |= 0x80;
}
 
interrupt [TIM2_OVF] void timer2_overflow(void)
{
    TCNT2 = -1;
    if(go_tim2)
    {   
        count++;
        if(count==10000)
        {
            count = 1000;
        }
    }     
}

void RandomNumber()
{
    four = (char)count / 1000;
    three = ((char)count % 1000) / 100;
    two = (((char)count % 1000) % 100) / 10;
    one = (((char)count % 1000) % 100) % 10;
    
    four = four + 48;
    three = three + 48;
    two = two + 48;
    one = one + 48;  
    
    rand_num[0] = three;
    rand_num[1] = one;
    rand_num[2] = four;
    rand_num[3] = two;    
}

void Init_Timer0(void)
{
    TIMSK = (1<<TOIE0);
    TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00);
    TCNT0 = -5;
    SREG |= 0x80;
}

interrupt [TIM0_OVF] void timer0_overflow(void)
{
    TCNT0= -5; // ī���� �� �缳��(684ms)
    if(go_tim0)
    {
        cnt++;
        if(cnt == 2881) 
        {
            sec++;
            sec1 = ~sec1;
            cnt = 0;
        }
        if(sec == 3) 
        { 
            sec3 = ~sec3;  
            sec_escape++;
            sec = 0;
        }
    } 
} 

void EasyMode()
{
    LCD_Init();
    LCD_Pos(0,6);
    LCD_Str(rand_num);    
} 

void NormalMode()
{
    LCD_Init();
    LCD_Pos(i,6);
    LCD_Str(rand_num);
    i=~i;          
}
void Hard_j()
{
    j = (j * 3) % 10;
}

void HardMode()
{   
    LCD_Init(); 
    Hard_j();
    LCD_Pos(i,j);
    LCD_Str(rand_num);
    i=~i;
}

void ModeSelect()
{ 
    key_Num = 0;
    Keymain();

    select = 0;
    while(1)
    {   
        LCD_Init();
        LCD_Pos(0,1);
        LCD_Str(mode);
        if(key_Num == 10) 
        {
            LCD_Pos(1,8);
            LCD_Str(easy);
            select = 1;
        }
        else if(key_Num == 11) 
        {
            LCD_Pos(1,8);
            LCD_Str(normal);
            select = 2;
        }
        else if(key_Num == 12) 
        {
            LCD_Pos(1,8);
            LCD_Str(hard);
            select = 3;
        }
        else if(key_Num == 13) break;
    }                           
}

void DisNum()
{
    LCD_Init();
    go_tim0 = ~go_tim0;
    while(1)
    {   
        if(sec3)
        {
            if(select == 1)
            {
                EasyMode();
            }
            else if(select == 2)
            {
                NormalMode();
            }
            else if(select == 3)
            {
                HardMode();
            }
            if(sec_escape == 5)
            {
                sec_escape = 0;
                go_tim0 = ~go_tim0;
                break;   
            }
        }
    }
    go_tim0 = ~go_tim0;
}
     
void EntAnswer()
{
    unsigned int a = 0, compp = 0; 
    rst_buf();
    Keymain();
    while(1)
    {    
        LCD_Init();
        Keymain();
        LCD_Pos(0,1);
        LCD_Str(answer);
        LCD_Pos(1,5);    
        LCD_Str1(entrand_num);
        for( k=0; k<4; k++)
        {
            entrand_num[i] = buf_seg[i];
        }
        if(entrand_num[3])
        {
            compp = strcmp(entrand_num, rand_num);
            if(compp == 0)
            {
                PassSound();
                rst_buf();
                break;
            }
            else
            {
                WrongSound();
                LCD_Clear();
                LCD_Pos(0,5);
                LCD_Str(wrong);
                delay_ms(2000);
                rst_buf();
                a++;
                if(a>=3)
                {
                    a=0;
                    y=~y;
                    break;
                }
            }     
        }
    } 
} 

void Dis_Fail()
{
    DDRG =0x10;
    SSound(Sol);
    delay_ms(10);
    SSound(Mi);
    delay_ms(10);
    SSound(Do);
    LCD_Init();
    LCD_Pos(0,5);
    LCD_Str(fail);
    delay_ms(2000); 
    y = ~y;
}

void aaa()
{
    Keymain();
    while(1)
        { 
            go_tim2 = 1;
               
                ModeSelect();
                break;     
            
             
        }
}
void  bbb()
{   
    Keymain();
    while(1)
        {
            DisNum();
            EntAnswer();
            if(y == 0)
            {
                break;
            }
            else if(y==1)
            {
                Dis_Fail();
                break;
            }
        }
}    
void main(void)                        
{                                   
     
    init_reg();
    Keymain();
    Init_Timer2(); 
    Init_Timer0();        
    
    LCD_Init();
    LCD_Pos(0,1);
    LCD_Str(game);
    LCD_Pos(1,1);
    LCD_Str(push); 
    while(1)
    {   
        rst_buf();
        aaa();
        rst_buf(); 
        go_tim2 = 0; 
        RandomNumber();
        bbb();
    }              
}  
