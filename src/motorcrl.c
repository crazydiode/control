#include "DSP28_Device.h"
#include "motor.h"

void startmotor(void)
{
    EALLOW;
    EvaRegs.CMPR1 = pwm;
    EvaRegs.CMPR2 = pwm;
    EvaRegs.CMPR3 = pwm;

   	GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=0;//�趨cap1~3Ϊgpio
   	GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=0;
   	GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=0;   
   	GpioMuxRegs.GPADIR.bit.GPIOA8=0;//�趨cap1~3Ϊ����
   	GpioMuxRegs.GPADIR.bit.GPIOA9=0;
   	GpioMuxRegs.GPADIR.bit.GPIOA10=0;
   	capstastus=(GpioDataRegs.GPADAT.all&0x0700)>>8;  //ȡ������ʱ3���������ص�״̬
   	if(dir==1)
    {
     	switch(capstastus)//ir2136 ��hin��lin�Ƿ����
   	 	{
   			case 1: EvaRegs.ACTR.all=0x7fd;break;//h3 fall
   			case 2: EvaRegs.ACTR.all=0xfd7;break;//h1 fall
   			case 3: EvaRegs.ACTR.all=0x7df;break;//h2 rise
   			case 4: EvaRegs.ACTR.all=0xd7f;break;//h2 fall
   			case 5: EvaRegs.ACTR.all=0xf7d;break;//h1 rise
   			case 6: EvaRegs.ACTR.all=0xdf7;break;//h3 rise
   	 	}
    }
    else
    {
    	switch(capstastus)//ir2136 ��hin��lin�Ƿ����
   	 	{
   			case 5: EvaRegs.ACTR.all=0xfd7;break;//h1 rise    
   			case 1: EvaRegs.ACTR.all=0xd7f;break;//h3 fall
   			case 3: EvaRegs.ACTR.all=0xdf7;break;//h2 rise
   			case 2: EvaRegs.ACTR.all=0xf7d;break;//h1 fall
   			case 6: EvaRegs.ACTR.all=0x7fd;break;//h3 rise
   			case 4: EvaRegs.ACTR.all=0x7df;break;//h2 fall
   	 	}
    }
      
   /*************************/
   	GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=1;//�����趨cap1~3Ϊgpio
   	GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=1;
   	GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=1; 
   	
   /***��ʱ��1,2������**/
    //EvaRegs.T1CNT = 0x0000;
    //EvaRegs.T1CON.all = 0x0b40;
//  	EvaRegs.T2CNT = 0x0000;
//    EvaRegs.T2CON.all = 0x1440;//������ʱ��2
   	EDIS;
	
}

void stopmotor(void)
{
   EALLOW;
   EvaRegs.CMPR1 = t1prd;
   EvaRegs.CMPR2 = t1prd;
   EvaRegs.CMPR3 = t1prd;
   //IER&=0xFFFB; 
   //EvaRegs.T2CON.all = 0x1400;//������ʱ��2
   EDIS;
}
