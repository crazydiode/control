/*****************************************************************************/
/* COMM.H	v1.00														     */
/* 版权(c)	2003-   	北京合众达电子技术有限责任公司						 */
/* 设计者:	段立锋															 */
/*****************************************************************************/
#ifndef _prtocol_H_
#define _protocol_H_

/********************************************************************************/
/* 常量定义																		*/
/********************************************************************************/
#define Word                  0xaa55  //代表文字
#define Number                0x55aa  //代表数字

/*帧类型的设定*/
#define  FRAME_DATA           0x4        //数据帧
#define  FRAME_CMD            0x2        //命令帧
#define  FRAME_DATASEND       0x1        //数据传送帧
#define  FRAME_DATAEND        0x3        //数据传送结束帧
#define  USE_DATASEND		  0x5		 //用户数据传送
#define  USE_DATAEND		  0x6		 //用户数据结束

//帧标志
//多帧使用
#define  FRAME_CONT   		  0x1        //中间帧
#define  FRAME_END		      0x2        //结束帧
//单帧使用
#define FRAME_SING		      0x3        //单帧


/*握手信号*/
#define  HAND 		          0xAAA0	 //握手信号

/*自检功能*/ 
#define  SRAMCH_OK 	      	  0xAA11	 //SRAM自检成功命令
#define  SRAMCH_ERROR 	      0xAA12	 //SRAM自检不成功命令
#define	 SDRAMCH_OK	          0xAA15	 //SDRAM自检成功命令
#define	 SDRAMCH_ERROR	      0xAA16	 //SDRAM自检不成功命令
#define  CHECK_OVER			  0xAA1C	 //自检结束

/*校验处理*/
#define  CHECK_OK 		      0xAA01	 //数据校验正确
#define  CHECK_ERR            0xAA00	 //数据校验错误

/*结果传送*/
#define  CHAR_DATASEND	      0x5555	 //字符串显示地址
#define  NUM_DATASEND	      0x5553	 //数字发送命令

/*脱机演示实验*/
/*采样率调整*/
#define  SAMPL6K     0xAB66     //采样率为6k
#define  SAMPL8K     0xAB88		//采样率为8k
#define  SAMPL44K    0xAB44		//采样率为44k
#define  SAMPL96K    0xAB96		//采样率为96k 
//电机
#define MTR_START		0xAB4A///实验开始
#define MTR_OVER		0xAB4B///退出实验

#define STP_HALT 	    0xAB40//步进电机停止
#define STP_RUN   		0xAB41//步进电机运行
#define STP_RVS  	    0xAB42//步进电机反向
#define DC_RUN   	    0xAB43//直流电机运行
#define DC_HALT 		0xAB44//直流电机刹车
#define DC_RVS			0xAB45//直流电机反向
#define MTRCONFIG 	    0xAB46//电机设置


typedef struct _MotorConfig
{
	unsigned int StpMode;		//步进电机模式
	unsigned int StpDir;		//步进电机方向
	unsigned int StpHlfStp;		//步进电机半步
	unsigned int StpSpdDgr;	    //步进电机速度、步数
	unsigned int DcDir;		    //直流电机方向
	unsigned int DcSpd;		    //直流电机速度
} MotorConfig, *PMotorConfig;
//步进电机半步
#define FULLSTEP	    0x0
#define HALFSTEP	    0x1

//电机方向常量
#define CLOCKWISE	    0x0
#define ANTICLOCKWISE	0x1

//步进电机模式常量
#define ROTATE		    0x0
#define LOCATE		    0x1

//电机子卡
#define  MOTOR_START	0xAA30	 //电机实验开始
#define  MOTOR_OVER     0xAA32	 //电机实验结束

#define MOTOR_HALT 	    0xAA34//电机停止
#define MOTOR_RUN   	0xAA35//电机运行
#define MOTOR_RVS  	    0xAA36//电机反向

#define MOTOR_CLOCKW  	0xAA37//电机运行
#define MOTOR_CCLOCK    0xAA38//电机反向

//音频
#define  ADI_START		0xAB30	 //实验开始
#define  ADI_OVER		0xAB31	 //退出实验

#define  ADI_Audio		0xAB3A	 //试听实验 modify
#define  ADI_ECHO       0xAB3B	 //回音实验
#define  ADI_MIX		0xAB3C	 //混响实验
//滤波
/*FILT实验命令帧使用如下命令字（存放Data[0]）*/
#define  FILT_START		 	0xAB20	 //实验开始
#define  FILT_SET        	0xAB21	 //实验设置
#define  FILT_OVER       	0xAB22	 //实验结束


/*FILT设置命令帧中的参数（从Data[1]开始），依次为下面结构中的元素*/
typedef struct _FILT_Config
{
	unsigned int Fren;	    //采样频率 
	unsigned int Len; 	    //采样长度
    unsigned int FilterType;    //滤波器类型
} FILTConfig, *PFILTConfig;
//AD
/*AD实验命令帧使用如下命令字（存放Data[0]）*/
#define  AD_START		 	0xAB00	 //实验开始
#define  AD_SET		 	    0xAB01	 //实验设置
#define  AD_SAMPLEOVER 	    0xAB02	 //采样结束
#define  AD_OVER       	    0xAB03	 //实验结束
#define  AD_DATASEND	 	0xAB04	 //数据发送


/*AD设置命令帧中的参数（从Data[1]开始），依次为下面结构中的元素*/
typedef struct _AD_Config
{
	unsigned int Fren;	    //采样频率 
	unsigned int Len; 	    //采样长度
} ADConfig, *PADConfig;

//FFT
/*FFT实验命令帧使用如下命令字（存放Data[0]）*/
#define  FFT_START		 	0xAB10	 //实验开始
#define  FFT_SET        	0xAB11	 //实验设置
#define  FFT_OVER       	0xAB13	 //实验结束

/*FFT设置命令帧中的参数（从Data[1]开始），依次为下面结构中的元素*/
typedef struct _FFT_Config
{
	unsigned int Fren;	    //采样频率 
	unsigned int Len; 	    //采样长度
} FFTConfig, *PFFTConfig;

/*数字IO实验命令帧使用如下命令字（存放Data[0]）*/
#define TRAF_START		0xAB50	 //实验开始
#define TRAF_OVER    	0xAB51	 //实验结束
/*数字IO命令帧中的参数（从Data[1]开始）*/
#define TRAF_AUTO		0xAB54	//交通灯自动模式
#define TRAF_MANNUL		0xAB55	//交通灯手动模式
#define TRAF_NIGHT		0xAB56	//交通灯夜间模式
#define TRAF_RESET		0xAB59	//交通灯复位

#define TRAF_EAST		0xAB5A	//交通灯东西通行
#define TRAF_SOUTH		0xAB5B	//交通灯南北通行
#define TRAF_FORBID		0xAB5C	//交通灯禁行

#define  EASTEWEST     0x88c	//交通灯东西通（南北禁行）
#define  SOUTHNORTH    0x311	//交通灯南北通（东西禁行）  
#define  IOCHANGE      0x462	//交通灯各方向黄灯亮
#define  ALLFORBIN     0x914	//交通灯各方向均禁行

/*采样率调整*/
#define  SAMPLRATE6K        0x06//采样率为6k
#define  SAMPLRATE8K        0x03//采样率为8k
#define  SAMPLRATE44K       0x0 //采样率为44k
#define  SAMPLRATE96K       0x07//采样率为96k 

#endif
/************************用户扩展使用*********************************/
#define  USE_START    0xFF00//用户命令开始
#define  USE_END      0xFF01//用户命令结束
#define  USE_EDIT        0xFFA0//编辑菜单
#define  USE_OPTION      0xFFA1//选项菜单

/***********************************************************************/
//	No	more
/***********************************************************************/
