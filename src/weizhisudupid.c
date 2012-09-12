#include "DSP28_Device.h"
#include "motor.h"
#include <stdio.h> 

/***************************************************/
unsigned  int  *LEDReg  = (unsigned int *)0x2800;    //LED 控制寄存器
unsigned  int  *KEY_SEL_REG= (unsigned int *)0x2E00;
unsigned int speed_resolution=5;


unsigned int * LED_COMM=(Uint16 *)0x3000;
unsigned int * LED_DATA=(Uint16 *)0x3080;

interrupt void Scirxinta_isr(void);
interrupt void Scitxinta_isr(void);
char speed_real[4];    //存放要放的速度值
char receive[8];
char realspeed_flag;  //有真实速度要显示的标志
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
void pidcontrol(int u,int y);//速度调节

unsigned int t1prd=117;//定时器1的周期(所以PWM可调节为0-117)
unsigned int t2prd=117;//定时器2的标志

unsigned int dir=0;//电机转动方向

unsigned int dccurent;//直流母线电流
Uint32 Sumdcc=0;
unsigned int dcccount=0,Averagedcc=0;//母线电流平均值初始化
unsigned int dcvoltage;//直流母线电压
Uint32 Sumdcv=0;
unsigned int dcvcount=0,Averagedcv=0;//母线电压平均值初始化

unsigned int capstastus;//定义cap口的电平
Uint32 Time,T2cnt,count=0;//,Speed=0,;
unsigned Pole=2;//极对数：转子上NS的对数
Uint32 Sum=0,Average=0;
unsigned int l=0,nnn=0;
unsigned Speed=0;  //霍尔捕获的电机真实速度

//速度到pwm的初始值
unsigned int pwm=20;//pwm的占空比初值 20：（117-20） 高 低
float kp=0.003,ki=0.02,kd=0.0;
int Speedset=4000;  //速度设置2200转
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
	char speed_char[4];    //存放要放的速度值

	*LEDReg=0xff;
	
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

	//初始化外设外扩总线
	InitXintf();	
		
	/*初始化GPIO*/
	Gpioinit();

	/*初始化AD*/
	InitAdc();
	
	/*初始化PWM*/
	Init_eva_pwm(); 

	/*初始化LCD*/
	Lcd_Init();
    
    /*初始化CAP*/
    Capinit();

	/*初始化Sci*/
	InitSci();

	//设置中断向量表
	EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.CAPINT1=&cap_isr;
    PieVectTable.CAPINT2=&cap_isr;
    PieVectTable.CAPINT3=&cap_isr;
    PieVectTable.T1PINT=&t1pint_isr;
    PieVectTable.T2PINT=&t2pint_isr;
    PieVectTable.PDPINTA=&pdpinta_isr;  
    PieVectTable.RXAINT=&Scirxinta_isr;  
    PieVectTable.TXAINT=&Scitxinta_isr;      
 
	
	PieCtrl.PIEIER2.bit.INTx4=1;//T1pint中断
    PieCtrl.PIEIER3.bit.INTx1=1;//T2pint中断
    PieCtrl.PIEIER3.bit.INTx5=1;//Cap1中断
    PieCtrl.PIEIER3.bit.INTx6=1;//Cap2中断
    PieCtrl.PIEIER3.bit.INTx7=1;//Cap3中断
    PieCtrl.PIEIER1.bit.INTx1=1;//pdpinta中断     	（EV-A）
	PieCtrl.PIEIER9.bit.INTx1 = 1; //SCi接收中断
	PieCtrl.PIEIER9.bit.INTx2 = 1; //Sci发射中断
	/* 设置IER寄存器 */

	IER |= M_INT1;
    IER |= M_INT2; // t1pint enable
    IER |= M_INT3; // capture enable    
   	IER |= M_INT9;			//开sci接收和发射中断

   	EDIS;    // This is needed to disable write to EALL
//	speed_show_length=sprintf (speed_char, "%d", Speedset);    //将速度的转化为字符型，方便显示
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
    	
    	if((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03){			//有按键按下
		    delay(100);
			if((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03){  //确实有按键按下
				switch(KeyValue){   //判断哪个键按下
					case 0x02:		
					//	*LEDReg=0xFE;		//一号键按下
						Speedset+=speed_resolution;
						if(Speedset>6000) 		// 最大速度为6000
							Speedset=6000; 
						break;
					case 0x01:		
					//	*LEDReg=0xFD;		//二号键按下
						Speedset-=speed_resolution;
						if(Speedset<0)
							Speedset=0;
						break;
				//	case 0x0B:		*LEDReg=0xFB;		//三号键按下
				//	break;
				//	case 0x07:		*LEDReg=0xF7;		//四号键按下
				//	break;
					default:	;
				}
			refresh=1;
			}
		   while((KeyValue=(GpioDataRegs.GPBDAT.all)&0X03)!=0x03);    //等待按键按下
		}   
		     	
	}	
} 	


interrupt void  t2pint_isr(void)          //t2用作计算换相时间
{
  count++; 
  EvaRegs.EVAIFRB.bit.T2PINT=1;//清除中断标志
  EvaRegs.EVAIMRB.bit.T2PINT=1;//中断允许    
  PieCtrl.PIEACK.bit.ACK3=1;//向cpu申请中断  IN3中断允许
}


interrupt void  cap_isr(void)
{  
   /*****以下用来检测传感器的输出电平，用来换向****/
   Uint32 kk=t2prd;
   EALLOW;
   GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=0;//设定cap1~3为gpio
   GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=0;
   GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=0;   
   EDIS;    // This is needed to disable write to EALL
   GpioMuxRegs.GPADIR.bit.GPIOA8=0;//设定cap1~3为输入
   GpioMuxRegs.GPADIR.bit.GPIOA9=0;
   GpioMuxRegs.GPADIR.bit.GPIOA10=0;
   capstastus=(GpioDataRegs.GPADAT.all&0x0700)>>8;
 // 顺时针转3-1-5-4-6-2   逆时针转  2-6-4-5-1-3 
   if(dir==1)
    {   //逆时针 2-6-4-5-1-3
     	switch(capstastus)//ir2136 的hin和lin是反向的
   	 	{ //PWM输出低电平导通，因为IR2136位低电平有效
   			case 1: EvaRegs.ACTR.all=0x7fd;break;//  1 6，其它PWM都强制拉高；驱动芯片2136负逻辑，输入低电平时驱动输出
   			case 2: EvaRegs.ACTR.all=0xfd7;break;//  2 3	低有效 
   			case 3: EvaRegs.ACTR.all=0x7df;break;//  3 6
   			case 4: EvaRegs.ACTR.all=0xd7f;break;//  5 4
   			case 5: EvaRegs.ACTR.all=0xf7d;break;//  4 1
   			case 6: EvaRegs.ACTR.all=0xdf7;break;//  2 5
   	 	}
    }
    else
    {	//顺时针3-1-5-4-6-2
    	switch(capstastus)//ir2136 的hin和lin是反向的
   	 	{
   			case 5: EvaRegs.ACTR.all=0xfd7;break;//	3 2
   			case 1: EvaRegs.ACTR.all=0xd7f;break;//	5 4	
   			case 3: EvaRegs.ACTR.all=0xdf7;break;//	5 2
   			case 2: EvaRegs.ACTR.all=0xf7d;break;//	4 1
   			case 6: EvaRegs.ACTR.all=0x7fd;break;//	6 1
   			case 4: EvaRegs.ACTR.all=0x7df;break;//	6 3
   	 	}
    }
      
   
   /*以下用来计算转速*/
    T2cnt=EvaRegs.T2CNT;//读取定时器2的值
    Time=kk*count+ T2cnt;//获得运转1相所需时间   count在t2中断中记录 
    Sum+=Time;      
    l++;
        
    if(l==12)//每转12/6/pole计算一下转速 cap中断12次的平均速度。对于极对数2的电机，为一转的平均速度
    {
      Average=Sum/12;  
              
      Speed=kk*20000*60/(Average*6*Pole);//计算转速   pole=2     
      Sum=0;      
      l=0;               
      test0[nnn]=Speed;//测试用，存储速度值 
   /*以下用来显示实际速度*/         //放在中断中，中断嵌套会有问题？？
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
    EvaRegs.T2CON.all = 0x1400;//关闭定时器2   
    EvaRegs.T2CNT = 0x0000;
    EvaRegs.T2CON.all = 0x1440;//启动定时器2
    
   
    /*************************/
    GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8=1;//重新设定cap1~3为gpio
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9=1;
    GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10=1; 
 
    EvaRegs.EVAIFRC.all = 7 ; // 清捕捉中断
    EvaRegs.CAPFIFO.all = 0x01500; // 清空捕捉堆栈   CAP为两级缓存，所以先写入一个值，有下一个值时则中断
    PieCtrl.PIEACK.bit.ACK3=1;//cap1中断向cpu申请中断
}

interrupt void  t1pint_isr(void)   //每个PWM周期中断一次
{
    dcvoltage=(AdcRegs.RESULT0)>>4;    //12位AD所以结果要右移4位
  	dccurent=(AdcRegs.RESULT1)>>4; 
  	
  	yy++;
  	if(yy==10000) 
  	{
  		yy=0;
  		GpioDataRegs.GPADAT.bit.GPIOA13 =!GpioDataRegs.GPADAT.bit.GPIOA13;//程序运行显示  
    }
   //母线电压检测，过压保护，
   //注：电机在启动或者转动方向改变时，可能母线电压有脉动
   //采用多次求平均值
    Sumdcv+=dcvoltage;    
    dcvcount++;
    if(dcvcount==500)
    {
    	Averagedcv=Sumdcv/500;//求电压平均值
    	Sumdcv=0;
    	dcvcount=0;	
    }    
  	
  	//母线电流检测，过流保护，采用多次求平均值
  	Sumdcc+=dccurent;    
    dcccount++;
    if(dcccount==200)
    {
    	Averagedcc=Sumdcc/200;//求平均母线电流
    	Sumdcc=0;
    	dcccount=0;	    	
    }    
    
  			
  	if((Averagedcv>=3000)|(Averagedcc>=3000))  //question
  	{
  		stopmotor();
  		if(Averagedcv>=3000) 
  		GpioDataRegs.GPADAT.bit.GPIOA11 =1;//过压显示
  		if(Averagedcc>=3000)
  		GpioDataRegs.GPADAT.bit.GPIOA12 =1;//过流显示	
  	}
  	
    else
    {   
        GpioDataRegs.GPADAT.bit.GPIOA11 =0;//过压不显示
        GpioDataRegs.GPADAT.bit.GPIOA12 =0;//过不流显示 	
  		////速度调节
  		test1[t2]=Averagedcc;
  		t2++;
  		if(t2==5000)   //question
  		{   
  			pidcontrol(Speedset,Speed);//250ms 
  			t2=0;  		
  		}
  	
  	}
  	/******************************/  
  	AdcRegs.ADC_ST_FLAG.bit.INT_SEQ1_CLR=1; //清除状态字
 	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;//复位seq1
  
  //	EvaRegs.EVAIFRA.bit.T1PINT=1;//清除中断标志
  	EvaRegs.EVAIMRA.bit.T1PINT=1;//中断允许 
   
  	PieCtrl.PIEACK.bit.ACK2=1;//写1清零

}
 

interrupt void  pdpinta_isr(void)
{
  
  	EvaRegs.EVAIFRA.bit.PDPINTA=1;//清除PDPINTA中断标志
  	PieCtrl.PIEACK.bit.ACK1=1;//向cpu申请中断
  	
    stopmotor();    
  	EvaRegs.COMCONA.bit.FCOMPOE=1;//重新使能比较输出？？？？？？？？
 
}



//pid控制  pwm范围0-117，因为IR2136为低电平驱动，pwm值越小，占空比越大，输出电压越大
void pidcontrol(int u,int y)   
{
   
	ek=u-y;
	duk=kp*(ek-ek1)+ki*ek+kd*(ek+ek2-ek1*2);    //增量PID，不存在积分饱和现象
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
	EvaRegs.CMPR1=pwm;    //定时器比较单元
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
//	SCI接收中断
/***********************************************************************/

interrupt void Scirxinta_isr(void)
{
	EINT;
	receive[rece_time]=SciaRegs.SCIRXBUF.bit.RXDT;
	rece_time++;
	if(receive[rece_time-2]=='\r' && receive[rece_time-1]=='\n'){
		rece_time=rece_time-3;
		if(rece_time<4){               //当发出的速度大于4位数时
			int i=0,speed_uart=0;
			for(i=0;i<=rece_time;i++)
				speed_uart=speed_uart*10+receive[i]-48;
			Speedset=speed_uart;
			refresh=1;
			speed_uart=0;
		}
		rece_time=0;
	}
	PieCtrl.PIEACK.bit.ACK9=1;     //使能输出
}

interrupt void Scitxinta_isr()
{

}
