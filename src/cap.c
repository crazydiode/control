#include "DSP28_Device.h"     // DSP281x Headerfile Include File
#include "DSP28_Ev.h" 

void Capinit(void)
{  
   EALLOW;
   EvaRegs.CAPCON.bit.CAPRES=0;   
   EvaRegs.CAPCON.bit.CAPQEPN=1;//cap1~2使能
   EvaRegs.CAPCON.bit.CAP3EN=1;   //cap3使能
   EvaRegs.CAPCON.bit.CAP12TSEL=0;//选择定时器2作为时基，定时器1用于产生PWM波
   EvaRegs.CAPCON.bit.CAP3TSEL=0;//选择定时器2作为时基，定时器1用于产生PWM波
   EvaRegs.CAPCON.bit.CAP1EDGE=3;  //双沿捕获
   EvaRegs.CAPCON.bit.CAP2EDGE=3;
   EvaRegs.CAPCON.bit.CAP3EDGE=3;
 
   EvaRegs.EVAIFRC.bit.CAP1INT=1; //清除中断标志 
   EvaRegs.EVAIFRC.bit.CAP2INT=1;
   EvaRegs.EVAIFRC.bit.CAP3INT=1;
   EvaRegs.EVAIMRC.bit.CAP1INT=1; //中断使能
   EvaRegs.EVAIMRC.bit.CAP2INT=1;
   EvaRegs.EVAIMRC.bit.CAP3INT=1;   
   
   EvaRegs.CAPFIFO.bit.CAP1FIFO =1;    //2个捕获才会触发一次中断，所以先设一个值
   EvaRegs.CAPFIFO.bit.CAP2FIFO =1;
   EvaRegs.CAPFIFO.bit.CAP3FIFO =1;
   EDIS;
   
}



