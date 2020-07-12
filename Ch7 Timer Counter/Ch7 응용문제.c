#include <mega128.h>
#include <delay.h>            

#define FND_Null        0
#define FND_Star        14
#define FND_Sharp       15

#define FND_DIG PORTE // FND �ڸ���
#define FND_DATA PORTB // FND ������ 

unsigned int Port_char[] ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x88,0x83,0xc4,0xa1,0x84,0x8e,0xff,0xbf};  // 0~F �� ����ǥ
unsigned int Port_fnd[] ={0x1f,0x2f,0x4f,0x8f,0x0f};
// FND0 ON, FND1 ON, FND2 ON, FND3 ON, ALL FND OFF
unsigned char New_key_data=0, key_Num=0;                          
unsigned int buf_seg[4] = {FND_Null,FND_Null,FND_Null,FND_Null}; 
bit Key_off_flag=0;
unsigned int sec, min;
unsigned int count = 0;

void init_reg(void)
{
    DDRE = 0xf0;            //FND Sel
    DDRB = 0xff;            //FND Data Line
    DDRC = 0x0f;            // ���� 4bit Col(�Է�), ���� 4bit Row(���)
    PORTC = 0x0f;           //Port �ʱ�ȭ       
    PORTE = Port_fnd[4];    //ALL FND OFF   
}

void Print_Segment(unsigned int* seg_value)
{

      PORTE = Port_fnd[0]; 
      PORTB = Port_char[seg_value[0]]; 
      delay_ms(1);          
      PORTE = Port_fnd[1]; 
      PORTB = Port_char[seg_value[1]];
      delay_ms(1);        
      PORTE  = Port_fnd[2]; 
      PORTB =  Port_char[seg_value[2]];
      delay_ms(1);                     
      PORTE = Port_fnd [3]; 
      PORTB = Port_char[seg_value[3]];  
      delay_ms(1);                                     
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
 
void Init_Timer0(void)
{
    TIMSK = (1<<TOIE0); // �� �����÷ο�(TOV0) ���ͷ�Ʈ ����
    TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00); // �� �Ϲ� ���, 1024���ֺ� ���
    TCNT0 = -5; // �� ī���� �� ����(1clk = 694ms)
    SREG |= 0x80; // �����ͷ�Ʈ ���۽���(��ü���ͷ�Ʈ�㰡)
}

interrupt [TIM0_OVF] void timer0_overflow(void)
{
    TCNT0= -5; // ī���� �� �缳��(684ms)
    count++;
    if(count == 2881) // 1�ʰ� ��������
    {
        sec++;     //sec�� ����
        count = 0; //ī��Ʈ�� �ʱ�ȭ 
    }
    if(sec == 60) //sec�� 60�� �� ��
    {
        sec = 0;   //sec�� �ʱ�ȭ
        min++;     //min�� ����
    }
}

void FND_Display(unsigned int Min, unsigned int Sec)
{
    FND_DIG = Port_fnd[0]; // ���� 0�� �ڸ�
    FND_DATA = Port_char[Sec%10];
    delay_ms(1);


    FND_DIG = Port_fnd[1]; // ���� 10�� �ڸ�
    FND_DATA = Port_char[Sec/10];
    delay_ms(1);
 
    if(Sec % 2) 
    {
        FND_DIG = Port_fnd[2]; // ���� 1�� �ڸ�
        FND_DATA = Port_char[Min%10] & 0x7F;
        delay_ms(1);
    }
    else 
    {
        FND_DIG = Port_fnd[2]; // ���� 1�� �ڸ�
        FND_DATA = Port_char[Min%10];
        delay_ms(1);
    }

    FND_DIG = Port_fnd [3]; // ���� 10�� �ڸ�
    FND_DATA = Port_char[Min/10];
    delay_ms(1);
} 
                                 
void main(void)                        
{                                   
      init_reg();
      sec = 0;
      min = 0;           
      while(1)                 
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
            Key_off_flag=1;       
        Print_Segment(buf_seg);
               
        sec = buf_seg[0]+(10*buf_seg[1]); //buf_seg[0]���� ���� 1���ڸ�, buf_seg[1]���� ���� 10���ڸ��� ����
        min = buf_seg[2]+(10*buf_seg[3]); //buf_seg[2]���� ���� 1���ڸ�, buf_seg[3]���� ���� 10���ڸ��� ����               
        while(buf_seg[3])//buf_seg[3]�� ���� ���ð�� ���ѷ��� ���� (�ð� ����)
        {
            Init_Timer0();
            init_reg();
            FND_Display(min, sec);    
        }   
    }
}