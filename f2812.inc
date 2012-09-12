* File: 	f2812.inc					*
* 
* Include file with CPU/Periperal register declarations		*

*F2812 XINTF configuration AND control register mapping
XTIMING0	.set	0b20H		;xintf timing register,zone0 access
XTIMING1	.set	0b22H		;xintf timing register,zone1 access
XTIMING2	.set	0b24H		;xintf timing register,zone2 access
XTIMING6	.set	0b2cH		;xintf timing register,zone6 access
XTIMING7	.set	0b2eH		;xintf timing register,zone7 access
XINTCNF2	.set	0b34H		;xintf configuration register
XBANK		.set	0b38H		;xintf bank control register

* F2812 cpu timer 0/1/2 registers
TIMER0TIM	.set	0C00H
TIMER0TIMH	.set	0C01H
TIMER0PRD	.set	0C02H
TIMER0PRDH	.set	0C03H
TIMER0TCR	.set	0C04H
TIMER0TPR	.set	0C06H		;TPRlo is TDDR and TPRhi is PSC
TIMER0TPRH	.set	0C07H

TIMER1TIM	.set	0C08H
TIMER1TIMH	.set	0C09H
TIMER1PRD	.set	0C0AH
TIMER1PRDH	.set	0C0BH
TIMER1TCR	.set	0C0CH
TIMER1TPR	.set	0C0EH	        ;TPRlo is TDDR and TPRhi is PSC
TIMER1TPRH	.set	0C0FH

TIMER2TIM	.set	0C10H
TIMER2TIMH	.set	0C11H
TIMER2PRD	.set	0C12H
TIMER2PRDH	.set	0C13H
TIMER2TCR	.set	0C14H
TIMER2TPR	.set	0C16H       ; TPRlo is TDDR and TPRhi is PSC
TIMER2TPRH	.set	0C17H

*F2812 PIE Configuration and Control Registers
PIECTRL		.set	0CE0H		;PIE,Configuration
PIEACK		.set	0CE1H		;PIE,Acknowledge Register
PIEIER1		.set	0CE2H		;PIE,INT1 Group Enable Register
PIEIFR1		.set	0CE3H		;PIE,INT1 Group Flag Register
PIEIER2		.set	0CE4H		;PIE,INT2 Group Enable Register
PIEIFR2		.set	0CE5H		;PIE,INT2 Group Flag Register
PIEIER3		.set	0CE6H		;PIE,INT3 Group Enable Register
PIEIFR3		.set	0CE7H		;PIE,INT3 Group Flag Register
PIEIER4		.set	0CE8H		;PIE,INT4 Group Enable Register
PIEIFR4		.set	0CE9H		;PIE,INT4 Group Flag Register
PIEIER5		.set	0CEAH		;PIE,INT5 Group Enable Register
PIEIFR5		.set	0CEBH		;PIE,INT5 Group Flag Register
PIEIER6		.set	0CECH		;PIE,INT6 Group Enable Register
PIEIFR6		.set	0CEDH		;PIE,INT6 Group Flag Register
PIEIER7		.set	0CEEH		;PIE,INT7 Group Enable Register
PIEIFR7		.set	0CEFH		;PIE,INT7 Group Flag Register
PIEIER8		.set	0CF0H		;PIE,INT8 Group Enable Register
PIEIFR8		.set	0CF1H		;PIE,INT8 Group Flag Register
PIEIER9		.set	0CF2H		;PIE,INT9 Group Enable Register
PIEIFR9		.set	0CF3H		;PIE,INT9 Group Flag Register
PIEIER10	.set	0CF4H		;PIE,INT10 Group Enable Register
PIEIFR10	.set	0CF5H		;PIE,INT10 Group Flag Register
PIEIER11	.set	0CF6H		;PIE,INT11 Group Enable Register
PIEIFR11	.set	0CF7H		;PIE,INT11 Group Flag Register
PIEIER12	.set	0CF8H		;PIE,INT12 Group Enable Register
PIEIFR12	.set	0CF9H		;PIE,INT12 Group Flag Register

*F2812 CAN Registers Map
CANME		.set	6000H		;Mailbox enable
CANMD		.set	6002H		;Mailbox direction
CANTRS		.set	6004H		;Tranmit request set
CANTRR		.set	6006H		;Tranmit request reset
CANTA		.set	6008H		;Tranmission acknowledge
CANAA		.set	600aH		;Abort acknowledge
CANRMP		.set	600cH		;Receive message pending
CANRML		.set	600eH		;Receive message lost
CANRFP		.set	6010H		;Remote frame pending
CANGAM		.set	6012H		;Global acceptance mask
CANMC		.set	6014H		;Master control
CANBTC		.set	6016H		;Bit-timing configuration
CANES		.set	6018H		;Error and status
CANTEC		.set	601aH		;Transmit error counter
CANREC		.set	601cH		;Receive error counter
CANGIF0		.set	601eH		;Global interrupt flag 0
CANGIM		.set	6020H		;Global interrupt mask
CANGIF1		.set	6022H		;Global interrupt flag 1
CANMIM		.set	6024H		;Mailbox interrupt mask
CANMIL		.set	6026H		;Mailbox interrupt level
CANOPC		.set	6028H		;Overwrite protection control
CANTIOC		.set	602aH		;TX I/O control
CANRIOC		.set	602cH		;RX I/O control
CANTSC		.set	602eH		;Time stamp counter
CANTOC		.set	6030H		;Time-out control
CANTOS		.set	6032H		;Time-out status

*F2812 system registers
HISPCP		.set	701AH		;high-speed Peripheral clock register
LOSPCP		.set	701BH		;low-speed Peripheral clock register
PCLKCR		.set	701CH		;Peripheral clock control register
LPMCR0		.set	701EH		;low power mode control register 0
LPMCR1		.set	701FH		;low power mode control register 1
PLLCR		.set	7021H		;PLL Control Register
SCSR		.set	7022H		;System Control & Status Register
WDCNTR		.set	7023H		;Watchdog Counter Register
WDkey		.set	7025H		;Watchdog Reset Key Register
WDCR		.set	7029H		;Watchdog Control Register

*F2812 SPI Registers
SPICCR		.set	7040H		;SPI Configuration Control Register
SPICTL		.set	7041H		;SPI Operation Control Register
SPISTS		.set	7042H		;SPI Status Register
SPIBRR		.set	7044H		;SPI Baud Rate Register
SPIRXEMU	.set	7046H		;SPI Receive Emulation Buffer Register
SPIRXBUF	.set	7047H		;SPI Serial Input Buffer Register
SPITXBUF	.set	7048H		;SPI Serial Output Buffer Register
SPIDAT		.set	7049H		;SPI Serial Data Register
SPIFFTX		.set	704aH		;SPI FIFO Transmit Register
SPIFFRX		.set	704bH		;SPI FIFO Receive Register
SPIFFCT		.set	704cH		;SPI FIFO Control Register
SPIPRI		.set	704fH		;SPI Priority Control Register

*F2812 SCI-A Registers
SCICCRA		.set	7050H		;SCI-A Communications Control Register
SCICTL1A	.set	7051H		;SCI-A Control Register 1
SCIHBAUDA	.set	7052H		;SCI-A Baud Register,High Bits
SCILBAUDA	.set	7053H		;SCI-A Baud Register,Low Bits
SCICTL2A	.set	7054H		;SCI-A Control Register 2
SCIRXSTA	.set	7055H		;SCI-A Receive Status Register
SCIRXEMUA	.set	7056H		;SCI-A Receive Emulation Data Buffer Register
SCIRXBUFA	.set	7057H		;SCI-A Receive Data Buffer Register
SCITXBUFA	.set	7059H		;SCI-A Transmit Data Buffer Register
SCIFFTXA	.set	705aH		;SCI-A FIFO Transmit Register
SCIFFRXA	.set	705bH		;SCI-A FIFO Receive Register
SCIFFCTA	.set	705cH		;SCI-A FIFO Control Register
SCIPRIA		.set	705fH		;SCI-A Priority Control Register

*F2812 External Interrupts Registers
XINT1CR		.set	7070H		;xint1 control register
XINT2CR		.set	7071H		;xint2 control register
XNMICR		.set	7077H		;xnmi control register
XINT1CTR	.set	7078H		;xint1 counter register
XINT2CTR	.set	7079H		;xint2 counter register
XNMICTR		.set	707fH		;xnmi counter register

*F2812 GPIO Mux Registers
GPAMUX		.set	70c0H		;GPIO A Mux Control Register
GPADIR		.set	70c1H		;GPIO A Direction Control Register
GPAQUAL		.set	70c2H		;GPIO A Input Qualification Control Register

GPBMUX		.set	70C4H		;GPIO B Mux Control Register
GPBDIR		.set	70C5H		;GPIO B Direction Control Register
GPBQUAL		.set	70C6H		;GPIO B Input Qualification Control Register

GPDMUX		.set	70CCH		;GPIO D Mux Control Register
GPDDIR		.set	70CDH		;GPIO D Direction Control Register
GPDQUAL		.set	70CEH		;GPIO D Input Qualification Control Register

GPEMUX		.set	70d0H		;GPIO E Mux Control Register
GPEDIR		.set	70d1H		;GPIO E Direction Control Register
GPEQUAL		.set	70d2H		;GPIO E Input Qualification Control Register

GPFMUX		.set	70D4H		;GPIO F Mux Control Register
GPFDIR		.set	70D5H		;GPIO F Direction Control Register

GPGMUX		.set	70D8H		;GPIO G Mux Control Register
GPGDIR		.set	70D9H		;GPIO G Direction Control Register

*F2812 GPIO Data Registers
GPADAT		.set	70E0H		;GPIO A Data Register
GPASET		.set	70E1H		;GPIO A Set Register
GPACLEAR	.set	70E2H		;GPIO A Clear Register
GPATOGGLE	.set	70E3H		;GPIO A Toggle Register

GPBDAT		.set	70E4H		;GPIO B Data Register
GPBSET		.set	70E5H		;GPIO B Set Register
GPBCLEAR	.set	70E6H		;GPIO B Clear Register
GPBTOGGLE	.set	70E7H		;GPIO B Toggle Register

GPDDAT		.set	70ECH		;GPIO D Data Register
GPDSET		.set	70EDH		;GPIO D Set Register
GPDCLEAR	.set	70EEH		;GPIO D Clear Register
GPDTOGGLE	.set	70EFH		;GPIO D Toggle Register

GPEDAT		.set	70F0H		;GPIO E Data Register
GPESET		.set	70F1H		;GPIO E Set Register
GPECLEAR	.set	70F2H		;GPIO E Clear Register
GPETOGGLE	.set	70F3H		;GPIO E Toggle Register

GPFDAT		.set	70F4H		;GPIO F Data Register
GPFSET		.set	70F5H		;GPIO F Set Register
GPFCLEAR	.set	70F6H		;GPIO F Clear Register
GPFTOGGLE	.set	70F7H		;GPIO F Toggle Register

GPGDAT		.set	70F8H		;GPIO G Data Register
GPGSET		.set	70F9H		;GPIO G Set Register
GPGCLEAR	.set	70FAH		;GPIO G Clear Register
GPGTOGGLE	.set	70FBH		;GPIO G Toggle Register

*F2812 ADC Registers
ADCTRL1		.set	7100H		;ADC Control Register 1
ADCTRL2		.set	7101H		;ADC Control Register 2
ADCMAXCONV	.set	7102H		;ADC Maximum Conversion Channels Register
ADCCHSELSEQ1	.set	7103H		;ADC Channel Select Sequencing Control Register 1
ADCCHSELSEQ2	.set	7104H		;ADC Channel Select Sequencing Control Register 2
ADCCHSELSEQ3	.set	7105H		;ADC Channel Select Sequencing Control Register 3
ADCCHSELSEQ4	.set	7106H		;ADC Channel Select Sequencing Control Register 4
ADCASEQSR	.set	7107H		;ADC Auto-Sequence Status Register
ADCRESULT0	.set	7108H		;ADC Conversion Result Buffer Register 0
ADCRESULT1	.set	7109H		;ADC Conversion Result Buffer Register 1
ADCRESULT2	.set	710aH		;ADC Conversion Result Buffer Register 2
ADCRESULT3	.set	710bH		;ADC Conversion Result Buffer Register 3
ADCRESULT4	.set	710cH		;ADC Conversion Result Buffer Register 4
ADCRESULT5	.set	710dH		;ADC Conversion Result Buffer Register 5
ADCRESULT6	.set	710eH		;ADC Conversion Result Buffer Register 6
ADCRESULT7	.set	710fH		;ADC Conversion Result Buffer Register 7
ADCRESULT8	.set	7110H		;ADC Conversion Result Buffer Register 8
ADCRESULT9	.set	7111H		;ADC Conversion Result Buffer Register 9
ADCRESULT10	.set	7112H		;ADC Conversion Result Buffer Register 10
ADCRESULT11	.set	7113H		;ADC Conversion Result Buffer Register 11
ADCRESULT12	.set	7114H		;ADC Conversion Result Buffer Register 12
ADCRESULT13	.set	7115H		;ADC Conversion Result Buffer Register 13
ADCRESULT14	.set	7116H		;ADC Conversion Result Buffer Register 14
ADCRESULT15	.set	7117H		;ADC Conversion Result Buffer Register 15
ADCTRL3		.set	7118H		;ADC Control Register 3
ADCST		.set	7119H		;ADC Status Register

*F2812 EVA Registers
GPTCONA		.set	7400H		;GP Timer control register a
T1CNT		.set	7401H		;GP Timer 1 counter Register
T1CMPR		.set	7402H		;GP Timer 1 Compare Register
T1PR		.set	7404H		;GP Timer 1 Period Register
T1CON		.set	7405H		;GP Timer 1 Control Register
T2CNT		.set	7406H		;GP Timer 2 Control Register
T2CMPR		.set	7406H		;GP Timer 2 Compare Register
T2PR		.set	7407H		;GP Timer 2 Period Register
T2CON		.set	7408H		;GP Timer 2 Control Register
EXTCONA		.set	7409H		;External Control Register A
COMCONA		.set	7411H		;Compare Control Register A
ACTRA		.set	7413H		;Compare Action Control Register A
DBTCONA		.set	7415H		;Dead-Band Timer Control Register a
CMPR1		.set	7417H		;Compare Register 1
CMPR2		.set	7418H		;Compare Register 2
CMPR3		.set	7419H		;Compare Register 3
CAPCONA		.set	7420H		;Capture Control Register A
CAPFIFOA	.set	7422H		;Capture FIFO Status Register A
CAP1FIFO	.set	7423H		;Capture FIFO Stack 1
CAP2FIFO	.set	7424H		;Capture FIFO Stack 2
CAP3FIFO	.set	7425H		;Capture FIFO Stack 3
CAP1FBOT	.set	7427H		;Bottom Register Of Capture FIFO Stack 1
CAP2FBOT	.set	7428H		;Bottom Register Of Capture FIFO Stack 2
CAP3FBOT	.set	7429H		;Bottom Register Of Capture FIFO Stack 3
EVAIMRA		.set	742cH		;Interrupt Mask Register A
EVAIMRB		.set	742dH		;Interrupt Mask Register B
EVAIMRC		.set	742eH		;Interrupt Mask Register C
EVAIFRA		.set	742fH		;Interrupt Flag Register A
EVAIFRB		.set	7430H		;Interrupt Flag Register B
EVAIFRC		.set	7431H		;Interrupt Flag Register C

*F2812 SCI-B Registers
SCICCRB		.set	7750H		;SCI-B Communications Control Register
SCICTL1B	.set	7751H		;SCI-B Control Register 1
SCIHBAUDB	.set	7752H		;SCI-B Baud Register,High Bits
SCILBAUDB	.set	7753H		;SCI-B Baud Register,Low Bits
SCICTL2B	.set	7754H		;SCI-B Control Register 2
SCIRXSTB	.set	7755H		;SCI-B Receive Status Register
SCIRXEMUB	.set	7756H		;SCI-B Receive Emulation Data Buffer Register
SCIRXBUFB	.set	7757H		;SCI-B Receive Data Buffer Register
SCITXBUFB	.set	7759H		;SCI-B Transmit Data Buffer Register
SCIFFTXB	.set	775aH		;SCI-B FIFO Transmit Register
SCIFFRXB	.set	775bH		;SCI-B FIFO Receive Register
SCIFFCTB	.set	775cH		;SCI-B FIFO Control Register
SCIPRIB		.set	775fH		;SCI-B Priority Control Register

*F2812 MCBSP Register Summy
DRR2		.set	7800H		;McBSP Data Receive Register 2
DRR1		.set	7801H		;McBSP Data Receive Register 1
DXR2		.set	7802H		;McBSP Data Tranmit Register 2
DXR2		.set	7802H		;McBSP Data Tranmit Register 1

SPCR2		.set	7804H		;McBSP Serial Port Control Register 2
SPCR1		.set	7805H		;McBSP Serial Port Control Register 1
RCR2		.set	7806H		;McBSP Receive Control Register 2
RCR1		.set	7807H		;McBSP Receive Control Register 1
XCR2		.set	7808H		;McBSP Transmit Control Register 2
XCR1		.set	7809H		;McBSP Transmit Control Register 1
SRGR2		.set	780aH		;McBSP Sample Rate Generator Register 2
SRGR1		.set	780bH		;McBSP Sample Rate Generator Register 1

MCR2		.set	780cH		;McBSP Multichannel Register 2
MCR1		.set	780dH		;McBSP Multichannel Register 1
RCERA		.set	780eH		;McBSP Receive Channel Enable Register Partition A
RCERB		.set	780fH		;McBSP Receive Channel Enable Register Partition B
XCERA		.set	7810H		;McBSP Transmit Channel Enable Register Partition A
XCERB		.set	7811H		;McBSP Transmit Channel Enable Register Partition B
PCR1		.set	7812H		;McBSP Pin Control Register
RCERC		.set	7813H		;McBSP Receive Channel Enable Register Partition C
RCERD		.set	7814H		;McBSP Receive Channel Enable Register Partition D
XCERC		.set	7815H		;McBSP Transmit Channel Enable Register Partition C
XCERD		.set	7816H		;McBSP Transmit Channel Enable Register Partition D
RCERE		.set	7817H		;McBSP Receive Channel Enable Register Partition E
RCERF		.set	7818H		;McBSP Receive Channel Enable Register Partition F
XCERE		.set	7819H		;McBSP Transmit Channel Enable Register Partition E
XCERF		.set	781AH		;McBSP Transmit Channel Enable Register Partition F
RCERG		.set	781BH		;McBSP Receive Channel Enable Register Partition G
RCERH		.set	781CH		;McBSP Receive Channel Enable Register Partition H
XCERG		.set	781DH		;McBSP Transmit Channel Enable Register Partition G
XCERH		.set	781EH		;McBSP Transmit Channel Enable Register Partition H

MFFTX		.set	7820H		;McBSP Transmit FIFO Register
MFFRX		.set	7821H		;McBSP Receive FIFO Register
MFFCT		.set	7822H		;McBSP FIFO Control Register
MFFINT		.set	7823H		;McBSP FIFO Interrupt Register
MFFST		.set	7824H		;McBSP FIFO Status Register

*F2812 
