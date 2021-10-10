/*********************************

   5KG   LOADCELL Driver  with HX711

**********************************/

#include <mega32a.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdlib.h>

// Declare your global variables here
#define HX711_DOUT    PIND.3
#define HX711_SCK     PORTD.2
#define HX_COMPLEMENT 0x800000
#define HX_REF        0.360598//360.91549
//0.360598
#define HX711_DOUT1    PIND.5
#define HX711_SCK1     PORTD.4
#define HX_REF1        0.360598//9.33 

#define HX711_DOUT2    PINB.1
#define HX711_SCK2     PORTB.0
#define HX_REF2        0.360598//9.33 

void sort_array(float a[], int size);
void HX711_Read(void);
void get_scale(void);
void get_offset(void);
//unsigned long HX711_Read1(void);
//void get_scale1(void);
//void get_offset1(void);
//unsigned long HX711_Read2(void);
//void get_scale2(void);
//void get_offset2(void);
//void motor(void);
//void delay_msec(long int ha);
unsigned long Weight_Shiwu = 0,Weight_Maopi =0,Weight_Shiwu1 = 0,Weight_Maopi1 =0,Weight_Shiwu2 = 0,Weight_Maopi2 =0; 
unsigned long HX711_Buffer = 0,HX711_Buffer1 = 0,HX711_Buffer2 = 0,HX711_Buffer_sum = 0,HX711_Buffer1_sum = 0,HX711_Buffer2_sum = 0;
float         scale = 0,scale1 = 0,scale2 = 0 ,scale_array[25],scale1_array[25],scale2_array[25];
unsigned int timer2_counter,pw=75;//i,i2,i3,sw=0,i2;
unsigned char  buffer1[12];
unsigned char  buffer2[12];
unsigned char  buffer3[12];
char  buffer[30];
char  str1[10];
char  str2[10];
char  str3[10];

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer2 value
TCNT2=0xB2;
// Place your code here
if(timer2_counter == pw ) PORTC.1 = 0;
timer2_counter += 1;
if(timer2_counter >= 100){ timer2_counter = 0; PORTC.1 = 1;}
}

void send( unsigned char Data1[12], unsigned char Data2[12], unsigned char Data3[12]);
unsigned char buffer1[12];
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 24
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index=0,rx_rd_index=0;
#else
unsigned int rx_wr_index=0,rx_rd_index=0;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0) rx_buffer_overflow=1;
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif
void main(void)
{

PORTA=0x00;
DDRA=0x00;

//DDRD = (1<<DDD0);
///////////////////////
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=Out Bit0=Out 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=In Bit2=Out Bit1=Out Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (1<<DDD2) | (1<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=T Bit2=0 Bit1=0 Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
/////////////////////////// 
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

get_offset();
//get_offset1();
//get_offset2();

 
lcd_init(16);
lcd_putsf("Doctor naderi");
delay_ms(1000);

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 9.984 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0xB2;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// Global enable interrupts
#asm("sei")

while (1)
      {
        get_scale(); 
        //get_scale1();
        //get_scale2();  
        ftoa(scale,2,str1);  
        sprintf(buffer1,"%s",str1); 
        ftoa(scale1,2,str2);  
        sprintf(buffer2,"%s",str2);
        ftoa(scale2,2,str3);  
        sprintf(buffer3,"%s",str3);
        send(buffer1,buffer2,buffer3); 
        sprintf(buffer," %4.1fg %4.1fg %4.1fg",scale,scale1,scale2);//sprintf(buffer," %ld ",Weight_Shiwu);//sprintf(buffer," %1.3f --> Gram",scale);
        lcd_clear();  
        lcd_gotoxy(0,0);
        lcd_puts(buffer); 
//        get_scale1();
//        sprintf(buffer,"%4.1fg",scale1);//sprintf(buffer," %ld ",Weight_Shiwu1);//sprintf(buffer," %1.3f --> Gram",scale);
//        lcd_gotoxy(0,8);
//        lcd_puts(buffer);
//        get_scale2();
//        sprintf(buffer,"%4.1fg",scale2);//sprintf(buffer," %ld ",Weight_Shiwu2);//sprintf(buffer," %1.3f --> Gram",scale);
//        lcd_gotoxy(1,0);
//        lcd_puts(buffer);
        delay_ms(10);
        //motor();
      }
}


void HX711_Read(void)
{    
    unsigned char i;
    unsigned long count = 0;  
    unsigned long count1 = 0;
    unsigned long count2 = 0;  
    
      
    HX711_Buffer = 0;
    HX711_Buffer1 = 0;
    HX711_Buffer2 = 0;
    
      HX711_DOUT=1;
      HX711_SCK=0;
      HX711_DOUT1=1;
      HX711_SCK1=0;
      HX711_DOUT2=1;
      HX711_SCK2=0;
      
      while(HX711_DOUT);
      for(i=0;i<24;i++)  //gain --> 128
        {
            HX711_SCK=1;
            HX711_SCK1=1;
            HX711_SCK2=1;
            count=count<<1;
            count1=count1<<1;
            count2=count2<<1;
            //delay_us(1);
            HX711_SCK=0; 
            HX711_SCK1=0;
            HX711_SCK2=0;
                       
            if(HX711_DOUT)
                count++;
            if(HX711_DOUT1)
                count1++;
            if(HX711_DOUT2)
                count2++;
        }
           
         
       HX711_SCK=1;
       HX711_SCK1=1;
       HX711_SCK2=1;
       
      count=count ^ HX_COMPLEMENT;//2,s Complement 
      count1=count1 ^ HX_COMPLEMENT;//2,s Complement
      count2=count2 ^ HX_COMPLEMENT;//2,s Complement
      
      HX711_SCK=0;
      HX711_SCK1=0;
      HX711_SCK2=0; 
      
      HX711_Buffer = count;
      HX711_Buffer1 = count1;
      HX711_Buffer2 = count2;
}//HX711_Read


void get_scale(void)
{
    unsigned int i;
    unsigned int j;  
    
    HX711_Buffer_sum = 0;
    HX711_Buffer1_sum = 0;
    HX711_Buffer2_sum = 0;
    
    
    for(j=0 ; j<4 ;j++){     
    for(i=0 ; i<25 ;i++){
    HX711_Read();

    HX711_Buffer = HX711_Buffer/100;
    if(HX711_Buffer > Weight_Maopi)
    {
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi;

        Weight_Shiwu = ((float)Weight_Shiwu/HX_REF);

        scale_array[i] = (float)Weight_Shiwu/10;
    }  
    else scale_array[i] = (float)0;


    //scale = (scale_array[0] + scale_array[1] + scale_array[2]) / 3;

    HX711_Buffer1 = HX711_Buffer1/100;
    if(HX711_Buffer1 > Weight_Maopi1)
    {
        Weight_Shiwu1 = HX711_Buffer1;
        Weight_Shiwu1 = Weight_Shiwu1 - Weight_Maopi1;

        Weight_Shiwu1 = ((float)Weight_Shiwu1/HX_REF);

        scale1_array[i] = (float)Weight_Shiwu1/10;
    }  
    else scale1_array[i] = (float)0;


    //scale1 = (scale1_array[0] + scale1_array[1] + scale1_array[2]) / 3;   

    HX711_Buffer2 = HX711_Buffer2/100;
    if(HX711_Buffer2 > Weight_Maopi2)
    {
        Weight_Shiwu2 = HX711_Buffer2;
        Weight_Shiwu2 = Weight_Shiwu2 - Weight_Maopi2;

        Weight_Shiwu2 = ((float)Weight_Shiwu2/HX_REF);

        scale2_array[i] = (float)Weight_Shiwu2/10;
    } 
    else scale2_array[i] = (float)0;
    }
    //scale2 = (scale2_array[0] = scale2_array[1] = scale2_array[2]) / 3;     
        sort_array(scale_array,25);
        sort_array(scale1_array,25);
        sort_array(scale2_array,25);
        HX711_Buffer_sum += scale_array[12]/100;
        HX711_Buffer1_sum += scale1_array[12]/100;
        HX711_Buffer2_sum += scale2_array[12]/100;
    }
    scale = (float)HX711_Buffer_sum / 4 ;
    scale1 = (float)HX711_Buffer1_sum / 4 ;
    scale2 = (float)HX711_Buffer2_sum / 4 ;
}


void get_offset(void)
{
    int i=0;

    HX711_Buffer_sum = 0;
    HX711_Buffer1_sum = 0;
    HX711_Buffer2_sum = 0;    
   	for( i = 0 ; i < 10 ; i++)
	{   
        HX711_Read();
		HX711_Buffer_sum += HX711_Buffer ;
        HX711_Buffer1_sum += HX711_Buffer1 ;
        HX711_Buffer2_sum += HX711_Buffer2 ;
        delay_us(500);
	}
    HX711_Buffer_sum = HX711_Buffer_sum /10;     
    Weight_Maopi = HX711_Buffer_sum/100;  
    HX711_Buffer1_sum = HX711_Buffer1_sum /10;     
    Weight_Maopi1 = HX711_Buffer1_sum/100;
    HX711_Buffer2_sum = HX711_Buffer2_sum /10;     
    Weight_Maopi2 = HX711_Buffer2_sum/100;

}//get_offset


void send( unsigned char Data1[12], unsigned char Data2[12], unsigned char Data3[12])//,unsigned char Data2[12] )
{
//char ATN=0X44;
putchar('A');
puts(Data1);
putchar('E');
putchar('B');
puts(Data2);
putchar('E');
putchar('C');
puts(Data3);
putchar('N');
//puts(Data2);
//putchar('M');
}

void sort_array(float a[], int size) {
    int i=0;
    int o=0;
    for(i=0; i<(size-1); i++) {
        for(o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}