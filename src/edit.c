#include "DSP28_Device.h"
#include "comm.h"
#include "mcbsp.h"
#include "gui_string.h"

unsigned int i,j;
unsigned int send_flag;

/*******************************************************/
#define DATATYPE 1 /* 0代表退出；1代表选项菜单*/
#define menu_num 5 /*通过num来控制输出文字的个数，通过gui_string.sam控制内容。*/
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

#if DATATYPE==1
	Test =USE_EDIT; //数据 0xFFA0  
#endif 
#if DATATYPE==0
	Test =USE_DATAEND; //用户数据结束 0x6
#endif 
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
	
	/*初始化MCBSP外设*/
	InitMcbsp();
	
   	EALLOW;	// This is needed to write to EALLOW protected registers
   	PieVectTable.MRINTA= &MRINTA_ISR;
//  	PieVectTable.MXINTA= &MXINTA_ISR;
//	PieVectTable.MRINTA_ISR = &MRINTA_ISR;
	EDIS;   // This is needed to disable write to EALLOW protected registers
	
	/* 设置IER寄存器 */
	IER |= M_INT6;
    // Enable global Interrupts and higher priority real-time debug events:
	
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
	
    psend=(PmcbspForDec)(&mcbspx[0]);  

// Step 6. IDLE loop. Just sit and loop forever (optional):	
    while(1){
        switch(Test) {
           case USE_EDIT://发送菜单
                psend->Length=FRAMLENGTH; 		
 	   			psend->Type=USE_DATASEND;  //0x5  用户数据传送
	   		 	psend->Mutul=FRAME_SING;			
	 		    psend->Data[0]=USE_START;  //0xFF00 用户命令开始
	  		    psend->Data[1]=USE_EDIT;    //0xFFA0  编辑菜单
	    		psend->Data[2]=menu_num;    
			    for(k=0;k<=menu_num;k++)
			        psend->Data[4+k]=strEdit[k];
		    	mcbsp_tx((unsigned int *)psend);
                Test=0; 
            break;
            case USE_DATAEND://结束用户菜单
                psend->Length=FRAMLENGTH; 		
 	   			psend->Type=USE_DATAEND;  //0x6  用户数据传送
	   		 	psend->Mutul=FRAME_SING;  //单帧			
	 		    psend->Data[0]=USE_END;  //0xFF01 用户命令开始
	  		    mcbsp_tx((unsigned int *)psend);
	  		    Test=0;
            break;
            default:
		    break;  
		}
		mcbsp_rx(&mcbspr[0]);
   	    mcbsp_s=mcbsp_status();
    	if((mcbsp_s == 0xffff)||(mcbsp_s == 3)){        
		     /*通知主机程序通讯出错,准备重发*/
		   	psend->Length = FRAMLENGTH;
		   	psend->Type = FRAME_CMD;
			psend->Data[0] = CHECK_ERR;
		   	psend->Mutul = FRAME_SING;
			mcbsp_tx((unsigned int *) psend);
		}
		if(mcbsp_s == 0){
			 preceive=(PmcbspForDec)(&mcbspr[0]);
			if(preceive->Data[0]==USE_DATAEND){//Enter键
				Edit_result=preceive->Data[1];
			}		   		   		     	     	
		}
	}	
} 	


/***********************************************************************/
//	No	more
/***********************************************************************/

