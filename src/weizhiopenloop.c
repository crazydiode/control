#include "DSP28_Device.h"
#include "motor.h"


/***************************************************/
interrupt void t1pint_isr(void);
interrupt void t2pint_isr(void);
interrupt void  cap_isr(void);
interrupt void  pdpinta_isr(void);
void delay(unsigned int m);

unsigned int t1prd=117;//定时器1的周期
unsigned int t2prd=117;//定时器2的标志

unsigned int dir=0;//电机转动方向
unsigned int pwm=20;

unsigned int dccurent,u,v,w,speedad;//直流母线电流
unsigned int dcvoltage;//直流母线电压
Uint32 Sumdcv=0;
unsigned int dcvcount=0,Averagedcv=0;//母线电压平均值初始化


unsigned int capstastus;//定义cap口的电平
Uint32 Time,T2cnt,Speed=0,count=0;
unsigned Pole=2;//极对数
Uint32 Sum=0,Average=0;
unsigned int l=0,nn=0;

int t2=0;
unsigned int test[2000],test1[2000],test2[2000];
int sss=0;
/***************************************************/

void main(void)
{
	/*初始化系统*/
	InitSysCtrl();

	/*关中断*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	/*初始化PIE*/
	InitPieCtrl();

	/*初始化PIE矢量表*/
	InitPieVectTable();	
	
	/*初始化GPIO*/
	Gpioinit();
	
	/*初始化AD*/
	InitAdc();
	
	/*初始化PWM*/
	Init_eva_pwm(); 
    
    /*初始化CAP*/
    Capinit();	
	
	EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.CAPINT1=&cap_isr;
    PieVectTable.CAPINT2=&cap_isr;
    PieVectTable.CAPINT3=&cap_isr;
    PieVectTable.T1PINT=&t1pint_isr;
    PieVectTable.T2PINT=&t2pint_isr;
    PieVectTable.PDPINTA=&pdpinta_isr;    
    EDIS;    // This is needed to disable write to EALL
	
	PieCtrl.PIEIER2.bit.INTx4=1;//T1pint中断
    PieCtrl.PIEIER3.bit.INTx1=1;//T2pint中断
    PieCtrl.PIEIER3.bit.INTx5=1;//Cap1中断
    PieCtrl.PIEIER3.bit.INTx6=1;//Cap2中断
    PieCtrl.PIEIER3.bit.INTx7=1;//Cap3中断
    PieCtrl.PIEIER1.bit.INTx1=1;//pdpinta中断    	
	/* 设置IER寄存器 */

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
  EvaRegs.EVAIFRB.bit.T2PINT=1;//清除中断标志
  EvaRegs.EVAIMRB.bit.T2PINT=1;//中断允许    
  PieCtrl.PIEACK.bit.ACK3=1;//向cpu申请中断 
}


interrupt void  cap_isr(void)
{  
   /*****以下用来检测传感器的输出电平，用来换向****/
   Uint32 kk=t2prd;
   EALLOW;
   GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=0;//设定cap1~3为gpio
   GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=0;
   GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=0;   
   GpioMuxRegs.GPADIR.bit.GPIOA8=0;//设定cap1~3为输入
   GpioMuxRegs.GPADIR.bit.GPIOA9=0;
   GpioMuxRegs.GPADIR.bit.GPIOA10=0;
   capstastus=(GpioDataRegs.GPADAT.all&0x0700)>>8;
   
   if(dir==1)
    {
     	switch(capstastus)//ir2136 的hin和lin是反向的
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
    	switch(capstastus)//ir2136 的hin和lin是反向的
   	 	{
   			case 5: EvaRegs.ACTR.all=0xfd7;break;//h1 rise    
   			case 1: EvaRegs.ACTR.all=0xd7f;break;//h3 fall
   			case 3: EvaRegs.ACTR.all=0xdf7;break;//h2 rise
   			case 2: EvaRegs.ACTR.all=0xf7d;break;//h1 fall
   			case 6: EvaRegs.ACTR.all=0x7fd;break;//h3 rise
   			case 4: EvaRegs.ACTR.all=0x7df;break;//h2 fall
   	 	}
    }
      
   
   /*以下用来计算转速*/
    T2cnt=EvaRegs.T2CNT;//读取定时器2的值
    Time=kk*count+ T2cnt;//获得运转1相所需时间    
    Sum+=Time;   
    l++;
        
    if(l==12)//每转12/6/pole计算一下转速
    {
      Average=Sum/12;  
              
      Speed=kk*20000*60/(Average*6*Pole);//计算转速        
      Sum=0;      
      l=0;      
      test[nn]=Speed;//测试用，存储速度值      
      nn++;      
      if(nn==2000)
      {      	 
      	  nn=0;  
      }

    }
    
    count=0;
    EvaRegs.T2CON.all = 0x1400;//关闭定时器2   
    EvaRegs.T2CNT = 0x0000;
    EvaRegs.T2CON.all = 0x1440;//启动定时器2
    
   
    /*************************/
    GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=1;//重新设定cap1~3为gpio
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=1;
    GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=1; 
   
    EvaRegs.EVAIFRC.all = 7 ; // 清捕捉中断
    EvaRegs.CAPFIFO.all = 0x01500; // 清空捕捉堆栈   
    PieCtrl.PIEACK.bit.ACK3=1;//cap1中断向cpu申请中断
}

interrupt void  t1pint_isr(void)
{
    dcvoltage=(AdcRegs.RESULT0)>>4;
  	dccurent=(AdcRegs.RESULT3)>>4;
    w=(AdcRegs.RESULT1)>>4;
    u=(AdcRegs.RESULT2)>>4;  	
    speedad=(AdcRegs.RESULT4)>>4;
    v=(AdcRegs.RESULT5)>>4;     
    
    
    
   //母线电压检测，过压保护，
   //注：电机在启动或者转动方向改变时，可能母线电压有脉动
   //采用多次求平均值
    Sumdcv+=dcvoltage;
    dcvcount++;
    if(dcvcount==500)
    {
    	Averagedcv=Sumdcv/500;//求平均母线电压
    	Sumdcv=0;
    	dcvcount=0;	
    }    
  		
  	if(Averagedcv>=3000)
  	{
  		stopmotor();
  		GpioDataRegs.GPADAT.bit.GPIOA11 =1;//过压显示	
  	}    
  	
  	/******************************/  
  	AdcRegs.ADC_ST_FLAG.bit.INT_SEQ1_CLR=1; //清除状态字
 	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;//复位seq1
  
  	EvaRegs.EVAIFRA.bit.T1PINT=1;//清除中断标志
  	EvaRegs.EVAIMRA.bit.T1PINT=1;//中断允许 
   
  	PieCtrl.PIEACK.bit.ACK2=1;//向cpu申请中断

}


interrupt void  pdpinta_isr(void)
{
  
  	EvaRegs.EVAIFRA.bit.PDPINTA=1;//清除PDPINTA中断标志
  	PieCtrl.PIEACK.bit.ACK1=1;//向cpu申请中断
  	
    stopmotor();
    GpioDataRegs.GPADAT.bit.GPIOA12 =1;//故障显示
  	EvaRegs.COMCONA.bit.FCOMPOE=1;//重新使能比较输出
 
}

/***********************************************************************/
//	No	more
/***********************************************************************/



