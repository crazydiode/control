#include "DSP28_Device.h"     // DSP281x Headerfile Include File
#include "DSP28_Ev.h" 

void Capinit(void)
{  
   EALLOW;
   EvaRegs.CAPCON.bit.CAPRES=0;   
   EvaRegs.CAPCON.bit.CAPQEPN=1;//cap1~2ʹ��
   EvaRegs.CAPCON.bit.CAP3EN=1;   //cap3ʹ��
   EvaRegs.CAPCON.bit.CAP12TSEL=0;//ѡ��ʱ��2��Ϊʱ������ʱ��1���ڲ���PWM��
   EvaRegs.CAPCON.bit.CAP3TSEL=0;//ѡ��ʱ��2��Ϊʱ������ʱ��1���ڲ���PWM��
   EvaRegs.CAPCON.bit.CAP1EDGE=3;  //˫�ز���
   EvaRegs.CAPCON.bit.CAP2EDGE=3;
   EvaRegs.CAPCON.bit.CAP3EDGE=3;
 
   EvaRegs.EVAIFRC.bit.CAP1INT=1; //����жϱ�־ 
   EvaRegs.EVAIFRC.bit.CAP2INT=1;
   EvaRegs.EVAIFRC.bit.CAP3INT=1;
   EvaRegs.EVAIMRC.bit.CAP1INT=1; //�ж�ʹ��
   EvaRegs.EVAIMRC.bit.CAP2INT=1;
   EvaRegs.EVAIMRC.bit.CAP3INT=1;   
   
   EvaRegs.CAPFIFO.bit.CAP1FIFO =1;    //2������Żᴥ��һ���жϣ���������һ��ֵ
   EvaRegs.CAPFIFO.bit.CAP2FIFO =1;
   EvaRegs.CAPFIFO.bit.CAP3FIFO =1;
   EDIS;
   
}



