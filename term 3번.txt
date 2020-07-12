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

unsigned char i = 0; 
bit y = 0;     

unsigned char New_key_data=0, key_Num=0;                          
unsigned int buf_seg[11] = {FND_Null, FND_Null, FND_Null, FND_Null,
                             FND_Null, FND_Null, FND_Null, FND_Null,    
                             FND_Null, FND_Null, '\0'};
bit Key_off_flag=0;

unsigned char door[] = "DOOR OPEN";
unsigned char anykey[] = "PRESS ANYKEY";
unsigned char master[] = "ENTER MASTERKEY";
unsigned char masterset[] = "MASTERKEY SET";
unsigned char entpass[] = "ENTER PASSWORD";
unsigned char pressm1[] = "PRESS M1";
unsigned char close[] = "CLOSE";
unsigned char open[] = "OPEN";

unsigned char password[5] = {32,32,32,32,'\0'};
unsigned char entpassword[5] = {32,32,32,32,'\0'};
unsigned char masternum[11] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, '\0'};
//0,1,2,3,4,5,6,7,m1,m2로 마스터 키 저장.
unsigned char entmaster[11] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, '\0'};
 
void init_reg(void)
{
    DDRE = 0xf0;            //FND Sel
    DDRB = 0xff;            //FND Data Line
    DDRC = 0x0f;            //상위 4bit Col(입력), 하위 4bit Row(출력)
    PORTC = 0x0f;           //Port 초기화          
}

unsigned char KeyScan(void)                     // 4X4 키패드 스캔 함수, 출력 값은 10진수 1~16  {
{
    unsigned int Key_Scan_Line_Sel = 0xf7;      // Init_data 하위 니블의 출력을 결정  
    // 하위 니블(4bit)을 스위칭(연속적으로 돌아가면서)하면서 출력    
    unsigned char Key_Scan_sel=0, key_scan_num=0;         
    unsigned char Get_Key_Data=0;            // 실제 키 데이터        
      
    //키스캔 부분  
    for(Key_Scan_sel=0; Key_Scan_sel<4; Key_Scan_sel++)     
    {           
          // 초기화 
          PORTC = Key_Scan_Line_Sel;               
          delay_us(10);                                
            
          //수신 부분
          Get_Key_Data = (PINC & 0xf0);   // 74LS14의 데이터 취득      
            
          if(Get_Key_Data != 0x00)
          {                  
                switch(Get_Key_Data)        // C포트 내용의 상위 니블(4bit)만 받음            
                {
                      case 0x10:            // 0001이 맞으면 현재 count값에 4를 곱한후                                        
                                            //  1을 더하고 key_scan_num변수에 저장
                            key_scan_num = Key_Scan_sel*4 + 1;                
                            break;                
                      case 0x20:            // 0010이 맞으면 현재 count값에 4를 곱한후
                                            //  2를 더하고 key_scan_num변수에 저장
                            key_scan_num = Key_Scan_sel*4 + 2;                
                            break;                
                      case 0x40:            // 0100이 맞으면 현재 count값에 4를 곱한후 
                                            //  3를 더하고 key_scan_num변수에 저장           
                            key_scan_num = Key_Scan_sel*4 + 3;                
                            break;                
                      case 0x80:            // 1000이 맞으면 현재 count값에 4를 곱한후                                                      
                                            //  4를 더하고 key_scan_num변수에 저장                                         
                            key_scan_num = Key_Scan_sel*4 + 4;                 
                            break;
                      default :
                            key_scan_num = FND_Null; 
                            break;                
                }           
                return key_scan_num;       
          }               
          Key_Scan_Line_Sel = (Key_Scan_Line_Sel>>1);
        //Init_data를 시프트 함, Key_Scan_Line 이동     
    } 
    return key_scan_num;         
}

unsigned char keypad_in()               // 채터링 방지 구문 
{
       if(Key_off_flag)                   // Key_off_flag가 1일 경우, 즉 키를 눌렀을 경우
      {                   
            buf_seg[9] = buf_seg[8];    
            buf_seg[8] = buf_seg[7];        
            buf_seg[7] = buf_seg[6];            
            buf_seg[6] = buf_seg[5];    
            buf_seg[5] = buf_seg[4];        
            buf_seg[4] = buf_seg[3];
            buf_seg[3] = buf_seg[2];    
            buf_seg[2] = buf_seg[1];        
            buf_seg[1] = buf_seg[0];         
            Key_off_flag = ~Key_off_flag;    // 키 입력을 한번만 받기 위한 Key_off_flag 반전
      }
      else;
           buf_seg[0] = key_Num;           // 누른 키값을 세그먼트에 표현 □□□■ 
      return key_Num;                       // 전역변수 key_Num 리턴
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
    LCD_Str(open);
    delay_ms(3000);    
}   

void WrongSound()
{
    DDRG =0x10;
    SSound(Fa);
    delay_ms(10);
    SSound(Fa);
    delay_ms(10);
}

void first()
{
    while(1)
    {
        Keymain();
        LCD_Init();
        LCD_Pos(0,3);
        LCD_Str(door);
        LCD_Pos(1,1);
        LCD_Str(anykey);
        
        if(key_Num) 
        {
            delay_ms(1000);
            rst_buf();
            break;
        }
    }
}

void masterkey()
{
    while(1)
    {
        LCD_Init();
        Keymain();
        LCD_Pos(0,0);
        LCD_Str(master);
        LCD_Pos(1,1);    
        LCD_Str1(masternum);
        for(i =0; i<10; i++)
        {
            masternum[i] = buf_seg[i];
        }
        if(masternum[9])
        {
            rst_buf();
            LCD_Clear();
            LCD_Pos(0,2);
            LCD_Str(masterset);
            delay_ms(3000);
            break;
        }    
    }
}
void passkey()
{
    while(1)
    {
        Keymain();
        LCD_Init();
        LCD_Pos(0,1);
        LCD_Str(entpass);
        LCD_Pos(1,5);
        LCD_Str1(password);
        for(i =0; i<4; i++)
        {
            password[i] = buf_seg[i];
        }
        if(password[3])
        {
            rst_buf();
            break;
        }
    }
}

void m1()
{
    while(1)
    {
        Keymain();
        LCD_Init();
        LCD_Pos(0,3);
        LCD_Str(pressm1);
        
        if(key_Num == 10) 
        {
            LCD_Init();
            LCD_Pos(0,3);
            LCD_Str(close);
            delay_ms(3000);
            break;
        }
    }
}

void entpasskey()
{
    unsigned int a = 0, compp = 0; 
    rst_buf();
    while(1)
    {    
        LCD_Init();
        Keymain();
        LCD_Pos(0,1);
        LCD_Str(entpass);
        LCD_Pos(1,5);    
        LCD_Str1(entpassword);
        for(i =0; i<4; i++)
        {
            entpassword[i] = buf_seg[i];
        }
        if(entpassword[3])
        {
            compp = strcmp(entpassword, password);
            if(compp == 0)
            {
                PassSound();
                rst_buf();
                break;
            }
            else
            {
                WrongSound();
                delay_ms(1000);
                rst_buf();
                a++;
                if(a>=2)
                {
                    a=0;
                    y=~y;
                    break;
                }
            }     
        }
    }        
}

void entermaster()
{
    unsigned int compm = 0;
    while(1)
    {
        LCD_Init();
        Keymain();
        LCD_Pos(0,0);
        LCD_Str(master);
        LCD_Pos(1,1);    
        LCD_Str1(entmaster);
        for(i =0; i<10; i++)
        {
            entmaster[i] = buf_seg[i];
        }
        if(entmaster[9])
        {
            compm = strcmp(masternum,entmaster);
            if(compm == 0)
            {
                PassSound(); 
                rst_buf();
                y=~y;
                break;
            }
            else
            {
                WrongSound();
                rst_buf();
            } 
        }
    }
}
                                
void main(void)                        
{                                   
     init_reg();
     DDRG = 0x10;;
     Keymain();
     LCD_Init();
     first();
     masterkey();
     passkey();
     m1();
     while(1)
     {
        entpasskey();
        if(y==1)
        { 
            entermaster();
        }
     }
}