#include "DSP28_Device.h" 
#include "DSP28_Ev.h"
#include "motor.h"

void Init_eva_pwm(void)
{

   EALLOW;
   
 //��ʼ����ʱ��1��pwm1~6�����  
   EvaRegs.T1PR = t1prd;   // TIMʱ��ΪHCLK37.5M  �趨PWM�ز�Ƶ��20K=37.5M/��2*117*8��T1�ж�Ƶ��20K
   EvaRegs.T1CNT = 0x0000;      // ��ʱ��1��������ֵ   
   EvaRegs.T1CON.all = 0x0b40;//8 ��Ƶ,ʹ�ܶ�ʱ����������������ģʽ�������ĶԳ�PWM
  // EvaRegs.T1CON.all = 0x0c40;//16��Ƶ,ʹ�ܶ�ʱ����������������ģʽ���ڲ�ʱ��
   EvaRegs.DBTCONA.all = 0x03f8;//32��Ƶ������ʱ��2.5us����,ʱ��PWM1-PWM6������������ʱ����3.��37.5/32)
   
   EvaRegs.DBTCONA.all =0;
   EvaRegs.GPTCONA.bit.T1TOADC =2;//��ʱ��1�����ж�����ADC��AD��������20K
   EvaRegs.EVAIMRA.bit.T1PINT=1;//��ʱ��1�����ж�����
   EvaRegs.EVAIFRA.bit.T1PINT=1;//�����־

   // ʹ�� PWM1-PWM6
   EvaRegs.CMPR1 = t1prd;
   EvaRegs.CMPR2 = t1prd;
   EvaRegs.CMPR3 = t1prd;  
   EvaRegs.COMCONA.all = 0x8200;//ʹ�ܱȽϲ����ppwm1~6����Ӧ�ıȽ��߼��������ȽϼĴ���CMPRx��װ����T1CNT=0����


   // ��ʼ����ʱ��2 
   EvaRegs.T2PR =t2prd;       // ��ʱ��2������Ϊ20k=37.5/(117*16)  
   EvaRegs.T2CNT =0x0000;      // Timer2 counter   
   EvaRegs.T2CON.all =0x1440;//16��Ƶ,ʹ�ܶ�ʱ��������������ģʽ   
   EvaRegs.EVAIMRB.bit.T2PINT = 1;//��ʱ��2�����ж�����
   EvaRegs.EVAIFRB.bit.T2PINT = 1;//�����־  
   
   EvaRegs.EVAIFRA.bit.PDPINTA=1;//���PDPINTA�жϱ�־
   EvaRegs.EVAIMRA.bit.PDPINTA=1;//PDPINTA�ж�����
   EDIS;
} 



