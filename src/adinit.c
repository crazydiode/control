//###########################################################################
//
// FILE:	DSP281x_Adc.c
//
// TITLE:	DSP281x ADC Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | ADC_usDELAY changed from 5000L to 8000L
//###########################################################################

#include "DSP28_Device.h"     // DSP281x Headerfile Include File
#include "DSP28_Adc.h"   // DSP281x Examples Include File



//---------------------------------------------------------------------------
// InitAdc: 
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
void InitAdc(void)
{

   	unsigned int i;	
   	
   	
	AdcRegs.ADCTRL1.bit.RESET=1;
	asm(" NOP");
	AdcRegs.ADCTRL1.bit.RESET=0;
	AdcRegs.ADCTRL1.bit.SUSMOD=3;  //仿真悬挂模式3，仿真悬挂时立即停止
	AdcRegs.ADCTRL1.bit.ACQ_PS=0;  //采样窗长度
	AdcRegs.ADCTRL1.bit.CPS=0;    //ADC时钟为HSPCLK时钟（1时，2分频）
	AdcRegs.ADCTRL1.bit.CONT_RUN=0;  //启动/停止模式，排序器需要复位到CONV00
	AdcRegs.ADCTRL1.bit.SEQ_CASC=1;  //单排序器模式（16位）
	
	AdcRegs.ADCTRL3.bit.ADCBGRFDN=3;   //带隙和参考电路电源开
	for(i=0;i<10000;i++)	asm(" NOP");
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;   //ADC电源开
	for(i=0;i<5000;i++)	asm(" NOP");
	AdcRegs.ADCTRL3.bit.ADCCLKPS=15;   //ADC时钟=HCLK/（2*16）=1.17M
	AdcRegs.ADCTRL3.bit.SMODE_SEL=0;  //0 顺序采样模式； 1 同时采样模式
	
	AdcRegs.MAX_CONV.all=0x0005;  //一次AD 6次转换
    AdcRegs.CHSELSEQ1.bit.CONV00=0x0;   //通道0
    AdcRegs.CHSELSEQ1.bit.CONV01=0x1;
    AdcRegs.CHSELSEQ1.bit.CONV02=0x2;
    AdcRegs.CHSELSEQ1.bit.CONV03=0x3;    
    AdcRegs.CHSELSEQ2.bit.CONV04=0x4;
    AdcRegs.CHSELSEQ2.bit.CONV05=0x5;
   
   
   
   /*
   AdcRegs.ADCMAXCONV.all = 0x0005;       // Setup 2 conv's on SEQ1
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA3 as 1st SEQ1 conv.
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x3;
   AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x4;
   AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x1;
   AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x2; // Setup ADCINA2 as 2nd SEQ1 conv.
   AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5; 
   */
   AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1 = 1;  // Enable EVASOC to start SEQ1
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
	
  }	

//===========================================================================
// No more.
//===========================================================================

