#include "DSP28_Device.h"
#include "motor.h"
#include <stdio.h> 

/***************************************************/
unsigned  int  *LEDReg  = (unsigned int *)0x2800;    //LED ���ƼĴ���
unsigned  int  *KEY_SEL_REG= (unsigned int *)0x2E00;
unsigned int speed_resolution=5;


unsigned int * LED_COMM=(Uint16 *)0x3000;
unsigned int * LED_DATA=(Uint16 *)0x3080;

interrupt void Scirxinta_isr(void);
interrupt void Scitxinta_isr(void);
char speed_real[4];    //���Ҫ�ŵ��ٶ�ֵ
char receive[8];
char realspeed_flag;  //����ʵ�ٶ�Ҫ��ʾ�ı�־
int rece_time=0;
char refresh=1;
///GpioDataRegs.GPBDAT.bit.GPIOB10=NAND RNB
///////////////////////////////////////////////
unsigned int XPOS,YPOS;
/////////////////////////////////////////////////

void ldelay(unsigned int);

/***************************************************/
interrupt void t1pint_isr(void);
interrupt void t2pint_isr(void);
interrupt void  cap_isr(void);
interrupt void  pdpinta_isr(void);
void delay(unsigned int m);
void pidcontrol(int u,int y);//�ٶȵ���

unsigned int t1prd=117;//��ʱ��1������(����PWM�ɵ���Ϊ0-117)
unsigned int t2prd=117;//��ʱ��2�ı�־

unsigned int dir=0;//���ת������

unsigned int dccurent;//ֱ��ĸ�ߵ���
Uint32 Sumdcc=0;
unsigned int dcccount=0,Averagedcc=0;//ĸ�ߵ���ƽ��ֵ��ʼ��
unsigned int dcvoltage;//ֱ��ĸ�ߵ�ѹ
Uint32 Sumdcv=0;
unsigned int dcvcount=0,Averagedcv=0;//ĸ�ߵ�ѹƽ��ֵ��ʼ��

unsigned int capstastus;//����cap�ڵĵ�ƽ
Uint32 Time,T2cnt,count=0;//,Speed=0,;
unsigned Pole=2;//��������ת����NS�Ķ���
Uint32 Sum=0,Average=0;
unsigned int l=0,nnn=0;
unsigned Speed=0;  //��������ĵ����ʵ�ٶ�

//�ٶȵ�pwm�ĳ�ʼֵ
unsigned int pwm=20;//pwm��ռ�ձȳ�ֵ 20����117-20�� �� ��
float kp=0.003,ki=0.02,kd=0.0;
int Speedset=4000;  //�ٶ�����2200ת
int ek=0,ek1=0,ek2=0;
int du;
float duk;

int t2=0;
unsigned int yy=0;
unsigned int test0[2000],test1[5000];
int sss=0;

/***************************************************/
void main(void)
{
	unsigned int KeyValue=0x0f;
	char speed_char[4];    //���Ҫ�ŵ��ٶ�ֵ

	*LEDReg=0xff;
	
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

	//��ʼ��������������
	InitXintf();	
		
	/*��ʼ��GPIO*/
	Gpioinit();

	/*��ʼ��AD*/
	InitAdc();
	
	/*��ʼ��PWM*/
	Init_eva_pwm(); 

	/*��ʼ��LCD*/
	Lcd_Init();
    
    /*��ʼ��CAP*/
    Capinit();

	/*��ʼ��Sci*/
	InitSci();

	//�����ж�������
	EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.CAPINT1=&cap_isr;
    PieVectTable.CAPINT2=&cap_isr;
    PieVectTable.CAPINT3=&cap_isr;
    PieVectTable.T1PINT=&t1pint_isr;
    PieVectTable.T2PINT=&t2pint_isr;
    PieVectTable.PDPINTA=&pdpinta_isr;  
    PieVectTable.RXAINT=&Scirxinta_isr;  
    PieVectTable.TXAINT=&Scitxinta_isr;      
 
	
	PieCtrl.PIEIER2.bit.INTx4=1;//T1pint�ж�
    PieCtrl.PIEIER3.bit.INTx1=1;//T2pint�ж�
    PieCtrl.PIEIER3.bit.INTx5=1;//Cap1�ж�
    PieCtrl.PIEIER3.bit.INTx6=1;//Cap2�ж�
    PieCtrl.PIEIER3.bit.INTx7=1;//Cap3�ж�
    PieCtrl.PIEIER1.bit.INTx1=1;//pdpinta�ж�     	��EV-A��
	PieCtrl.PIEIER9.bit.INTx1 = 1; //SCi�����ж�
	PieCtrl.PIEIER9.bit.INTx2 = 1; //Sci�����ж�
	/* ����IER�Ĵ��� */

	IER |= M_INT1;
    IER |= M_INT2; // t1pint enable
    IER |= M_INT3; // capture enable    
   	IER |= M_INT9;			//��sci���պͷ����ж�

   	EDIS;    // This is needed to disable write to EALL
//	speed_show_length=sprintf (speed_char, "%d", Speedset);    //���ٶȵ�ת��Ϊ�ַ��ͣ�������ʾ
//	lcdwda(6,1,speed_show_length,speed_char); 
    
    // Enable global Interrupts and higher priority real-time debug events:
	startmotor();
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM  


// Step 6. IDLE loop. Just sit and loop forever (optional):	
    while(1)
    {  
   
    	if(refresh==1){
				speed_char[0]=Speedset/1000+48;
				speed_char[1]=Speedset/100%10+48;
				speed_char[2]=Speedset/10%10+48;	
				speed_char[3]=Speedset%10+48;
				lcdw_chars(12,0,4,speed_char);
				refresh=0;   
		}

		if(realspeed_flag==1){
			  speed_real[0]=Speed/1000+48;
	  		  speed_real[1]=Speed/100%10+48;
	          speed_real[2]=Speed/10%10+48;	
	          speed_real[3]=Speed%10+48;
	          lcdw_chars(12,1,4,speed_real);
			  realspeed_flag=0;
			}
    	
    	if((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03){			//�а�������
		    delay(100);
			if((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03){  //ȷʵ�а�������
				switch(KeyValue){   //�ж��ĸ�������
					case 0x02:		
					//	*LEDReg=0xFE;		//һ�ż�����
						Speedset+=speed_resolution;
						if(Speedset>6000) 		// ����ٶ�Ϊ6000
							Speedset=6000; 
						break;
					case 0x01:		
					//	*LEDReg=0xFD;		//���ż�����
						Speedset-=speed_resolution;
						if(Speedset<0)
							Speedset=0;
						break;
				//	case 0x0B:		*LEDReg=0xFB;		//���ż�����
				//	break;
				//	case 0x07:		*LEDReg=0xF7;		//�ĺż�����
				//	break;
					default:	;
				}
			refresh=1;
			}
		   while((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03);    //�ȴ���������
		}   
		     	
	}	
} 	


interrupt void  t2pint_isr(void)          //t2�������㻻��ʱ��
{
  count++; 
  EvaRegs.EVAIFRB.bit.T2PINT=1;//����жϱ�־
  EvaRegs.EVAIMRB.bit.T2PINT=1;//�ж�����    
  PieCtrl.PIEACK.bit.ACK3=1;//��cpu�����ж�  IN3�ж�����
}


interrupt void  cap_isr(void)
{  
   /*****����������⴫�����������ƽ����������****/
   Uint32 kk=t2prd;
   EALLOW;
   GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=0;//�趨cap1~3Ϊgpio
   GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=0;
   GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=0;   
   EDIS;    // This is needed to disable write to EALL
   GpioMuxRegs.GPADIR.bit.GPIOA8=0;//�趨cap1~3Ϊ����
   GpioMuxRegs.GPADIR.bit.GPIOA9=0;
   GpioMuxRegs.GPADIR.bit.GPIOA10=0;
   capstastus=(GpioDataRegs.GPADAT.all&0x0700)>>8;
 // ˳ʱ��ת3-1-5-4-6-2   ��ʱ��ת  2-6-4-5-1-3 
   if(dir==1)
    {   //��ʱ�� 2-6-4-5-1-3
     	switch(capstastus)//ir2136 ��hin��lin�Ƿ����
   	 	{ //PWM����͵�ƽ��ͨ����ΪIR2136λ�͵�ƽ��Ч
   			case 1: EvaRegs.ACTR.all=0x7fd;break;//  1 6������PWM��ǿ�����ߣ�����оƬ2136���߼�������͵�ƽʱ�������
   			case 2: EvaRegs.ACTR.all=0xfd7;break;//  2 3	����Ч 
   			case 3: EvaRegs.ACTR.all=0x7df;break;//  3 6
   			case 4: EvaRegs.ACTR.all=0xd7f;break;//  5 4
   			case 5: EvaRegs.ACTR.all=0xf7d;break;//  4 1
   			case 6: EvaRegs.ACTR.all=0xdf7;break;//  2 5
   	 	}
    }
    else
    {	//˳ʱ��3-1-5-4-6-2
    	switch(capstastus)//ir2136 ��hin��lin�Ƿ����
   	 	{
   			case 5: EvaRegs.ACTR.all=0xfd7;break;//	3 2
   			case 1: EvaRegs.ACTR.all=0xd7f;break;//	5 4	
   			case 3: EvaRegs.ACTR.all=0xdf7;break;//	5 2
   			case 2: EvaRegs.ACTR.all=0xf7d;break;//	4 1
   			case 6: EvaRegs.ACTR.all=0x7fd;break;//	6 1
   			case 4: EvaRegs.ACTR.all=0x7df;break;//	6 3
   	 	}
    }
      
   
   /*������������ת��*/
    T2cnt=EvaRegs.T2CNT;//��ȡ��ʱ��2��ֵ
    Time=kk*count+ T2cnt;//�����ת1������ʱ��   count��t2�ж��м�¼ 
    Sum+=Time;      
    l++;
        
    if(l==12)//ÿת12/6/pole����һ��ת�� cap�ж�12�ε�ƽ���ٶȡ����ڼ�����2�ĵ����Ϊһת��ƽ���ٶ�
    {
      Average=Sum/12;  
              
      Speed=kk*20000*60/(Average*6*Pole);//����ת��   pole=2     
      Sum=0;      
      l=0;               
      test0[nnn]=Speed;//�����ã��洢�ٶ�ֵ 
   /*����������ʾʵ���ٶ�*/         //�����ж��У��ж�Ƕ�׻������⣿��
   /* 
	  speed_real[0]=Speed/1000+48;
	  speed_real[1]=Speed/100%10+48;
	  speed_real[2]=Speed/10%10+48;	
	  speed_real[3]=Speed%10+48;
	  lcdw_chars(12,1,4,speed_real);
   */ 
   	  realspeed_flag=1;      
      nnn++;      
      if(nnn==2000)
      {      	 
      	  nnn=0;  
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
    EvaRegs.CAPFIFO.all = 0x01500; // ��ղ�׽��ջ   CAPΪ�������棬������д��һ��ֵ������һ��ֵʱ���ж�
    PieCtrl.PIEACK.bit.ACK3=1;//cap1�ж���cpu�����ж�
}

interrupt void  t1pint_isr(void)   //ÿ��PWM�����ж�һ��
{
    dcvoltage=(AdcRegs.RESULT0)>>4;    //12λAD���Խ��Ҫ����4λ
  	dccurent=(AdcRegs.RESULT1)>>4; 
  	
  	yy++;
  	if(yy==10000) 
  	{
  		yy=0;
  		GpioDataRegs.GPADAT.bit.GPIOA13 =!GpioDataRegs.GPADAT.bit.GPIOA13;//����������ʾ  
    }
   //ĸ�ߵ�ѹ��⣬��ѹ������
   //ע���������������ת������ı�ʱ������ĸ�ߵ�ѹ������
   //���ö����ƽ��ֵ
    Sumdcv+=dcvoltage;    
    dcvcount++;
    if(dcvcount==500)
    {
    	Averagedcv=Sumdcv/500;//���ѹƽ��ֵ
    	Sumdcv=0;
    	dcvcount=0;	
    }    
  	
  	//ĸ�ߵ�����⣬�������������ö����ƽ��ֵ
  	Sumdcc+=dccurent;    
    dcccount++;
    if(dcccount==200)
    {
    	Averagedcc=Sumdcc/200;//��ƽ��ĸ�ߵ���
    	Sumdcc=0;
    	dcccount=0;	    	
    }    
    
  			
  	if((Averagedcv>=3000)|(Averagedcc>=3000))  //question
  	{
  		stopmotor();
  		if(Averagedcv>=3000) 
  		GpioDataRegs.GPADAT.bit.GPIOA11 =1;//��ѹ��ʾ
  		if(Averagedcc>=3000)
  		GpioDataRegs.GPADAT.bit.GPIOA12 =1;//������ʾ	
  	}
  	
    else
    {   
        GpioDataRegs.GPADAT.bit.GPIOA11 =0;//��ѹ����ʾ
        GpioDataRegs.GPADAT.bit.GPIOA12 =0;//��������ʾ 	
  		////�ٶȵ���
  		test1[t2]=Averagedcc;
  		t2++;
  		if(t2==5000)   //question
  		{   
  			pidcontrol(Speedset,Speed);//250ms 
  			t2=0;  		
  		}
  	
  	}
  	/******************************/  
  	AdcRegs.ADC_ST_FLAG.bit.INT_SEQ1_CLR=1; //���״̬��
 	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;//��λseq1
  
  //	EvaRegs.EVAIFRA.bit.T1PINT=1;//����жϱ�־
  	EvaRegs.EVAIMRA.bit.T1PINT=1;//�ж����� 
   
  	PieCtrl.PIEACK.bit.ACK2=1;//д1����

}
 

interrupt void  pdpinta_isr(void)
{
  
  	EvaRegs.EVAIFRA.bit.PDPINTA=1;//���PDPINTA�жϱ�־
  	PieCtrl.PIEACK.bit.ACK1=1;//��cpu�����ж�
  	
    stopmotor();    
  	EvaRegs.COMCONA.bit.FCOMPOE=1;//����ʹ�ܱȽ��������������������
 
}



//pid����  pwm��Χ0-117����ΪIR2136Ϊ�͵�ƽ������pwmֵԽС��ռ�ձ�Խ�������ѹԽ��
void pidcontrol(int u,int y)   
{
   
	ek=u-y;
	duk=kp*(ek-ek1)+ki*ek+kd*(ek+ek2-ek1*2);    //����PID�������ڻ��ֱ�������
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
	EvaRegs.CMPR1=pwm;    //��ʱ���Ƚϵ�Ԫ
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


void ldelay(unsigned int k)
{
	while(k>0)
	{
		k--;
		delay(50000);
	}
}	


/***********************************************************************/
//	SCI�����ж�
/***********************************************************************/

interrupt void Scirxinta_isr(void)
{
	EINT;
	receive[rece_time]=SciaRegs.SCIRXBUF.bit.RXDT;
	rece_time++;
	if(receive[rece_time-2]=='\r' && receive[rece_time-1]=='\n'){
		rece_time=rece_time-3;
		if(rece_time<4){               //���������ٶȴ���4λ��ʱ
			int i=0,speed_uart=0;
			for(i=0;i<=rece_time;i++)
				speed_uart=speed_uart*10+receive[i]-48;
			Speedset=speed_uart;
			refresh=1;
			speed_uart=0;
		}
		rece_time=0;
	}
	PieCtrl.PIEACK.bit.ACK9=1;     //ʹ�����
}

interrupt void Scitxinta_isr()
{

}
