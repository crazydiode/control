#include "DSP28_Device.h"
#include "comm.h"
#include "mcbsp.h"
#include "gui_string.h"
#include "motor.h"

#define DATATYPE 0 /* 0����edit�˵���1����ѡ��˵�*/

unsigned int i,j;
unsigned int send_flag;
/***************************************************/
interrupt void mrinta_isr(void);
interrupt void t1pint_isr(void);
interrupt void t2pint_isr(void);
interrupt void  cap_isr(void);
interrupt void  pdpinta_isr(void);
void delay(unsigned int m);
void pidcontrol(int u,int y);//���ڵ���
void pidcontrol1(int u,int y);//�����ٶ�

unsigned int t1prd=117;//��ʱ��1������
unsigned int t2prd=117;//��ʱ��2�ı�־

unsigned int dir=1;//���ת������
unsigned int start=0;//�������ֹͣ��־
unsigned int setflag=0;//��ʾ�Ƿ�����ٶ��趨
unsigned int sudusendflag=0,xssdbz=0;//�ٶ��ϴ���־���ٶ���ʾ�ļ���
unsigned int sdxsjs=0;


unsigned int dccurent,u,v,w,speedad;//ֱ��ĸ�ߵ���
unsigned int dcvoltage;//ֱ��ĸ�ߵ�ѹ
Uint32 Sumdcv=0;
unsigned int dcvcount=0,Averagedcv=0;//ĸ�ߵ�ѹƽ��ֵ��ʼ��


unsigned int capstastus;//����cap�ڵĵ�ƽ
Uint32 Time,T2cnt,Speed=0,count=0;
unsigned Pole=2;//������
Uint32 Sum=0,Average=0;
unsigned int l=0,nn=0;
//�ٶȵ�pwm�ĳ�ʼֵ
unsigned int pwm=10;//pwm��ռ�ձȳ�ֵ
float kp=0.1,ki=0.1,kd=0.0;
int Speedset=1500;
int ek=0,ek1=0,ek2=0;
int du;
float duk;

int t2=0;

unsigned int test[2000],test1[5000],test2[2000];
int sss=0;
/***************************************************/

/*******************************************************/

#define menu_num 5 /*ͨ��num������������ֵĸ�����ͨ��gui_string.sam�������ݡ�*/
unsigned int Test;
/////////////////////////////////////////////////////////////////////////
unsigned int Edit_result;
unsigned int i,k;
/////////////////////////////////////////////////////////////////////////
unsigned int mcbspx[FRAMLENGTH];
unsigned int mcbspr[FRAMLENGTH];
unsigned int mcbsp_s;
PmcbspForDec psend;
PmcbspForDec preceive;
/************************************************/
void main(void)
{

#if DATATYPE==0
	Test =FRAME_DATASEND; //0x1  ���ݴ���֡
#endif
#if DATATYPE==1
	Test =FRAME_DATAEND; // 0x3  ���ݴ��ͽ���֡
#endif 
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
	
	/*��ʼ��MCBSP����*/
	InitMcbsp();
	
	EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.CAPINT1=&cap_isr;
    PieVectTable.CAPINT2=&cap_isr;
    PieVectTable.CAPINT3=&cap_isr;
    PieVectTable.T1PINT=&t1pint_isr;
    PieVectTable.T2PINT=&t2pint_isr;
    PieVectTable.PDPINTA=&pdpinta_isr;
    PieVectTable.MRINTA= &mrinta_isr;
    EDIS;    // This is needed to disable write to EALL
	
	PieCtrl.PIEIER2.bit.INTx4=1;//T1pint�ж�
    PieCtrl.PIEIER3.bit.INTx1=1;//T2pint�ж�
    PieCtrl.PIEIER3.bit.INTx5=1;//Cap1�ж�
    PieCtrl.PIEIER3.bit.INTx6=1;//Cap2�ж�
    PieCtrl.PIEIER3.bit.INTx7=1;//Cap3�ж�
    PieCtrl.PIEIER1.bit.INTx1=1;//pdpinta�ж� 
    PieCtrl.PIEIER6.bit.INTx5 = 1;//McBSP�����ж�	
	/* ����IER�Ĵ��� */

	IER |= M_INT1;
    IER |= M_INT2; // t1pint enable
    IER |= M_INT3; // capture enable
    IER |= M_INT6;
    
    
    // Enable global Interrupts and higher priority real-time debug events:
	//startmotor();
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
	
    psend=(PmcbspForDec)(&mcbspx[0]);  
    
    
// Step 6. IDLE loop. Just sit and loop forever (optional):	
    while(1)
    {    
           switch(Test)       
           {
            case FRAME_DATASEND:
 				psend->Length=FRAMLENGTH; 		
 	   			psend->Type=FRAME_DATASEND;  //0x1  ���ݴ���
	   		 	psend->Mutul=FRAME_SING;			
	 		    psend->Data[0]=MOTOR_START;  //0xAA30	���ʵ�鿪ʼ
		    	mcbsp_tx((unsigned int *)psend);
                Test=0;
            break;
            case FRAME_DATAEND:
                psend->Length=FRAMLENGTH; 		
 	   			psend->Type=FRAME_DATAEND;  //0x3  ���ݴ���
	   		 	psend->Mutul=FRAME_SING;			
	 		    psend->Data[0]=MOTOR_OVER;  //0xAA32	���ʵ�����
	  		    mcbsp_tx((unsigned int *)psend);
	  		    Test=0;
            break;
            default:
		    break;  
		}	
		mcbsp_rx(&mcbspr[0]);
   	    mcbsp_s=mcbsp_status();
    	if((mcbsp_s == 0xffff)||(mcbsp_s == 3)){        
		     /*֪ͨ��������ͨѶ����,׼���ط�*/
		   	psend->Length = FRAMLENGTH;
		   	psend->Type = FRAME_CMD;
			psend->Data[0] = CHECK_ERR;
		   	psend->Mutul = FRAME_SING;
			mcbsp_tx((unsigned int *) psend);
		}
		if(mcbsp_s == 0)
		{
			 preceive=(PmcbspForDec)(&mcbspr[0]);
			 if(mcbspr[3]==0xaa38)
			 {
			 	switch(mcbspr[4])
			 	{
			 		case 0xaa35:
			 		{
			 			start=1;
			 			xssdbz=1;
			 			startmotor();break;//�������
			 		}
			 		case 0xaa34:
			 		{
			 			start=0;
			 			xssdbz=1;
			 			stopmotor();//ֹͣ���
			 			setflag=0;
			 			break;
			 		}
			 		case 0xaa36:
			 		{
			 			if(start==1)
			 			{
			 				dir=!dir;//����
			 				stopmotor();
			 				delay(1000);
			 				startmotor();
			 				xssdbz=1;
			 			}
			 			break;
			 		}	
			 		default:break;		 		
			 	}
			 }
			 if(mcbspr[3]==0xaa37)//pid�����趨
			 {
			    xssdbz=1;
			    Speedset=mcbspr[4];
			 	kp=(float)mcbspr[5]/1000;
			 	ki=(float)mcbspr[6]/1000;
			 	kd=(float)mcbspr[7]/1000;
			 	if(start==1)
			 	setflag=1;//��ʾ��ʼ�趨�ٶȣ�pid 
			 	else
			 	setflag=0;
			 }
		}
			 
		if((sudusendflag==1)&&(xssdbz==1))//ÿ��2s�ϴ�һ���ٶ�
		{
			sudusendflag=0;
			psend->Length = FRAMLENGTH;
		   	psend->Type = FRAME_DATASEND;
			psend->Data[0] = 0xaa20;
			if(start==0)//���ֹͣ״̬���ٶ�Ϊ0
			Speed=0x0;
			psend->Data[1] =Speed;
		   	psend->Mutul = FRAME_SING;
			mcbsp_tx((unsigned int *) psend);
		}		
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
    
    sdxsjs++;
    if(sdxsjs==40000)
    {
    	sudusendflag=1;//�ٶ��ϴ���־��λ
    	sdxsjs=0;
    	GpioDataRegs.GPADAT.bit.GPIOA13 =!GpioDataRegs.GPADAT.bit.GPIOA13;//����������ʾ	
    }//�ٶ��ϴ��ü�ʱ
    
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
    else
    {   
        GpioDataRegs.GPADAT.bit.GPIOA11 =0;   	
  		test1[t2]=dcvoltage; 
  		test2[t2]=dccurent;   
  	
  		////�ٶȵ���
  		t2++;
  		if(t2==5000)
  		{
  		    if(setflag==1)
  		    {
   			pidcontrol(Speedset,Speed);//50ms
   			}
  			t2=0;
  		}
  	
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


/**************************************/
interrupt void mrinta_isr(void)     // McBSP-A
{
	PieCtrl.PIEACK.bit.ACK6 = 1;
	if(Mcbsp_RxRdy() == 1)
	{
		RevBuffer[datarevlength] = McbspRegs.DRR1.all;
		datarevlength++;

	}
	EINT;
}


//pid����
void pidcontrol(int u,int y)
{
   
	ek=u-y;
	duk=kp*(ek-ek1)+ki*ek+kd*(ek+ek2-ek1*2);
	du=(int)duk;
	if(duk>1)  duk=1;
	if(duk<-1) duk=-1;	
	pwm-=du;    
	if(pwm<4)
	{
	  pwm=4;		
	}
	if(pwm>70)
	{
	  pwm=70;	
	}
	EvaRegs.CMPR1=pwm;
	EvaRegs.CMPR2=pwm;
	EvaRegs.CMPR3=pwm;
	
	ek2=ek1;
	ek1=ek;	
}

void delay(unsigned int m)
{
    unsigned int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<1500;j++)
		{}
	}
}


/***********************************************************************/
//	No	more
/***********************************************************************/


