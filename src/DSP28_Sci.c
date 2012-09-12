//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Sci.c
//
// TITLE:	DSP28 SCI Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#include "DSP28_Device.h"

//---------------------------------------------------------------------------
// InitSPI: 
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSci(void)
{
	// Initialize SCI-A:
	
	EALLOW;
	GpioMuxRegs.GPFMUX.all = 0x0030;
	EDIS;
	/* loopback   8 bit data */
	SciaRegs.SCICCR.all = 0x07;	// 1 bit stop, disable parity, idle mode, 8 bits data 
	
	SciaRegs.SCICTL1.all = 0x03; // enable TX 
	SciaRegs.SCICTL2.all = 0x03; //
	////////////////////9600///19200/////
	SciaRegs.SCIHBAUD = 0x00;//0x01;//0x00;
	SciaRegs.SCILBAUD = 0xF3;//0x44;//0xa2;
	
	SciaRegs.SCICTL1.all = 0x23;
	//tbd...
 	

	// Initialize SCI-B:

	//tbd...
}

/********************************************************************************
	name:	int SciaTx_Ready(void)
	input:	none
	output:	i	1:	ready
			0:	busy
*********************************************************************************/
                                     
/* int SciaTx_Ready(void)               */
/* {                                    */
/* 	unsigned int i;                     */
/* 	if(SciaRegs.SCICTL2.bit.TXRDY == 1) */
/* 	{                                   */
/* 		i = 1;                          */
/* 	}                                   */
/* 	else                                */
/* 	{                                   */
/* 		i = 0;                          */
/* 	}                                   */
/* 	return(i);                          */
/* }                                    */

/********************************************************************************
	name:	int SciaRx_Ready(void)
	input:	none
	output:	i	1:	new data
			0:	none
*********************************************************************************/
/*                                      */
/* int SciaRx_Ready(void)               */
/* {                                    */
/* 	unsigned int i;                     */
/* 	if(SciaRegs.SCIRXST.bit.RXRDY == 1) */
/* 	{                                   */
/* 		i = 1;                          */
/* 	}                                   */
/* 	else                                */
/* 	{                                   */
/* 		i = 0;                          */
/* 	}                                   */
/* 	return(i);                          */
/* }                                    */


	
//===========================================================================
// No more.
//===========================================================================
