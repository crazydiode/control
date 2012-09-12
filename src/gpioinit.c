#include "DSP28_Device.h"
#include "DSP28_Gpio.h"

void Gpioinit(void)
{

  EALLOW;  
  //PWM1~PWM6  
  GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0=1;  
  GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1=1;
  GpioMuxRegs.GPAMUX.bit.PWM3_GPIOA2=1;
  GpioMuxRegs.GPAMUX.bit.PWM4_GPIOA3=1;
  GpioMuxRegs.GPAMUX.bit.PWM5_GPIOA4=1;
  GpioMuxRegs.GPAMUX.bit.PWM6_GPIOA5=1;  
  //CAP1~CAP3
  GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8 =1;
  GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9 =1;
  GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10 =1;
  
  GpioMuxRegs.GPBMUX.bit.CAP4Q1_GPIOB8 =1;
  GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9 =1;
  GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10 =1;
  
  //GPDPINTA
  GpioMuxRegs.GPDMUX.bit.T1CTRIP_PDPA_GPIOD0 =1;

  //按键 PB.0 PB.1
	GpioMuxRegs.GPBMUX.bit.PWM7_GPIOB0=0;
	GpioMuxRegs.GPBMUX.bit.PWM8_GPIOB1=0;
    GpioMuxRegs.GPBDIR.bit.GPIOB0 =0;
 	GpioMuxRegs.GPBDIR.bit.GPIOB1 =0;

  //LED用于过压过流显示
  GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11 =0;//led1
  GpioMuxRegs.GPADIR.bit.GPIOA11 =1;  //方向为输出
  GpioDataRegs.GPADAT.bit.GPIOA11 =0;//默认为灭

  GpioMuxRegs.GPAMUX.bit.TCLKINA_GPIOA12=0;//led2
  GpioMuxRegs.GPADIR.bit.GPIOA12 =1;
  GpioDataRegs.GPADAT.bit.GPIOA12 =0;//默认为灭

  GpioMuxRegs.GPAMUX.bit.C1TRIP_GPIOA13 =0;//led3
  GpioMuxRegs.GPADIR.bit.GPIOA13 =1;
  GpioDataRegs.GPADAT.bit.GPIOA13 =0;//默认为灭  

  GpioMuxRegs.GPFMUX.all = 0x3F00; 
  
  EDIS;  
}


