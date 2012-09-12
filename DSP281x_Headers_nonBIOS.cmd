/*
//###########################################################################
//
// FILE:    DSP281x_Headers_BIOS.cmd
//
// TITLE:   DSP281x Peripheral registers linker command file 
//
// DESCRIPTION: 
// 
//          This file is for use in BIOS applications.
//
//          Linker command file to place the peripheral structures 
//          used within the DSP281x headerfiles into the correct memory
//          mapped locations.
//
//          This version of the file does not include the PieVectorTable structure.
//          For non-BIOS applications, please use the DSP281x_Headers_nonBIOS.cmd 
//          file which includes the PieVectorTable structure.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//      | 05 Mar 2003 | D.A. | Original based on DSP281x v0.58
//  1.00| 11 Sep 2003 | L.H. | Integrated into DSP281x header files
//      |             |      | Added missing eCAN file sections
// -----|-------------|------|-----------------------------------------------
//###########################################################################
*/

MEMORY
{
 PAGE 0:    /* Program Memory */

 PAGE 1:    /* Data Memory */
 
    /* Peripheral Frame 0:   */
   DEV_EMU    : origin = 0x000880, length = 0x000180
   FLASH_REGS : origin = 0x000A80, length = 0x000060
   CSM        : origin = 0x000AE0, length = 0x000010
   XINTF      : origin = 0x000B20, length = 0x000020
   CPU_TIMER0 : origin = 0x000C00, length = 0x000008
   CPU_TIMER1 : origin = 0x000C08, length = 0x000008		 
   CPU_TIMER2 : origin = 0x000C10, length = 0x000008		 
   PIE_CTRL   : origin = 0x000CE0, length = 0x000020
   PIE_VECT   : origin = 0x000D00, length = 0x000100

   /* Peripheral Frame 1:   */
   ECAN_A     : origin = 0x006000, length = 0x000100
   ECAN_AMBOX : origin = 0x006100, length = 0x000100

   /* Peripheral Frame 2:   */
   SYSTEM     : origin = 0x007010, length = 0x000020
   SPI_A      : origin = 0x007040, length = 0x000010
   SCI_A      : origin = 0x007050, length = 0x000010
   XINTRUPT   : origin = 0x007070, length = 0x000010
   GPIOMUX    : origin = 0x0070C0, length = 0x000020
   GPIODAT    : origin = 0x0070E0, length = 0x000020
   ADC        : origin = 0x007100, length = 0x000020
   EV_A       : origin = 0x007400, length = 0x000040
   EV_B       : origin = 0x007500, length = 0x000040
   SPI_B      : origin = 0x007740, length = 0x000010
   SCI_B      : origin = 0x007750, length = 0x000010
   MCBSP_A    : origin = 0x007800, length = 0x000040

   /* CSM Password Locations */
   CSM_PWL    : origin = 0x3F7FF8, length = 0x000008  /* Part of FLASHA.  CSM password locations. */
}

 
SECTIONS
{

/*** The PIE Vector table is called PIEVECT by DSP/BIOS ***/
    DevEmuRegsFile    : > DEV_EMU,    PAGE = 1
   FlashRegsFile     : > FLASH_REGS, PAGE = 1
   CsmRegsFile       : > CSM,        PAGE = 1
   XintfRegsFile     : > XINTF,      PAGE = 1
   CpuTimer0RegsFile : > CPU_TIMER0, PAGE = 1      
   CpuTimer1RegsFile : > CPU_TIMER1, PAGE = 1      
   CpuTimer2RegsFile : > CPU_TIMER2, PAGE = 1      
   PieCtrlRegsFile   : > PIE_CTRL,   PAGE = 1      
   PieVectTable      : > PIE_VECT,   PAGE = 1

   /* Allocate Peripheral Frame 2 Register Structures:   */
   ECanaRegsFile     : > ECAN_A,      PAGE = 1   
   ECanaMboxesFile   : > ECAN_AMBOX   PAGE = 1

   /* Allocate Peripheral Frame 1 Register Structures:   */
   SysCtrlRegsFile   : > SYSTEM,     PAGE = 1
   SpiaRegsFile      : > SPI_A,      PAGE = 1
   SciaRegsFile      : > SCI_A,      PAGE = 1
   XIntruptRegsFile  : > XINTRUPT,   PAGE = 1
   GpioMuxRegsFile   : > GPIOMUX,    PAGE = 1
   GpioDataRegsFile  : > GPIODAT     PAGE = 1
   AdcRegsFile       : > ADC,        PAGE = 1
   EvaRegsFile       : > EV_A,       PAGE = 1
   EvbRegsFile       : > EV_B,       PAGE = 1
   ScibRegsFile      : > SCI_B,      PAGE = 1
   McbspRegsFile    : > MCBSP_A,    PAGE = 1

   /* CSM Password Locations */
   CsmPwlFile      : > CSM_PWL,     PAGE = 1
}


/******************* end of file ************************/