#include"in430.h"
#include "msp430g2553.h"

#include "TaskManage.h"

void delay(unsigned int t);
void task1();
void task2();

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD; // �رտ��Ź�
  
  P1DIR = 0xff; //�趨P1��Ϊ�����ʽ
  P1OUT = 0xff; //��P1�ڸ���ʼֵ
  
  TA0CCTL0 = CCIE; //���ж� (Timer0_A3 Capture/Compare Control 0 )
  TA0CCR0 = 60000; //�趨��������
  TA0CTL = TASSEL_2 + MC_2; //��ʱ��ѡ��ʱ��SMCLK��ʹ��ģʽ������������ʽ
  
  AddTask(task1,7);
  AddTask(task2,3);
  _EINT(); //�����ж�
  P1OUT=0;
  while(1)
  {
    //P1OUT=0;
  }
  
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer1(void)
{
  _EINT();
  TaskSchedule();

}

void delay(unsigned int t)
{
  unsigned int j;
  for(;t>0;t--)
    for(j=0;j<1000;j++)
      ;
}

void task1()
{
  P1OUT^=0x01;
  delay(400);
  TaskDelay(2);
  //P1OUT^=0x01;
}
void task2()
{
  P1OUT^=0x40;
  TaskDelay(3);
  //P1OUT^=0x40;
}


