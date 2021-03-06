#include <mega128.h>
#include <delay.h>            

#define FND_Null        0
#define FND_Star        14
#define FND_Sharp       15

#define FND_DIG PORTE // FND 자리수
#define FND_DATA PORTB // FND 데이터 

unsigned int Port_char[] ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x88,0x83,0xc4,0xa1,0x84,0x8e,0xff,0xbf};  // 0~F 의 문자표
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
    DDRC = 0x0f;            // 상위 4bit Col(입력), 하위 4bit Row(출력)
    PORTC = 0x0f;           //Port 초기화       
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
            buf_seg[3] = buf_seg[2];        // 세그먼트 값 시프트  ■□□□ <- □■□□
            buf_seg[2] = buf_seg[1];        // 세그먼트 값 시프트  □■□□ <- □□■□
            buf_seg[1] = buf_seg[0];        // 세그먼트 값 시프트  □□■□ <- □□□■ 
            Key_off_flag = ~Key_off_flag;    // 키 입력을 한번만 받기 위한 Key_off_flag 반전
      }
      else;
           buf_seg[0] = key_Num;           // 누른 키값을 세그먼트에 표현 □□□■ 
      return key_Num;                       // 전역변수 key_Num 리턴
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
    count++;
    if(count == 2881) // 1초가 지났을때
    {
        sec++;     //sec값 증가
        count = 0; //카운트값 초기화 
    }
    if(sec == 60) //sec가 60이 될 때
    {
        sec = 0;   //sec값 초기화
        min++;     //min값 증가
    }
}

void FND_Display(unsigned int Min, unsigned int Sec)
{
    FND_DIG = Port_fnd[0]; // 초의 0의 자리
    FND_DATA = Port_char[Sec%10];
    delay_ms(1);


    FND_DIG = Port_fnd[1]; // 초의 10의 자리
    FND_DATA = Port_char[Sec/10];
    delay_ms(1);
 
    if(Sec % 2) 
    {
        FND_DIG = Port_fnd[2]; // 분의 1의 자리
        FND_DATA = Port_char[Min%10] & 0x7F;
        delay_ms(1);
    }
    else 
    {
        FND_DIG = Port_fnd[2]; // 분의 1의 자리
        FND_DATA = Port_char[Min%10];
        delay_ms(1);
    }

    FND_DIG = Port_fnd [3]; // 분의 10의 자리
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
               
        sec = buf_seg[0]+(10*buf_seg[1]); //buf_seg[0]값을 초의 1의자리, buf_seg[1]값을 초의 10의자리로 세팅
        min = buf_seg[2]+(10*buf_seg[3]); //buf_seg[2]값을 분의 1의자리, buf_seg[3]값을 분의 10의자리로 세팅               
        while(buf_seg[3])//buf_seg[3]에 값이 들어올경우 무한루프 실행 (시계 동작)
        {
            Init_Timer0();
            init_reg();
            FND_Display(min, sec);    
        }   
    }
}