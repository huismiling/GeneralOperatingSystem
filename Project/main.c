#include"in430.h"
#include "msp430g2553.h"

#include "TaskManage.h"

void delay(unsigned int t);
void task1();
void task2();

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD; // 关闭看门狗
  
  P1DIR = 0xff; //设定P1口为输出方式
  P1OUT = 0xff; //给P1口赋初始值
  
  TA0CCTL0 = CCIE; //打开中断 (Timer0_A3 Capture/Compare Control 0 )
  TA0CCR0 = 60000; //设定计数变量
  TA0CTL = TASSEL_2 + MC_2; //定时器选择时钟SMCLK，使用模式二连续计数方式
  
  AddTask(task1,7);
  AddTask(task2,3);
  _EINT(); //开总中断
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


