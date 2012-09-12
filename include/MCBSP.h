
#ifndef MCBSP_H
#define MCBSP_H

#define  FRAMLENGTH    0x80
#define  DataLength    0x7C

typedef struct McBSPForDec{
	unsigned int  Length;
	unsigned int  Type;
	unsigned int  Mutul;
	unsigned int  Data[DataLength];
	unsigned int  Check;
}McBSPForDec, *PmcbspForDec; 

extern PmcbspForDec psend;
extern PmcbspForDec preceive;
extern unsigned int mcbspx[FRAMLENGTH];
extern unsigned int mcbspr[FRAMLENGTH];
extern unsigned int datarevlength;
extern unsigned int timeout;
//变量
extern unsigned int RevBuffer[FRAMLENGTH];

//中断
/***********************************************************************/
/*  函数声明：	接收中断的中断程序                                     */
/***********************************************************************/
void interrupt McbspRevIsr(void);
//***********************************************************************/
/*	函数声明：	MCBSP数据发送                                          */
/*                                                                     */
/*	函数功能：	每次发送一帧的长度                                     */
/*                                                                     */
/*	参数：		addr,发送数据的地址                                    */
/***********************************************************************/
extern void mcbsp_tx(unsigned int * addr);
/***********************************************************************/
/*	函数声明：	MCBSP数据接收                                          */
/*                                                                     */
/*	函数功能：	MCBSP数据接收并校验，如果串口接收错误或者较验出错，要求*/
/*              重发，并进行重新接收。                                 */
/*                                                                     */
/*  参数：      recvaddr:    缓冲区的首址                              */
/*                                                                     */
/*  返回值：    0:  接收正确                                           */
/*              1:  若重发次数大于3，则系统出错                        */
/***********************************************************************/
int mcbsp_rece(unsigned int * recvaddr);
/***********************************************************************/
/*	函数声明：	MCBSP数据接收                                          */
/*                                                                     */
/*	函数功能：	响应中断，读取MCBSP的接收BUFFER内的数据，将数据        */
/*			    放在RevBuffer变量中                                    */
/*                                                                     */
/*  参数：                                                             */
/*              Channel：    有效的串口句柄			    			   */
/*	            datarevlength:  每次从缓冲区读取的长度                 */
/*	            recvaddr:    缓冲区的首址                              */
/*                                                                     */
/*  返回值      0:接收完成  				             			   */
/*              1：数据未准备好                                        */
/*              0xFFFF:串口接收错误                                    */
/*              2：中断超时错误                                        */
/*              3: 较验出错                                            */
/***********************************************************************/
unsigned int mcbsp_rx(unsigned int * recvaddr);
/***********************************************************************/
/*  函数声明：	串口接收状态,并设置相应的寄存器默认值  	               */
/*                                                                     */
/*  返回值      0:接收完成  				         				   */
/*              1：数据未准备好                                        */
/*              0xFFFF:串口接收错误                                    */
/*              2：中断超时错误                                        */
/*              3: 较验出错                                            */
/***********************************************************************/
int mcbsp_status();

#endif
/***********************************************************************/
//	No	more
/***********************************************************************/
