/*****************************************************************************/
/* COMM.H	v1.00														     */
/* ��Ȩ(c)	2003-   	�������ڴ���Ӽ����������ι�˾						 */
/* �����:	������															 */
/*****************************************************************************/
#ifndef _prtocol_H_
#define _protocol_H_

/********************************************************************************/
/* ��������																		*/
/********************************************************************************/
#define Word                  0xaa55  //��������
#define Number                0x55aa  //��������

/*֡���͵��趨*/
#define  FRAME_DATA           0x4        //����֡
#define  FRAME_CMD            0x2        //����֡
#define  FRAME_DATASEND       0x1        //���ݴ���֡
#define  FRAME_DATAEND        0x3        //���ݴ��ͽ���֡
#define  USE_DATASEND		  0x5		 //�û����ݴ���
#define  USE_DATAEND		  0x6		 //�û����ݽ���

//֡��־
//��֡ʹ��
#define  FRAME_CONT   		  0x1        //�м�֡
#define  FRAME_END		      0x2        //����֡
//��֡ʹ��
#define FRAME_SING		      0x3        //��֡


/*�����ź�*/
#define  HAND 		          0xAAA0	 //�����ź�

/*�Լ칦��*/ 
#define  SRAMCH_OK 	      	  0xAA11	 //SRAM�Լ�ɹ�����
#define  SRAMCH_ERROR 	      0xAA12	 //SRAM�Լ첻�ɹ�����
#define	 SDRAMCH_OK	          0xAA15	 //SDRAM�Լ�ɹ�����
#define	 SDRAMCH_ERROR	      0xAA16	 //SDRAM�Լ첻�ɹ�����
#define  CHECK_OVER			  0xAA1C	 //�Լ����

/*У�鴦��*/
#define  CHECK_OK 		      0xAA01	 //����У����ȷ
#define  CHECK_ERR            0xAA00	 //����У�����

/*�������*/
#define  CHAR_DATASEND	      0x5555	 //�ַ�����ʾ��ַ
#define  NUM_DATASEND	      0x5553	 //���ַ�������

/*�ѻ���ʾʵ��*/
/*�����ʵ���*/
#define  SAMPL6K     0xAB66     //������Ϊ6k
#define  SAMPL8K     0xAB88		//������Ϊ8k
#define  SAMPL44K    0xAB44		//������Ϊ44k
#define  SAMPL96K    0xAB96		//������Ϊ96k 
//���
#define MTR_START		0xAB4A///ʵ�鿪ʼ
#define MTR_OVER		0xAB4B///�˳�ʵ��

#define STP_HALT 	    0xAB40//�������ֹͣ
#define STP_RUN   		0xAB41//�����������
#define STP_RVS  	    0xAB42//�����������
#define DC_RUN   	    0xAB43//ֱ���������
#define DC_HALT 		0xAB44//ֱ�����ɲ��
#define DC_RVS			0xAB45//ֱ���������
#define MTRCONFIG 	    0xAB46//�������


typedef struct _MotorConfig
{
	unsigned int StpMode;		//�������ģʽ
	unsigned int StpDir;		//�����������
	unsigned int StpHlfStp;		//��������벽
	unsigned int StpSpdDgr;	    //��������ٶȡ�����
	unsigned int DcDir;		    //ֱ���������
	unsigned int DcSpd;		    //ֱ������ٶ�
} MotorConfig, *PMotorConfig;
//��������벽
#define FULLSTEP	    0x0
#define HALFSTEP	    0x1

//���������
#define CLOCKWISE	    0x0
#define ANTICLOCKWISE	0x1

//�������ģʽ����
#define ROTATE		    0x0
#define LOCATE		    0x1

//����ӿ�
#define  MOTOR_START	0xAA30	 //���ʵ�鿪ʼ
#define  MOTOR_OVER     0xAA32	 //���ʵ�����

#define MOTOR_HALT 	    0xAA34//���ֹͣ
#define MOTOR_RUN   	0xAA35//�������
#define MOTOR_RVS  	    0xAA36//�������

#define MOTOR_CLOCKW  	0xAA37//�������
#define MOTOR_CCLOCK    0xAA38//�������

//��Ƶ
#define  ADI_START		0xAB30	 //ʵ�鿪ʼ
#define  ADI_OVER		0xAB31	 //�˳�ʵ��

#define  ADI_Audio		0xAB3A	 //����ʵ�� modify
#define  ADI_ECHO       0xAB3B	 //����ʵ��
#define  ADI_MIX		0xAB3C	 //����ʵ��
//�˲�
/*FILTʵ������֡ʹ�����������֣����Data[0]��*/
#define  FILT_START		 	0xAB20	 //ʵ�鿪ʼ
#define  FILT_SET        	0xAB21	 //ʵ������
#define  FILT_OVER       	0xAB22	 //ʵ�����


/*FILT��������֡�еĲ�������Data[1]��ʼ��������Ϊ����ṹ�е�Ԫ��*/
typedef struct _FILT_Config
{
	unsigned int Fren;	    //����Ƶ�� 
	unsigned int Len; 	    //��������
    unsigned int FilterType;    //�˲�������
} FILTConfig, *PFILTConfig;
//AD
/*ADʵ������֡ʹ�����������֣����Data[0]��*/
#define  AD_START		 	0xAB00	 //ʵ�鿪ʼ
#define  AD_SET		 	    0xAB01	 //ʵ������
#define  AD_SAMPLEOVER 	    0xAB02	 //��������
#define  AD_OVER       	    0xAB03	 //ʵ�����
#define  AD_DATASEND	 	0xAB04	 //���ݷ���


/*AD��������֡�еĲ�������Data[1]��ʼ��������Ϊ����ṹ�е�Ԫ��*/
typedef struct _AD_Config
{
	unsigned int Fren;	    //����Ƶ�� 
	unsigned int Len; 	    //��������
} ADConfig, *PADConfig;

//FFT
/*FFTʵ������֡ʹ�����������֣����Data[0]��*/
#define  FFT_START		 	0xAB10	 //ʵ�鿪ʼ
#define  FFT_SET        	0xAB11	 //ʵ������
#define  FFT_OVER       	0xAB13	 //ʵ�����

/*FFT��������֡�еĲ�������Data[1]��ʼ��������Ϊ����ṹ�е�Ԫ��*/
typedef struct _FFT_Config
{
	unsigned int Fren;	    //����Ƶ�� 
	unsigned int Len; 	    //��������
} FFTConfig, *PFFTConfig;

/*����IOʵ������֡ʹ�����������֣����Data[0]��*/
#define TRAF_START		0xAB50	 //ʵ�鿪ʼ
#define TRAF_OVER    	0xAB51	 //ʵ�����
/*����IO����֡�еĲ�������Data[1]��ʼ��*/
#define TRAF_AUTO		0xAB54	//��ͨ���Զ�ģʽ
#define TRAF_MANNUL		0xAB55	//��ͨ���ֶ�ģʽ
#define TRAF_NIGHT		0xAB56	//��ͨ��ҹ��ģʽ
#define TRAF_RESET		0xAB59	//��ͨ�Ƹ�λ

#define TRAF_EAST		0xAB5A	//��ͨ�ƶ���ͨ��
#define TRAF_SOUTH		0xAB5B	//��ͨ���ϱ�ͨ��
#define TRAF_FORBID		0xAB5C	//��ͨ�ƽ���

#define  EASTEWEST     0x88c	//��ͨ�ƶ���ͨ���ϱ����У�
#define  SOUTHNORTH    0x311	//��ͨ���ϱ�ͨ���������У�  
#define  IOCHANGE      0x462	//��ͨ�Ƹ�����Ƶ���
#define  ALLFORBIN     0x914	//��ͨ�Ƹ����������

/*�����ʵ���*/
#define  SAMPLRATE6K        0x06//������Ϊ6k
#define  SAMPLRATE8K        0x03//������Ϊ8k
#define  SAMPLRATE44K       0x0 //������Ϊ44k
#define  SAMPLRATE96K       0x07//������Ϊ96k 

#endif
/************************�û���չʹ��*********************************/
#define  USE_START    0xFF00//�û����ʼ
#define  USE_END      0xFF01//�û��������
#define  USE_EDIT        0xFFA0//�༭�˵�
#define  USE_OPTION      0xFFA1//ѡ��˵�

/***********************************************************************/
//	No	more
/***********************************************************************/
