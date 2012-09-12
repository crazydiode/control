#include "DSP28_Device.h"
#include "motor.h"


/***************************************************/
interrupt void t1pint_isr(void);
interrupt void t2pint_isr(void);
interrupt void  cap_isr(void);
interrupt void  pdpinta_isr(void);
void delay(unsigned int m);

unsigned int t1prd=117;//��ʱ��1������
unsigned int t2prd=117;//��ʱ��2�ı�־

unsigned int dir=0;//���ת������
unsigned int pwm=20;

unsigned int dccurent,u,v,w,speedad;//ֱ��ĸ�ߵ���
unsigned int dcvoltage;//ֱ��ĸ�ߵ�ѹ
Uint32 Sumdcv=0;
unsigned int dcvcount=0,Averagedcv=0;//ĸ�ߵ�ѹƽ��ֵ��ʼ��


unsigned int capstastus;//����cap�ڵĵ�ƽ
Uint32 Time,T2cnt,Speed=0,count=0;
unsigned Pole=2;//������
Uint32 Sum=0,Average=0;
unsigned int l=0,nn=0;

int t2=0;
unsigned int test[2000],test1[2000],test2[2000];
int sss=0;
/***************************************************/

void main(void)
{
	/*��ʼ��ϵͳ*/
	InitSysCtrl();

	/*���ж�*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	/*��ʼ��PIE*/
	InitPieCtrl();

	/*��ʼ��PIEʸ����*/
	InitPieVectTable();	
	
	/*��ʼ��GPIO*/
	Gpioinit();
	
	/*��ʼ��AD*/
	InitAdc();
	
	/*��ʼ��PWM*/
	Init_eva_pwm(); 
    
    /*��ʼ��CAP*/
    Capinit();	
	
	EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.CAPINT1=&cap_isr;
    PieVectTable.CAPINT2=&cap_isr;
    PieVectTable.CAPINT3=&cap_isr;
    PieVectTable.T1PINT=&t1pint_isr;
    PieVectTable.T2PINT=&t2pint_isr;
    PieVectTable.PDPINTA=&pdpinta_isr;    
    EDIS;    // This is needed to disable write to EALL
	
	PieCtrl.PIEIER2.bit.INTx4=1;//T1pint�ж�
    PieCtrl.PIEIER3.bit.INTx1=1;//T2pint�ж�
    PieCtrl.PIEIER3.bit.INTx5=1;//Cap1�ж�
    PieCtrl.PIEIER3.bit.INTx6=1;//Cap2�ж�
    PieCtrl.PIEIER3.bit.INTx7=1;//Cap3�ж�
    PieCtrl.PIEIER1.bit.INTx1=1;//pdpinta�ж�    	
	/* ����IER�Ĵ��� */

	IER |= M_INT1;
    IER |= M_INT2; // t1pint enable
    IER |= M_INT3; // capture enable   
    
    
    // Enable global Interrupts and higher priority real-time debug events:
	startmotor();
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
	

    
    
// Step 6. IDLE loop. Just sit and loop forever (optional):	
    while(1)
    {           
	}	
} 	


interrupt void  t2pint_isr(void)
{
  count++; 
  EvaRegs.EVAIFRB.bit.T2PINT=1;//����жϱ�־
  EvaRegs.EVAIMRB.bit.T2PINT=1;//�ж�����    
  PieCtrl.PIEACK.bit.ACK3=1;//��cpu�����ж� 
}


interrupt void  cap_isr(void)
{  
   /*****����������⴫�����������ƽ����������****/
   Uint32 kk=t2prd;
   EALLOW;
   GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=0;//�趨cap1~3Ϊgpio
   GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=0;
   GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=0;   
   GpioMuxRegs.GPADIR.bit.GPIOA8=0;//�趨cap1~3Ϊ����
   GpioMuxRegs.GPADIR.bit.GPIOA9=0;
   GpioMuxRegs.GPADIR.bit.GPIOA10=0;
   capstastus=(GpioDataRegs.GPADAT.all&0x0700)>>8;
   
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
      
   
   /*������������ת��*/
    T2cnt=EvaRegs.T2CNT;//��ȡ��ʱ��2��ֵ
    Time=kk*count+ T2cnt;//�����ת1������ʱ��    
    Sum+=Time;   
    l++;
        
    if(l==12)//ÿת12/6/pole����һ��ת��
    {
      Average=Sum/12;  
              
      Speed=kk*20000*60/(Average*6*Pole);//����ת��        
      Sum=0;      
      l=0;      
      test[nn]=Speed;//�����ã��洢�ٶ�ֵ      
      nn++;      
      if(nn==2000)
      {      	 
      	  nn=0;  
      }

    }
    
    count=0;
    EvaRegs.T2CON.all = 0x1400;//�رն�ʱ��2   
    EvaRegs.T2CNT = 0x0000;
    EvaRegs.T2CON.all = 0x1440;//������ʱ��2
    
   
    /*************************/
    GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=1;//�����趨cap1~3Ϊgpio
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=1;
    GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=1; 
   
    EvaRegs.EVAIFRC.all = 7 ; // �岶׽�ж�
    EvaRegs.CAPFIFO.all = 0x01500; // ��ղ�׽��ջ   
    PieCtrl.PIEACK.bit.ACK3=1;//cap1�ж���cpu�����ж�
}

interrupt void  t1pint_isr(void)
{
    dcvoltage=(AdcRegs.RESULT0)>>4;
  	dccurent=(AdcRegs.RESULT3)>>4;
    w=(AdcRegs.RESULT1)>>4;
    u=(AdcRegs.RESULT2)>>4;  	
    speedad=(AdcRegs.RESULT4)>>4;
    v=(AdcRegs.RESULT5)>>4;     
    
    
    
   //ĸ�ߵ�ѹ��⣬��ѹ������
   //ע���������������ת������ı�ʱ������ĸ�ߵ�ѹ������
   //���ö����ƽ��ֵ
    Sumdcv+=dcvoltage;
    dcvcount++;
    if(dcvcount==500)
    {
    	Averagedcv=Sumdcv/500;//��ƽ��ĸ�ߵ�ѹ
    	Sumdcv=0;
    	dcvcount=0;	
    }    
  		
  	if(Averagedcv>=3000)
  	{
  		stopmotor();
  		GpioDataRegs.GPADAT.bit.GPIOA11 =1;//��ѹ��ʾ	
  	}    
  	
  	/******************************/  
  	AdcRegs.ADC_ST_FLAG.bit.INT_SEQ1_CLR=1; //���״̬��
 	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;//��λseq1
  
  	EvaRegs.EVAIFRA.bit.T1PINT=1;//����жϱ�־
  	EvaRegs.EVAIMRA.bit.T1PINT=1;//�ж����� 
   
  	PieCtrl.PIEACK.bit.ACK2=1;//��cpu�����ж�

}


interrupt void  pdpinta_isr(void)
{
  
  	EvaRegs.EVAIFRA.bit.PDPINTA=1;//���PDPINTA�жϱ�־
  	PieCtrl.PIEACK.bit.ACK1=1;//��cpu�����ж�
  	
    stopmotor();
    GpioDataRegs.GPADAT.bit.GPIOA12 =1;//������ʾ
  	EvaRegs.COMCONA.bit.FCOMPOE=1;//����ʹ�ܱȽ����
 
}

/***********************************************************************/
//	No	more
/***********************************************************************/



