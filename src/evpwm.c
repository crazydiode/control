#include "DSP28_Device.h" 
#include "DSP28_Ev.h"
#include "motor.h"

void Init_eva_pwm(void)
{

   EALLOW;
   
 //初始化定时器1和pwm1~6的输出  
   EvaRegs.T1PR = t1prd;   // TIM时钟为HCLK37.5M  设定PWM载波频率20K=37.5M/（2*117*8）T1中断频率20K
   EvaRegs.T1CNT = 0x0000;      // 定时器1计数器初值   
   EvaRegs.T1CON.all = 0x0b40;//8 分频,使能定时器操作，连续增减模式，产生的对称PWM
  // EvaRegs.T1CON.all = 0x0c40;//16分频,使能定时器操作，连续增减模式，内部时钟
   EvaRegs.DBTCONA.all = 0x03f8;//32分频，死区时间2.5us左右,时能PWM1-PWM6死区，死区定时周期3.（37.5/32)
   
   EvaRegs.DBTCONA.all =0;
   EvaRegs.GPTCONA.bit.T1TOADC =2;//定时器1周期中断启动ADC，AD采样周期20K
   EvaRegs.EVAIMRA.bit.T1PINT=1;//定时器1周期中断允许
   EvaRegs.EVAIFRA.bit.T1PINT=1;//清除标志

   // 使能 PWM1-PWM6
   EvaRegs.CMPR1 = t1prd;
   EvaRegs.CMPR2 = t1prd;
   EvaRegs.CMPR3 = t1prd;  
   EvaRegs.COMCONA.all = 0x8200;//使能比较操作pwm1~6由相应的比较逻辑驱动。比较寄存器CMPRx重装条件T1CNT=0下溢


   // 初始化定时器2 
   EvaRegs.T2PR =t2prd;       // 定时器2的周期为20k=37.5/(117*16)  
   EvaRegs.T2CNT =0x0000;      // Timer2 counter   
   EvaRegs.T2CON.all =0x1440;//16分频,使能定时器操作，连续增模式   
   EvaRegs.EVAIMRB.bit.T2PINT = 1;//定时器2周期中断允许
   EvaRegs.EVAIFRB.bit.T2PINT = 1;//清除标志  
   
   EvaRegs.EVAIFRA.bit.PDPINTA=1;//清除PDPINTA中断标志
   EvaRegs.EVAIMRA.bit.PDPINTA=1;//PDPINTA中断允许
   EDIS;
} 



