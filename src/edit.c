#include "DSP28_Device.h"
#include "comm.h"
#include "mcbsp.h"
#include "gui_string.h"

unsigned int i,j;
unsigned int send_flag;

/*******************************************************/
#define DATATYPE 1 /* 0�����˳���1����ѡ��˵�*/
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

#if DATATYPE==1
	Test =USE_EDIT; //���� 0xFFA0  
#endif 
#if DATATYPE==0
	Test =USE_DATAEND; //�û����ݽ��� 0x6
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
	
	/*��ʼ��MCBSP����*/
	InitMcbsp();
	
   	EALLOW;	// This is needed to write to EALLOW protected registers
   	PieVectTable.MRINTA= &MRINTA_ISR;
//  	PieVectTable.MXINTA= &MXINTA_ISR;
//	PieVectTable.MRINTA_ISR = &MRINTA_ISR;
	EDIS;   // This is needed to disable write to EALLOW protected registers
	
	/* ����IER�Ĵ��� */
	IER |= M_INT6;
    // Enable global Interrupts and higher priority real-time debug events:
	
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
	
    psend=(PmcbspForDec)(&mcbspx[0]);  

// Step 6. IDLE loop. Just sit and loop forever (optional):	
    while(1){
        switch(Test) {
           case USE_EDIT://���Ͳ˵�
                psend->Length=FRAMLENGTH; 		
 	   			psend->Type=USE_DATASEND;  //0x5  �û����ݴ���
	   		 	psend->Mutul=FRAME_SING;			
	 		    psend->Data[0]=USE_START;  //0xFF00 �û����ʼ
	  		    psend->Data[1]=USE_EDIT;    //0xFFA0  �༭�˵�
	    		psend->Data[2]=menu_num;    
			    for(k=0;k<=menu_num;k++)
			        psend->Data[4+k]=strEdit[k];
		    	mcbsp_tx((unsigned int *)psend);
                Test=0; 
            break;
            case USE_DATAEND://�����û��˵�
                psend->Length=FRAMLENGTH; 		
 	   			psend->Type=USE_DATAEND;  //0x6  �û����ݴ���
	   		 	psend->Mutul=FRAME_SING;  //��֡			
	 		    psend->Data[0]=USE_END;  //0xFF01 �û����ʼ
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
		if(mcbsp_s == 0){
			 preceive=(PmcbspForDec)(&mcbspr[0]);
			if(preceive->Data[0]==USE_DATAEND){//Enter��
				Edit_result=preceive->Data[1];
			}		   		   		     	     	
		}
	}	
} 	


/***********************************************************************/
//	No	more
/***********************************************************************/

