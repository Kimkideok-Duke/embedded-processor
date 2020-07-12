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
//0,1,2,3,4,5,6,7,m1,m2�� ������ Ű ����.
unsigned char entmaster[11] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, '\0'};
 
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
            buf_seg[9] = buf_seg[8];    
            buf_seg[8] = buf_seg[7];        
            buf_seg[7] = buf_seg[6];            
            buf_seg[6] = buf_seg[5];    
            buf_seg[5] = buf_seg[4];        
            buf_seg[4] = buf_seg[3];
            buf_seg[3] = buf_seg[2];    
            buf_seg[2] = buf_seg[1];        
            buf_seg[1] = buf_seg[0];         
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