
;CodeVisionAVR C Compiler V3.12 Advanced
;(C) Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Build configuration    : Release
;Chip type              : ATmega32A
;Program type           : Application
;Clock frequency        : 8.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : float, width, precision
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 512 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: No
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#define _MODEL_SMALL_

	#pragma AVRPART ADMIN PART_NAME ATmega32A
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2048
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x085F
	.EQU __DSTACK_SIZE=0x0200
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	PUSH R26
	PUSH R27
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	POP  R27
	POP  R26
	ICALL
	.ENDM

	.MACRO __CALL2EX
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	CALL __EEPROMRDD
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z
	MOVW R30,R0
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _timer2_counter=R4
	.DEF _timer2_counter_msb=R5
	.DEF _pw=R6
	.DEF _pw_msb=R7
	.DEF _rx_wr_index=R9
	.DEF _rx_rd_index=R8
	.DEF _rx_counter=R11
	.DEF __lcd_x=R10
	.DEF __lcd_y=R13
	.DEF __lcd_maxx=R12

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _timer2_ovf_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _usart_rx_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

;REGISTER BIT VARIABLES INITIALIZATION
__REG_BIT_VARS:
	.DW  0x0000

;GLOBAL REGISTER VARIABLES INITIALIZATION
__REG_VARS:
	.DB  0x4B,0x0,0x0,0x0
	.DB  0x0,0x0

_0x0:
	.DB  0x44,0x6F,0x63,0x74,0x6F,0x72,0x20,0x6E
	.DB  0x61,0x64,0x65,0x72,0x69,0x0,0x25,0x73
	.DB  0x2C,0x25,0x73,0x2C,0x25,0x73,0x0,0x20
	.DB  0x25,0x34,0x2E,0x31,0x66,0x67,0x20,0x25
	.DB  0x34,0x2E,0x31,0x66,0x67,0x20,0x25,0x34
	.DB  0x2E,0x31,0x66,0x67,0x0
_0x2000003:
	.DB  0x80,0xC0
_0x2020000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0
_0x2040060:
	.DB  0x1
_0x2040000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0,0x49,0x4E,0x46
	.DB  0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  0x02
	.DW  __REG_BIT_VARS*2

	.DW  0x06
	.DW  0x06
	.DW  __REG_VARS*2

	.DW  0x02
	.DW  __base_y_G100
	.DW  _0x2000003*2

	.DW  0x01
	.DW  __seed_G102
	.DW  _0x2040060*2

_0xFFFFFFFF:
	.DW  0

#define __GLOBAL_INI_TBL_PRESENT 1

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,__SRAM_START
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x260

	.CSEG
;/*********************************
;
;   5KG   LOADCELL Driver  with HX711
;
;**********************************/
;
;#include <mega32a.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;#include <alcd.h>
;#include <stdio.h>
;#include <stdlib.h>
;
;// Declare your global variables here
;#define HX711_DOUT    PIND.3
;#define HX711_SCK     PORTD.2
;#define HX_COMPLEMENT 0x800000
;#define HX_REF        0.360598//360.91549
;//0.360598
;#define HX711_DOUT1    PIND.5
;#define HX711_SCK1     PORTD.4
;#define HX_REF1        0.360598//9.33
;
;#define HX711_DOUT2    PINB.1
;#define HX711_SCK2     PORTB.0
;#define HX_REF2        0.360598//9.33
;
;void sort_array(float a[], int size);
;unsigned long HX711_Read(void);
;void get_scale(void);
;void get_offset(void);
;unsigned long HX711_Read1(void);
;void get_scale1(void);
;void get_offset1(void);
;unsigned long HX711_Read2(void);
;void get_scale2(void);
;void get_offset2(void);
;//void motor(void);
;//void delay_msec(long int ha);
;unsigned long Weight_Shiwu = 0,Weight_Maopi =0,Weight_Shiwu1 = 0,Weight_Maopi1 =0,Weight_Shiwu2 = 0,Weight_Maopi2 =0;
;unsigned long HX711_Buffer = 0,HX711_Buffer1 = 0,HX711_Buffer2 = 0;
;float         scale = 0,scale1 = 0,scale2 = 0,scale_sum = 0,scale1_sum = 0,scale2_sum = 0,scale_array[11],scale1_array[1 ...
;unsigned int timer2_counter,pw=75;//i,i2,i3,sw=0,i2;
;char  buffer1[12];
;char  buffer2[12];
;char  buffer3[12];
;char  buffer[30];
;char  buffer_test[32];
;char  str1[10];
;char  str2[10];
;char  str3[10];
;
;interrupt [TIM2_OVF] void timer2_ovf_isr(void)
; 0000 0035 {

	.CSEG
_timer2_ovf_isr:
; .FSTART _timer2_ovf_isr
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0036 // Reinitialize Timer2 value
; 0000 0037 TCNT2=0xB2;
	LDI  R30,LOW(178)
	OUT  0x24,R30
; 0000 0038 // Place your code here
; 0000 0039 if(timer2_counter == pw ) PORTC.1 = 0;
	__CPWRR 6,7,4,5
	BRNE _0x3
	CBI  0x15,1
; 0000 003A timer2_counter += 1;
_0x3:
	MOVW R30,R4
	ADIW R30,1
	MOVW R4,R30
; 0000 003B if(timer2_counter >= 100){ timer2_counter = 0; PORTC.1 = 1;}
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	CP   R4,R30
	CPC  R5,R31
	BRLO _0x6
	CLR  R4
	CLR  R5
	SBI  0x15,1
; 0000 003C }
_0x6:
	RJMP _0x5D
; .FEND
;
;void send( unsigned char Data1[12], unsigned char Data2[12], unsigned char Data3[12]);
;unsigned char buffer1[12];
;#define DATA_REGISTER_EMPTY (1<<UDRE)
;#define RX_COMPLETE (1<<RXC)
;#define FRAMING_ERROR (1<<FE)
;#define PARITY_ERROR (1<<UPE)
;#define DATA_OVERRUN (1<<DOR)
;
;// USART Receiver buffer
;#define RX_BUFFER_SIZE 24
;char rx_buffer[RX_BUFFER_SIZE];
;
;#if RX_BUFFER_SIZE <= 256
;unsigned char rx_wr_index=0,rx_rd_index=0;
;#else
;unsigned int rx_wr_index=0,rx_rd_index=0;
;#endif
;
;#if RX_BUFFER_SIZE < 256
;unsigned char rx_counter=0;
;#else
;unsigned int rx_counter=0;
;#endif
;
;// This flag is set on USART Receiver buffer overflow
;bit rx_buffer_overflow;
;
;// USART Receiver interrupt service routine
;interrupt [USART_RXC] void usart_rx_isr(void)
; 0000 005B {
_usart_rx_isr:
; .FSTART _usart_rx_isr
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 005C char status,data;
; 0000 005D status=UCSRA;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	IN   R17,11
; 0000 005E data=UDR;
	IN   R16,12
; 0000 005F if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x9
; 0000 0060    {
; 0000 0061    rx_buffer[rx_wr_index++]=data;
	MOV  R30,R9
	INC  R9
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer)
	SBCI R31,HIGH(-_rx_buffer)
	ST   Z,R16
; 0000 0062 #if RX_BUFFER_SIZE == 256
; 0000 0063    // special case for receiver buffer size=256
; 0000 0064    if (++rx_counter == 0) rx_buffer_overflow=1;
; 0000 0065 #else
; 0000 0066    if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
	LDI  R30,LOW(24)
	CP   R30,R9
	BRNE _0xA
	CLR  R9
; 0000 0067    if (++rx_counter == RX_BUFFER_SIZE)
_0xA:
	INC  R11
	LDI  R30,LOW(24)
	CP   R30,R11
	BRNE _0xB
; 0000 0068       {
; 0000 0069       rx_counter=0;
	CLR  R11
; 0000 006A       rx_buffer_overflow=1;
	SET
	BLD  R2,0
; 0000 006B       }
; 0000 006C #endif
; 0000 006D    }
_0xB:
; 0000 006E }
_0x9:
	LD   R16,Y+
	LD   R17,Y+
_0x5D:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
; .FEND
;
;#ifndef _DEBUG_TERMINAL_IO_
;// Get a character from the USART Receiver buffer
;#define _ALTERNATE_GETCHAR_
;#pragma used+
;char getchar(void)
; 0000 0075 {
; 0000 0076 char data;
; 0000 0077 while (rx_counter==0);
;	data -> R17
; 0000 0078 data=rx_buffer[rx_rd_index++];
; 0000 0079 #if RX_BUFFER_SIZE != 256
; 0000 007A if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
; 0000 007B #endif
; 0000 007C #asm("cli")
; 0000 007D --rx_counter;
; 0000 007E #asm("sei")
; 0000 007F return data;
; 0000 0080 }
;#pragma used-
;#endif
;void main(void)
; 0000 0084 {
_main:
; .FSTART _main
; 0000 0085 
; 0000 0086 PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 0087 DDRA=0x00;
	OUT  0x1A,R30
; 0000 0088 
; 0000 0089 //DDRD = (1<<DDD0);
; 0000 008A ///////////////////////
; 0000 008B // Port B initialization
; 0000 008C // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out
; 0000 008D DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
	LDI  R30,LOW(1)
	OUT  0x17,R30
; 0000 008E // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=0
; 0000 008F PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
	LDI  R30,LOW(0)
	OUT  0x18,R30
; 0000 0090 
; 0000 0091 // Port C initialization
; 0000 0092 // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=Out Bit0=Out
; 0000 0093 DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (1<<DDC1) | (1<<DDC0);
	LDI  R30,LOW(3)
	OUT  0x14,R30
; 0000 0094 // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=0 Bit0=0
; 0000 0095 PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
	LDI  R30,LOW(0)
	OUT  0x15,R30
; 0000 0096 
; 0000 0097 // Port D initialization
; 0000 0098 // Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=In Bit2=Out Bit1=Out Bit0=In
; 0000 0099 DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (1<<DDD2) | (1<<DDD1) | (0<<DDD0);
	LDI  R30,LOW(22)
	OUT  0x11,R30
; 0000 009A // State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=T Bit2=0 Bit1=0 Bit0=T
; 0000 009B PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
	LDI  R30,LOW(0)
	OUT  0x12,R30
; 0000 009C ///////////////////////////
; 0000 009D // USART initialization
; 0000 009E // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 009F // USART Receiver: On
; 0000 00A0 // USART Transmitter: On
; 0000 00A1 // USART Mode: Asynchronous
; 0000 00A2 // USART Baud Rate: 9600
; 0000 00A3 UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
	OUT  0xB,R30
; 0000 00A4 UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
	LDI  R30,LOW(152)
	OUT  0xA,R30
; 0000 00A5 UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
	LDI  R30,LOW(134)
	OUT  0x20,R30
; 0000 00A6 UBRRH=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 00A7 UBRRL=0x33;
	LDI  R30,LOW(51)
	OUT  0x9,R30
; 0000 00A8 
; 0000 00A9 get_offset();
	RCALL _get_offset
; 0000 00AA get_offset1();
	RCALL _get_offset1
; 0000 00AB get_offset2();
	RCALL _get_offset2
; 0000 00AC 
; 0000 00AD 
; 0000 00AE lcd_init(16);
	LDI  R26,LOW(16)
	CALL _lcd_init
; 0000 00AF lcd_putsf("Doctor naderi");
	__POINTW2FN _0x0,0
	CALL _lcd_putsf
; 0000 00B0 delay_ms(1000);
	LDI  R26,LOW(1000)
	LDI  R27,HIGH(1000)
	CALL _delay_ms
; 0000 00B1 
; 0000 00B2 // Timer/Counter 2 initialization
; 0000 00B3 // Clock source: System Clock
; 0000 00B4 // Clock value: 7.813 kHz
; 0000 00B5 // Mode: Normal top=0xFF
; 0000 00B6 // OC2 output: Disconnected
; 0000 00B7 // Timer Period: 9.984 ms
; 0000 00B8 ASSR=0<<AS2;
	LDI  R30,LOW(0)
	OUT  0x22,R30
; 0000 00B9 TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
	LDI  R30,LOW(7)
	OUT  0x25,R30
; 0000 00BA TCNT2=0xB2;
	LDI  R30,LOW(178)
	OUT  0x24,R30
; 0000 00BB OCR2=0x00;
	LDI  R30,LOW(0)
	OUT  0x23,R30
; 0000 00BC 
; 0000 00BD // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 00BE TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
	LDI  R30,LOW(64)
	OUT  0x39,R30
; 0000 00BF 
; 0000 00C0 // Global enable interrupts
; 0000 00C1 #asm("sei")
	sei
; 0000 00C2 
; 0000 00C3 while (1)
_0x10:
; 0000 00C4       {
; 0000 00C5         int i;
; 0000 00C6 
; 0000 00C7         get_scale();
	SBIW R28,2
;	i -> Y+0
	RCALL _get_scale
; 0000 00C8         get_scale1();
	RCALL _get_scale1
; 0000 00C9         get_scale2();
	RCALL _get_scale2
; 0000 00CA         ftoa(scale,2,str1);
	CALL SUBOPT_0x0
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R26,LOW(_str1)
	LDI  R27,HIGH(_str1)
	CALL _ftoa
; 0000 00CB         //sprintf(buffer1,"%s",str1);
; 0000 00CC         ftoa(scale1,2,str2);
	CALL SUBOPT_0x1
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R26,LOW(_str2)
	LDI  R27,HIGH(_str2)
	CALL _ftoa
; 0000 00CD         //sprintf(buffer2,"%s",str2);
; 0000 00CE         ftoa(scale2,2,str3);
	CALL SUBOPT_0x2
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R26,LOW(_str3)
	LDI  R27,HIGH(_str3)
	CALL _ftoa
; 0000 00CF         sprintf(buffer_test,"%s,%s,%s",str1,str2,str3);
	LDI  R30,LOW(_buffer_test)
	LDI  R31,HIGH(_buffer_test)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0x0,14
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_str1)
	LDI  R31,HIGH(_str1)
	CALL SUBOPT_0x3
	LDI  R30,LOW(_str2)
	LDI  R31,HIGH(_str2)
	CALL SUBOPT_0x3
	LDI  R30,LOW(_str3)
	LDI  R31,HIGH(_str3)
	CALL SUBOPT_0x3
	LDI  R24,12
	CALL _sprintf
	ADIW R28,16
; 0000 00D0         /*for(i=0;i<12;i++) buffer_test[i]=buffer1[i];
; 0000 00D1         buffer_test[12]=" " ;
; 0000 00D2         for(i=0;i<12;i++) buffer_test[i+13]=buffer2[i];
; 0000 00D3         buffer_test[25]=" " ;
; 0000 00D4         for(i=0;i<12;i++) buffer_test[i+26]=buffer3[i]; */
; 0000 00D5         puts(buffer_test);
	LDI  R26,LOW(_buffer_test)
	LDI  R27,HIGH(_buffer_test)
	CALL _puts
; 0000 00D6         //send(buffer1,buffer2,buffer3);
; 0000 00D7         sprintf(buffer," %4.1fg %4.1fg %4.1fg",scale,scale1,scale2);//sprintf(buffer," %ld ",Weight_Shiwu);//sprintf(buf ...
	LDI  R30,LOW(_buffer)
	LDI  R31,HIGH(_buffer)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0x0,23
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x0
	CALL SUBOPT_0x1
	CALL SUBOPT_0x2
	LDI  R24,12
	CALL _sprintf
	ADIW R28,16
; 0000 00D8         lcd_clear();
	CALL _lcd_clear
; 0000 00D9         lcd_gotoxy(0,0);
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDI  R26,LOW(0)
	CALL _lcd_gotoxy
; 0000 00DA         lcd_puts(buffer);
	LDI  R26,LOW(_buffer)
	LDI  R27,HIGH(_buffer)
	CALL _lcd_puts
; 0000 00DB //        get_scale1();
; 0000 00DC //        sprintf(buffer,"%4.1fg",scale1);//sprintf(buffer," %ld ",Weight_Shiwu1);//sprintf(buffer," %1.3f --> Gram",sca ...
; 0000 00DD //        lcd_gotoxy(0,8);
; 0000 00DE //        lcd_puts(buffer);
; 0000 00DF //        get_scale2();
; 0000 00E0 //        sprintf(buffer,"%4.1fg",scale2);//sprintf(buffer," %ld ",Weight_Shiwu2);//sprintf(buffer," %1.3f --> Gram",sca ...
; 0000 00E1 //        lcd_gotoxy(1,0);
; 0000 00E2 //        lcd_puts(buffer);
; 0000 00E3         delay_ms(10);
	LDI  R26,LOW(10)
	LDI  R27,0
	CALL _delay_ms
; 0000 00E4         //motor();
; 0000 00E5       }
	ADIW R28,2
	RJMP _0x10
; 0000 00E6 }
_0x13:
	RJMP _0x13
; .FEND
;
;
;unsigned long HX711_Read(void)
; 0000 00EA {
_HX711_Read:
; .FSTART _HX711_Read
; 0000 00EB     unsigned long count;
; 0000 00EC     unsigned char i;
; 0000 00ED 
; 0000 00EE       HX711_DOUT=1;
	SBIW R28,4
	ST   -Y,R17
;	count -> Y+1
;	i -> R17
	SBI  0x10,3
; 0000 00EF       HX711_SCK=0;
	CBI  0x12,2
; 0000 00F0       count=0;
	CALL SUBOPT_0x4
; 0000 00F1       while(HX711_DOUT);
_0x18:
	SBIC 0x10,3
	RJMP _0x18
; 0000 00F2       for(i=0;i<24;i++)  //gain --> 128
	LDI  R17,LOW(0)
_0x1C:
	CPI  R17,24
	BRSH _0x1D
; 0000 00F3         {
; 0000 00F4             HX711_SCK=1;
	SBI  0x12,2
; 0000 00F5             count=count<<1;
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
; 0000 00F6             HX711_SCK=0;
	CBI  0x12,2
; 0000 00F7             if(HX711_DOUT)
	SBIS 0x10,3
	RJMP _0x22
; 0000 00F8                 count++;
	CALL SUBOPT_0x7
; 0000 00F9         }
_0x22:
	SUBI R17,-1
	RJMP _0x1C
_0x1D:
; 0000 00FA        HX711_SCK=1;
	SBI  0x12,2
; 0000 00FB       count=count ^ HX_COMPLEMENT;//2,s Complement
	CALL SUBOPT_0x8
; 0000 00FC       HX711_SCK=0;
	CBI  0x12,2
; 0000 00FD       return(count);
	CALL SUBOPT_0x5
	LDD  R17,Y+0
	JMP  _0x20C0006
; 0000 00FE }//HX711_Read
; .FEND
;unsigned long HX711_Read1(void)
; 0000 0100 {
_HX711_Read1:
; .FSTART _HX711_Read1
; 0000 0101     unsigned long count;
; 0000 0102     unsigned char i;
; 0000 0103 
; 0000 0104       HX711_DOUT1=1;
	SBIW R28,4
	ST   -Y,R17
;	count -> Y+1
;	i -> R17
	SBI  0x10,5
; 0000 0105       HX711_SCK1=0;
	CBI  0x12,4
; 0000 0106       count=0;
	CALL SUBOPT_0x4
; 0000 0107       while(HX711_DOUT1);
_0x2B:
	SBIC 0x10,5
	RJMP _0x2B
; 0000 0108       for(i=0;i<24;i++)  //gain --> 128
	LDI  R17,LOW(0)
_0x2F:
	CPI  R17,24
	BRSH _0x30
; 0000 0109         {
; 0000 010A             HX711_SCK1=1;
	SBI  0x12,4
; 0000 010B             count=count<<1;
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
; 0000 010C             HX711_SCK1=0;
	CBI  0x12,4
; 0000 010D             if(HX711_DOUT1)
	SBIS 0x10,5
	RJMP _0x35
; 0000 010E                 count++;
	CALL SUBOPT_0x7
; 0000 010F         }
_0x35:
	SUBI R17,-1
	RJMP _0x2F
_0x30:
; 0000 0110        HX711_SCK1=1;
	SBI  0x12,4
; 0000 0111       count=count ^ HX_COMPLEMENT;//2,s Complement
	CALL SUBOPT_0x8
; 0000 0112       HX711_SCK1=0;
	CBI  0x12,4
; 0000 0113       return(count);
	CALL SUBOPT_0x5
	LDD  R17,Y+0
	JMP  _0x20C0006
; 0000 0114 }//HX711_Read
; .FEND
;unsigned long HX711_Read2(void)
; 0000 0116 {
_HX711_Read2:
; .FSTART _HX711_Read2
; 0000 0117     unsigned long count;
; 0000 0118     unsigned char i;
; 0000 0119 
; 0000 011A       HX711_DOUT2=1;
	SBIW R28,4
	ST   -Y,R17
;	count -> Y+1
;	i -> R17
	SBI  0x16,1
; 0000 011B       HX711_SCK2=0;
	CBI  0x18,0
; 0000 011C       count=0;
	CALL SUBOPT_0x4
; 0000 011D       while(HX711_DOUT2);
_0x3E:
	SBIC 0x16,1
	RJMP _0x3E
; 0000 011E       for(i=0;i<24;i++)  //gain --> 128
	LDI  R17,LOW(0)
_0x42:
	CPI  R17,24
	BRSH _0x43
; 0000 011F         {
; 0000 0120             HX711_SCK2=1;
	SBI  0x18,0
; 0000 0121             count=count<<1;
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
; 0000 0122             HX711_SCK2=0;
	CBI  0x18,0
; 0000 0123             if(HX711_DOUT2)
	SBIS 0x16,1
	RJMP _0x48
; 0000 0124                 count++;
	CALL SUBOPT_0x7
; 0000 0125         }
_0x48:
	SUBI R17,-1
	RJMP _0x42
_0x43:
; 0000 0126        HX711_SCK2=1;
	SBI  0x18,0
; 0000 0127       count=count ^ HX_COMPLEMENT;//2,s Complement
	CALL SUBOPT_0x8
; 0000 0128       HX711_SCK2=0;
	CBI  0x18,0
; 0000 0129       return(count);
	CALL SUBOPT_0x5
	LDD  R17,Y+0
	JMP  _0x20C0006
; 0000 012A }//HX711_Read
; .FEND
;
;void get_scale(void)
; 0000 012D {
_get_scale:
; .FSTART _get_scale
; 0000 012E     unsigned int i;
; 0000 012F     unsigned int j;
; 0000 0130     scale_sum = 0;
	CALL __SAVELOCR4
;	i -> R16,R17
;	j -> R18,R19
	LDI  R30,LOW(0)
	STS  _scale_sum,R30
	STS  _scale_sum+1,R30
	STS  _scale_sum+2,R30
	STS  _scale_sum+3,R30
; 0000 0131 
; 0000 0132     //for(j=0; j<3 ;j++){
; 0000 0133     //for(i=0 ; i<11 ;i++){
; 0000 0134     HX711_Buffer = 0;
	CALL SUBOPT_0x9
; 0000 0135 
; 0000 0136     HX711_Buffer = HX711_Read();
	RCALL _HX711_Read
	CALL SUBOPT_0xA
; 0000 0137     HX711_Buffer = HX711_Buffer/100;
	CALL SUBOPT_0xB
	CALL SUBOPT_0xC
	CALL SUBOPT_0xA
; 0000 0138     //if(HX711_Buffer > Weight_Maopi){
; 0000 0139         Weight_Shiwu = HX711_Buffer;
	LDS  R30,_HX711_Buffer
	LDS  R31,_HX711_Buffer+1
	LDS  R22,_HX711_Buffer+2
	LDS  R23,_HX711_Buffer+3
	CALL SUBOPT_0xD
; 0000 013A         Weight_Shiwu = Weight_Shiwu - Weight_Maopi;
	LDS  R26,_Weight_Maopi
	LDS  R27,_Weight_Maopi+1
	LDS  R24,_Weight_Maopi+2
	LDS  R25,_Weight_Maopi+3
	CALL SUBOPT_0xE
	CALL __SUBD12
	CALL SUBOPT_0xD
; 0000 013B 
; 0000 013C         Weight_Shiwu = ((float)Weight_Shiwu/HX_REF);
	CALL SUBOPT_0xF
	CALL SUBOPT_0x10
	LDI  R26,LOW(_Weight_Shiwu)
	LDI  R27,HIGH(_Weight_Shiwu)
	CALL SUBOPT_0x11
; 0000 013D 
; 0000 013E         scale = (float)Weight_Shiwu/30;
	CALL SUBOPT_0xF
	CALL SUBOPT_0x12
	STS  _scale,R30
	STS  _scale+1,R31
	STS  _scale+2,R22
	STS  _scale+3,R23
; 0000 013F     //}
; 0000 0140     /*
; 0000 0141     sort_array(scale_array,11);
; 0000 0142     scale_sum += scale_array[5];
; 0000 0143     }
; 0000 0144     scale = scale_sum/3;*/
; 0000 0145 }
	RJMP _0x20C000A
; .FEND
;void get_scale1(void)
; 0000 0147 {
_get_scale1:
; .FSTART _get_scale1
; 0000 0148     unsigned int i;
; 0000 0149     unsigned int j;
; 0000 014A     scale1_sum = 0;
	CALL __SAVELOCR4
;	i -> R16,R17
;	j -> R18,R19
	LDI  R30,LOW(0)
	STS  _scale1_sum,R30
	STS  _scale1_sum+1,R30
	STS  _scale1_sum+2,R30
	STS  _scale1_sum+3,R30
; 0000 014B 
; 0000 014C     //for(j=0; j<3 ;j++){
; 0000 014D     //for(i=0 ; i<11 ;i++){
; 0000 014E     HX711_Buffer1 = 0;
	CALL SUBOPT_0x13
; 0000 014F 
; 0000 0150     HX711_Buffer1 = HX711_Read1();
	RCALL _HX711_Read1
	CALL SUBOPT_0x14
; 0000 0151     HX711_Buffer1 = HX711_Buffer1/100;
	CALL SUBOPT_0x15
	CALL SUBOPT_0xC
	CALL SUBOPT_0x14
; 0000 0152     //if(HX711_Buffer1 > Weight_Maopi1)
; 0000 0153     //{
; 0000 0154         Weight_Shiwu1 = HX711_Buffer1;
	LDS  R30,_HX711_Buffer1
	LDS  R31,_HX711_Buffer1+1
	LDS  R22,_HX711_Buffer1+2
	LDS  R23,_HX711_Buffer1+3
	CALL SUBOPT_0x16
; 0000 0155         Weight_Shiwu1 = Weight_Shiwu1 - Weight_Maopi1;
	LDS  R26,_Weight_Maopi1
	LDS  R27,_Weight_Maopi1+1
	LDS  R24,_Weight_Maopi1+2
	LDS  R25,_Weight_Maopi1+3
	CALL SUBOPT_0x17
	CALL __SUBD12
	CALL SUBOPT_0x16
; 0000 0156 
; 0000 0157         Weight_Shiwu1 = ((float)Weight_Shiwu1/HX_REF1);
	CALL SUBOPT_0x18
	CALL SUBOPT_0x10
	LDI  R26,LOW(_Weight_Shiwu1)
	LDI  R27,HIGH(_Weight_Shiwu1)
	CALL SUBOPT_0x11
; 0000 0158 
; 0000 0159         scale1 = (float)Weight_Shiwu1 / 30;
	CALL SUBOPT_0x18
	CALL SUBOPT_0x12
	STS  _scale1,R30
	STS  _scale1+1,R31
	STS  _scale1+2,R22
	STS  _scale1+3,R23
; 0000 015A     //}
; 0000 015B     /*
; 0000 015C     else scale1_array[i] = (float)0;
; 0000 015D     }
; 0000 015E     sort_array(scale1_array,11);
; 0000 015F     scale1_sum += scale1_array[5];
; 0000 0160     }
; 0000 0161     scale1 = scale1_sum/3;  */
; 0000 0162 }
	RJMP _0x20C000A
; .FEND
;
;void get_scale2(void)
; 0000 0165 {
_get_scale2:
; .FSTART _get_scale2
; 0000 0166     unsigned int i;
; 0000 0167     unsigned int j;
; 0000 0168     scale2_sum = 0;
	CALL __SAVELOCR4
;	i -> R16,R17
;	j -> R18,R19
	LDI  R30,LOW(0)
	STS  _scale2_sum,R30
	STS  _scale2_sum+1,R30
	STS  _scale2_sum+2,R30
	STS  _scale2_sum+3,R30
; 0000 0169 
; 0000 016A     //for(j=0; j<3 ;j++){
; 0000 016B     //for(i=0 ; i<11 ;i++){
; 0000 016C     HX711_Buffer2 = 0;
	CALL SUBOPT_0x19
; 0000 016D 
; 0000 016E     HX711_Buffer2 = HX711_Read2();
	RCALL _HX711_Read2
	CALL SUBOPT_0x1A
; 0000 016F     HX711_Buffer2 = HX711_Buffer2/100;
	CALL SUBOPT_0x1B
	CALL SUBOPT_0xC
	CALL SUBOPT_0x1A
; 0000 0170     //if(HX711_Buffer2 > Weight_Maopi2)
; 0000 0171     //{
; 0000 0172         Weight_Shiwu2 = HX711_Buffer2;
	LDS  R30,_HX711_Buffer2
	LDS  R31,_HX711_Buffer2+1
	LDS  R22,_HX711_Buffer2+2
	LDS  R23,_HX711_Buffer2+3
	CALL SUBOPT_0x1C
; 0000 0173         Weight_Shiwu2 = Weight_Shiwu2 - Weight_Maopi2;
	LDS  R26,_Weight_Maopi2
	LDS  R27,_Weight_Maopi2+1
	LDS  R24,_Weight_Maopi2+2
	LDS  R25,_Weight_Maopi2+3
	CALL SUBOPT_0x1D
	CALL __SUBD12
	CALL SUBOPT_0x1C
; 0000 0174 
; 0000 0175         Weight_Shiwu2 = ((float)Weight_Shiwu2/HX_REF2);
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x10
	LDI  R26,LOW(_Weight_Shiwu2)
	LDI  R27,HIGH(_Weight_Shiwu2)
	CALL SUBOPT_0x11
; 0000 0176 
; 0000 0177         scale2 = (float)Weight_Shiwu2 / 30;
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x12
	STS  _scale2,R30
	STS  _scale2+1,R31
	STS  _scale2+2,R22
	STS  _scale2+3,R23
; 0000 0178     //}
; 0000 0179     /*else scale2_array[i] = (float)0;
; 0000 017A     }
; 0000 017B     sort_array(scale2_array,11);
; 0000 017C     scale2_sum += scale2_array[5];
; 0000 017D     }
; 0000 017E     scale2 = scale2_sum/3;*/
; 0000 017F }
_0x20C000A:
	CALL __LOADLOCR4
	ADIW R28,4
	RET
; .FEND
;
;
;void get_offset(void)
; 0000 0183 {
_get_offset:
; .FSTART _get_offset
; 0000 0184     int i=0;
; 0000 0185 
; 0000 0186     HX711_Buffer = 0;
	CALL SUBOPT_0x1F
;	i -> R16,R17
	CALL SUBOPT_0x9
; 0000 0187 
; 0000 0188    	for( i = 0 ; i < 10 ; i++)
	__GETWRN 16,17,0
_0x4E:
	__CPWRN 16,17,10
	BRGE _0x4F
; 0000 0189 	{
; 0000 018A 		HX711_Buffer = HX711_Buffer + HX711_Read();
	RCALL _HX711_Read
	CALL SUBOPT_0xB
	CALL __ADDD12
	CALL SUBOPT_0xA
; 0000 018B         delay_us(500);
	CALL SUBOPT_0x20
; 0000 018C 	}
	__ADDWRN 16,17,1
	RJMP _0x4E
_0x4F:
; 0000 018D     HX711_Buffer = HX711_Buffer /10;
	CALL SUBOPT_0xB
	CALL SUBOPT_0x21
	CALL SUBOPT_0xA
; 0000 018E     Weight_Maopi = HX711_Buffer/100;
	CALL SUBOPT_0xB
	CALL SUBOPT_0xC
	STS  _Weight_Maopi,R30
	STS  _Weight_Maopi+1,R31
	STS  _Weight_Maopi+2,R22
	STS  _Weight_Maopi+3,R23
; 0000 018F }//get_offset
	RJMP _0x20C0009
; .FEND
;void get_offset1(void)
; 0000 0191 {
_get_offset1:
; .FSTART _get_offset1
; 0000 0192     int i=0;
; 0000 0193 
; 0000 0194     HX711_Buffer1 = 0;
	CALL SUBOPT_0x1F
;	i -> R16,R17
	CALL SUBOPT_0x13
; 0000 0195 
; 0000 0196    	for( i = 0 ; i < 10 ; i++)
	__GETWRN 16,17,0
_0x51:
	__CPWRN 16,17,10
	BRGE _0x52
; 0000 0197 	{
; 0000 0198 		HX711_Buffer1 = HX711_Buffer1 + HX711_Read1();
	RCALL _HX711_Read1
	CALL SUBOPT_0x15
	CALL __ADDD12
	CALL SUBOPT_0x14
; 0000 0199         delay_us(500);
	CALL SUBOPT_0x20
; 0000 019A 	}
	__ADDWRN 16,17,1
	RJMP _0x51
_0x52:
; 0000 019B     HX711_Buffer1 = HX711_Buffer1 /10;
	CALL SUBOPT_0x15
	CALL SUBOPT_0x21
	CALL SUBOPT_0x14
; 0000 019C     Weight_Maopi1 = HX711_Buffer1/100;
	CALL SUBOPT_0x15
	CALL SUBOPT_0xC
	STS  _Weight_Maopi1,R30
	STS  _Weight_Maopi1+1,R31
	STS  _Weight_Maopi1+2,R22
	STS  _Weight_Maopi1+3,R23
; 0000 019D }//get_offset
	RJMP _0x20C0009
; .FEND
;void get_offset2(void)
; 0000 019F {
_get_offset2:
; .FSTART _get_offset2
; 0000 01A0     int i=0;
; 0000 01A1 
; 0000 01A2     HX711_Buffer2 = 0;
	CALL SUBOPT_0x1F
;	i -> R16,R17
	CALL SUBOPT_0x19
; 0000 01A3 
; 0000 01A4    	for( i = 0 ; i < 10 ; i++)
	__GETWRN 16,17,0
_0x54:
	__CPWRN 16,17,10
	BRGE _0x55
; 0000 01A5 	{
; 0000 01A6 		HX711_Buffer2 = HX711_Buffer2 + HX711_Read2();
	RCALL _HX711_Read2
	CALL SUBOPT_0x1B
	CALL __ADDD12
	CALL SUBOPT_0x1A
; 0000 01A7         delay_us(500);
	CALL SUBOPT_0x20
; 0000 01A8 	}
	__ADDWRN 16,17,1
	RJMP _0x54
_0x55:
; 0000 01A9     HX711_Buffer2 = HX711_Buffer2 /10;
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x21
	CALL SUBOPT_0x1A
; 0000 01AA     Weight_Maopi2 = HX711_Buffer2/100;
	CALL SUBOPT_0x1B
	CALL SUBOPT_0xC
	STS  _Weight_Maopi2,R30
	STS  _Weight_Maopi2+1,R31
	STS  _Weight_Maopi2+2,R22
	STS  _Weight_Maopi2+3,R23
; 0000 01AB }//get_offset
_0x20C0009:
	LD   R16,Y+
	LD   R17,Y+
	RET
; .FEND
;
;/*void delay_msec(long int ha)
;{
;for(i2=0;i2<=ha;i2++)
;{
;delay_ms(1);
;}
;}
;void motor(void)
;{
;//----------------------right
;PORTC.0=0;
;PORTC.1=1;
;delay_msec(pw);
;PORTC.1=0;
;delay_msec(sw);
;} */
;/*
;void send( unsigned char Data1[12], unsigned char Data2[12], unsigned char Data3[12])//,unsigned char Data2[12] )
;{
;//char ATN=0X44;
;
;puts(Data1);
;putchar(',');
;puts(Data2);
;putchar(',');
;puts(Data3);
;//puts(Data2);
;//putchar('M');
;}
;*/
;void sort_array(float a[], int size) {
; 0000 01CB void sort_array(float a[], int size) {
; 0000 01CC     int i=0;
; 0000 01CD     int o=0;
; 0000 01CE     for(i=0; i<(size-1); i++) {
;	a -> Y+6
;	size -> Y+4
;	i -> R16,R17
;	o -> R18,R19
; 0000 01CF         for(o=0; o<(size-(i+1)); o++) {
; 0000 01D0                 if(a[o] > a[o+1]) {
; 0000 01D1                     int t = a[o];
; 0000 01D2                     a[o] = a[o+1];
;	a -> Y+8
;	size -> Y+6
;	t -> Y+0
; 0000 01D3                     a[o+1] = t;
; 0000 01D4                 }
; 0000 01D5         }
; 0000 01D6     }
; 0000 01D7 }
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

	.DSEG

	.CSEG
__lcd_write_nibble_G100:
; .FSTART __lcd_write_nibble_G100
	ST   -Y,R26
	IN   R30,0x1B
	ANDI R30,LOW(0xF)
	MOV  R26,R30
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	OR   R30,R26
	OUT  0x1B,R30
	__DELAY_USB 13
	SBI  0x1B,2
	__DELAY_USB 13
	CBI  0x1B,2
	__DELAY_USB 13
	JMP  _0x20C0008
; .FEND
__lcd_write_data:
; .FSTART __lcd_write_data
	ST   -Y,R26
	LD   R26,Y
	RCALL __lcd_write_nibble_G100
    ld    r30,y
    swap  r30
    st    y,r30
	LD   R26,Y
	RCALL __lcd_write_nibble_G100
	__DELAY_USB 133
	RJMP _0x20C0008
; .FEND
_lcd_gotoxy:
; .FSTART _lcd_gotoxy
	ST   -Y,R26
	LD   R30,Y
	LDI  R31,0
	SUBI R30,LOW(-__base_y_G100)
	SBCI R31,HIGH(-__base_y_G100)
	LD   R30,Z
	LDD  R26,Y+1
	ADD  R26,R30
	RCALL __lcd_write_data
	LDD  R10,Y+1
	LDD  R13,Y+0
	ADIW R28,2
	RET
; .FEND
_lcd_clear:
; .FSTART _lcd_clear
	LDI  R26,LOW(2)
	CALL SUBOPT_0x22
	LDI  R26,LOW(12)
	RCALL __lcd_write_data
	LDI  R26,LOW(1)
	CALL SUBOPT_0x22
	LDI  R30,LOW(0)
	MOV  R13,R30
	MOV  R10,R30
	RET
; .FEND
_lcd_putchar:
; .FSTART _lcd_putchar
	ST   -Y,R26
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BREQ _0x2000005
	CP   R10,R12
	BRLO _0x2000004
_0x2000005:
	LDI  R30,LOW(0)
	ST   -Y,R30
	INC  R13
	MOV  R26,R13
	RCALL _lcd_gotoxy
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRNE _0x2000007
	RJMP _0x20C0008
_0x2000007:
_0x2000004:
	INC  R10
	SBI  0x1B,0
	LD   R26,Y
	RCALL __lcd_write_data
	CBI  0x1B,0
	RJMP _0x20C0008
; .FEND
_lcd_puts:
; .FSTART _lcd_puts
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
_0x2000008:
	CALL SUBOPT_0x23
	BREQ _0x200000A
	MOV  R26,R17
	RCALL _lcd_putchar
	RJMP _0x2000008
_0x200000A:
	RJMP _0x20C0007
; .FEND
_lcd_putsf:
; .FSTART _lcd_putsf
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
_0x200000B:
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,1
	STD  Y+1,R30
	STD  Y+1+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R17,R30
	CPI  R30,0
	BREQ _0x200000D
	MOV  R26,R17
	RCALL _lcd_putchar
	RJMP _0x200000B
_0x200000D:
	RJMP _0x20C0007
; .FEND
_lcd_init:
; .FSTART _lcd_init
	ST   -Y,R26
	IN   R30,0x1A
	ORI  R30,LOW(0xF0)
	OUT  0x1A,R30
	SBI  0x1A,2
	SBI  0x1A,0
	SBI  0x1A,1
	CBI  0x1B,2
	CBI  0x1B,0
	CBI  0x1B,1
	LDD  R12,Y+0
	LD   R30,Y
	SUBI R30,-LOW(128)
	__PUTB1MN __base_y_G100,2
	LD   R30,Y
	SUBI R30,-LOW(192)
	__PUTB1MN __base_y_G100,3
	LDI  R26,LOW(20)
	LDI  R27,0
	CALL _delay_ms
	CALL SUBOPT_0x24
	CALL SUBOPT_0x24
	CALL SUBOPT_0x24
	LDI  R26,LOW(32)
	RCALL __lcd_write_nibble_G100
	__DELAY_USW 200
	LDI  R26,LOW(40)
	RCALL __lcd_write_data
	LDI  R26,LOW(4)
	RCALL __lcd_write_data
	LDI  R26,LOW(133)
	RCALL __lcd_write_data
	LDI  R26,LOW(6)
	RCALL __lcd_write_data
	RCALL _lcd_clear
	RJMP _0x20C0008
; .FEND
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

	.CSEG
_putchar:
; .FSTART _putchar
	ST   -Y,R26
putchar0:
     sbis usr,udre
     rjmp putchar0
     ld   r30,y
     out  udr,r30
_0x20C0008:
	ADIW R28,1
	RET
; .FEND
_puts:
; .FSTART _puts
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
_0x2020003:
	CALL SUBOPT_0x23
	BREQ _0x2020005
	MOV  R26,R17
	RCALL _putchar
	RJMP _0x2020003
_0x2020005:
	LDI  R26,LOW(10)
	RCALL _putchar
_0x20C0007:
	LDD  R17,Y+0
	ADIW R28,3
	RET
; .FEND
_put_buff_G101:
; .FSTART _put_buff_G101
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x2020010
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,4
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x2020012
	__CPWRN 16,17,2
	BRLO _0x2020013
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	__PUTW1SNS 2,4
_0x2020012:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	CALL SUBOPT_0x25
	SBIW R30,1
	LDD  R26,Y+4
	STD  Z+0,R26
_0x2020013:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	TST  R31
	BRMI _0x2020014
	CALL SUBOPT_0x25
_0x2020014:
	RJMP _0x2020015
_0x2020010:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	ST   X+,R30
	ST   X,R31
_0x2020015:
	LDD  R17,Y+1
	LDD  R16,Y+0
_0x20C0006:
	ADIW R28,5
	RET
; .FEND
__ftoe_G101:
; .FSTART __ftoe_G101
	CALL SUBOPT_0x26
	LDI  R30,LOW(128)
	STD  Y+2,R30
	LDI  R30,LOW(63)
	STD  Y+3,R30
	CALL __SAVELOCR4
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	CPI  R30,LOW(0xFFFF)
	LDI  R26,HIGH(0xFFFF)
	CPC  R31,R26
	BRNE _0x2020019
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2FN _0x2020000,0
	CALL _strcpyf
	RJMP _0x20C0005
_0x2020019:
	CPI  R30,LOW(0x7FFF)
	LDI  R26,HIGH(0x7FFF)
	CPC  R31,R26
	BRNE _0x2020018
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2FN _0x2020000,1
	CALL _strcpyf
	RJMP _0x20C0005
_0x2020018:
	LDD  R26,Y+11
	CPI  R26,LOW(0x7)
	BRLO _0x202001B
	LDI  R30,LOW(6)
	STD  Y+11,R30
_0x202001B:
	LDD  R17,Y+11
_0x202001C:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x202001E
	CALL SUBOPT_0x27
	RJMP _0x202001C
_0x202001E:
	__GETD1S 12
	CALL __CPD10
	BRNE _0x202001F
	LDI  R19,LOW(0)
	CALL SUBOPT_0x27
	RJMP _0x2020020
_0x202001F:
	LDD  R19,Y+11
	CALL SUBOPT_0x28
	BREQ PC+2
	BRCC PC+2
	RJMP _0x2020021
	CALL SUBOPT_0x27
_0x2020022:
	CALL SUBOPT_0x28
	BRLO _0x2020024
	CALL SUBOPT_0x29
	CALL SUBOPT_0x2A
	RJMP _0x2020022
_0x2020024:
	RJMP _0x2020025
_0x2020021:
_0x2020026:
	CALL SUBOPT_0x28
	BRSH _0x2020028
	CALL SUBOPT_0x29
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x2C
	SUBI R19,LOW(1)
	RJMP _0x2020026
_0x2020028:
	CALL SUBOPT_0x27
_0x2020025:
	__GETD1S 12
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x28
	BRLO _0x2020029
	CALL SUBOPT_0x29
	CALL SUBOPT_0x2A
_0x2020029:
_0x2020020:
	LDI  R17,LOW(0)
_0x202002A:
	LDD  R30,Y+11
	CP   R30,R17
	BRLO _0x202002C
	__GETD2S 4
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	CALL _floor
	__PUTD1S 4
	CALL SUBOPT_0x29
	CALL __DIVF21
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x30
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	__GETD2S 4
	CALL __MULF12
	CALL SUBOPT_0x29
	CALL SUBOPT_0x31
	CALL SUBOPT_0x2C
	MOV  R30,R17
	SUBI R17,-1
	CPI  R30,0
	BRNE _0x202002A
	CALL SUBOPT_0x2F
	LDI  R30,LOW(46)
	ST   X,R30
	RJMP _0x202002A
_0x202002C:
	CALL SUBOPT_0x32
	SBIW R30,1
	LDD  R26,Y+10
	STD  Z+0,R26
	CPI  R19,0
	BRGE _0x202002E
	NEG  R19
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDI  R30,LOW(45)
	RJMP _0x2020113
_0x202002E:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDI  R30,LOW(43)
_0x2020113:
	ST   X,R30
	CALL SUBOPT_0x32
	CALL SUBOPT_0x32
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __DIVB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
	CALL SUBOPT_0x32
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __MODB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x20C0005:
	CALL __LOADLOCR4
	ADIW R28,16
	RET
; .FEND
__print_G101:
; .FSTART __print_G101
	ST   -Y,R27
	ST   -Y,R26
	SBIW R28,63
	SBIW R28,17
	CALL __SAVELOCR6
	LDI  R17,0
	__GETW1SX 88
	STD  Y+8,R30
	STD  Y+8+1,R31
	__GETW1SX 86
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   X+,R30
	ST   X,R31
_0x2020030:
	MOVW R26,R28
	SUBI R26,LOW(-(92))
	SBCI R27,HIGH(-(92))
	CALL SUBOPT_0x25
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+2
	RJMP _0x2020032
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x2020036
	CPI  R18,37
	BRNE _0x2020037
	LDI  R17,LOW(1)
	RJMP _0x2020038
_0x2020037:
	CALL SUBOPT_0x33
_0x2020038:
	RJMP _0x2020035
_0x2020036:
	CPI  R30,LOW(0x1)
	BRNE _0x2020039
	CPI  R18,37
	BRNE _0x202003A
	CALL SUBOPT_0x33
	RJMP _0x2020114
_0x202003A:
	LDI  R17,LOW(2)
	LDI  R30,LOW(0)
	STD  Y+21,R30
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x202003B
	LDI  R16,LOW(1)
	RJMP _0x2020035
_0x202003B:
	CPI  R18,43
	BRNE _0x202003C
	LDI  R30,LOW(43)
	STD  Y+21,R30
	RJMP _0x2020035
_0x202003C:
	CPI  R18,32
	BRNE _0x202003D
	LDI  R30,LOW(32)
	STD  Y+21,R30
	RJMP _0x2020035
_0x202003D:
	RJMP _0x202003E
_0x2020039:
	CPI  R30,LOW(0x2)
	BRNE _0x202003F
_0x202003E:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x2020040
	ORI  R16,LOW(128)
	RJMP _0x2020035
_0x2020040:
	RJMP _0x2020041
_0x202003F:
	CPI  R30,LOW(0x3)
	BRNE _0x2020042
_0x2020041:
	CPI  R18,48
	BRLO _0x2020044
	CPI  R18,58
	BRLO _0x2020045
_0x2020044:
	RJMP _0x2020043
_0x2020045:
	LDI  R26,LOW(10)
	MUL  R21,R26
	MOV  R21,R0
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x2020035
_0x2020043:
	LDI  R20,LOW(0)
	CPI  R18,46
	BRNE _0x2020046
	LDI  R17,LOW(4)
	RJMP _0x2020035
_0x2020046:
	RJMP _0x2020047
_0x2020042:
	CPI  R30,LOW(0x4)
	BRNE _0x2020049
	CPI  R18,48
	BRLO _0x202004B
	CPI  R18,58
	BRLO _0x202004C
_0x202004B:
	RJMP _0x202004A
_0x202004C:
	ORI  R16,LOW(32)
	LDI  R26,LOW(10)
	MUL  R20,R26
	MOV  R20,R0
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R20,R30
	RJMP _0x2020035
_0x202004A:
_0x2020047:
	CPI  R18,108
	BRNE _0x202004D
	ORI  R16,LOW(2)
	LDI  R17,LOW(5)
	RJMP _0x2020035
_0x202004D:
	RJMP _0x202004E
_0x2020049:
	CPI  R30,LOW(0x5)
	BREQ PC+2
	RJMP _0x2020035
_0x202004E:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x2020053
	CALL SUBOPT_0x34
	CALL SUBOPT_0x35
	CALL SUBOPT_0x34
	LDD  R26,Z+4
	ST   -Y,R26
	CALL SUBOPT_0x36
	RJMP _0x2020054
_0x2020053:
	CPI  R30,LOW(0x45)
	BREQ _0x2020057
	CPI  R30,LOW(0x65)
	BRNE _0x2020058
_0x2020057:
	RJMP _0x2020059
_0x2020058:
	CPI  R30,LOW(0x66)
	BREQ PC+2
	RJMP _0x202005A
_0x2020059:
	MOVW R30,R28
	ADIW R30,22
	STD  Y+14,R30
	STD  Y+14+1,R31
	CALL SUBOPT_0x37
	CALL __GETD1P
	CALL SUBOPT_0x38
	CALL SUBOPT_0x39
	LDD  R26,Y+13
	TST  R26
	BRMI _0x202005B
	LDD  R26,Y+21
	CPI  R26,LOW(0x2B)
	BREQ _0x202005D
	CPI  R26,LOW(0x20)
	BREQ _0x202005F
	RJMP _0x2020060
_0x202005B:
	CALL SUBOPT_0x3A
	CALL __ANEGF1
	CALL SUBOPT_0x38
	LDI  R30,LOW(45)
	STD  Y+21,R30
_0x202005D:
	SBRS R16,7
	RJMP _0x2020061
	LDD  R30,Y+21
	ST   -Y,R30
	CALL SUBOPT_0x36
	RJMP _0x2020062
_0x2020061:
_0x202005F:
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	ADIW R30,1
	STD  Y+14,R30
	STD  Y+14+1,R31
	SBIW R30,1
	LDD  R26,Y+21
	STD  Z+0,R26
_0x2020062:
_0x2020060:
	SBRS R16,5
	LDI  R20,LOW(6)
	CPI  R18,102
	BRNE _0x2020064
	CALL SUBOPT_0x3A
	CALL __PUTPARD1
	ST   -Y,R20
	LDD  R26,Y+19
	LDD  R27,Y+19+1
	CALL _ftoa
	RJMP _0x2020065
_0x2020064:
	CALL SUBOPT_0x3A
	CALL __PUTPARD1
	ST   -Y,R20
	ST   -Y,R18
	LDD  R26,Y+20
	LDD  R27,Y+20+1
	RCALL __ftoe_G101
_0x2020065:
	MOVW R30,R28
	ADIW R30,22
	CALL SUBOPT_0x3B
	RJMP _0x2020066
_0x202005A:
	CPI  R30,LOW(0x73)
	BRNE _0x2020068
	CALL SUBOPT_0x39
	CALL SUBOPT_0x3C
	CALL SUBOPT_0x3B
	RJMP _0x2020069
_0x2020068:
	CPI  R30,LOW(0x70)
	BRNE _0x202006B
	CALL SUBOPT_0x39
	CALL SUBOPT_0x3C
	STD  Y+14,R30
	STD  Y+14+1,R31
	LDD  R26,Y+14
	LDD  R27,Y+14+1
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x2020069:
	ANDI R16,LOW(127)
	CPI  R20,0
	BREQ _0x202006D
	CP   R20,R17
	BRLO _0x202006E
_0x202006D:
	RJMP _0x202006C
_0x202006E:
	MOV  R17,R20
_0x202006C:
_0x2020066:
	LDI  R20,LOW(0)
	LDI  R30,LOW(0)
	STD  Y+20,R30
	LDI  R19,LOW(0)
	RJMP _0x202006F
_0x202006B:
	CPI  R30,LOW(0x64)
	BREQ _0x2020072
	CPI  R30,LOW(0x69)
	BRNE _0x2020073
_0x2020072:
	ORI  R16,LOW(4)
	RJMP _0x2020074
_0x2020073:
	CPI  R30,LOW(0x75)
	BRNE _0x2020075
_0x2020074:
	LDI  R30,LOW(10)
	STD  Y+20,R30
	SBRS R16,1
	RJMP _0x2020076
	__GETD1N 0x3B9ACA00
	CALL SUBOPT_0x3D
	LDI  R17,LOW(10)
	RJMP _0x2020077
_0x2020076:
	__GETD1N 0x2710
	CALL SUBOPT_0x3D
	LDI  R17,LOW(5)
	RJMP _0x2020077
_0x2020075:
	CPI  R30,LOW(0x58)
	BRNE _0x2020079
	ORI  R16,LOW(8)
	RJMP _0x202007A
_0x2020079:
	CPI  R30,LOW(0x78)
	BREQ PC+2
	RJMP _0x20200B8
_0x202007A:
	LDI  R30,LOW(16)
	STD  Y+20,R30
	SBRS R16,1
	RJMP _0x202007C
	__GETD1N 0x10000000
	CALL SUBOPT_0x3D
	LDI  R17,LOW(8)
	RJMP _0x2020077
_0x202007C:
	__GETD1N 0x1000
	CALL SUBOPT_0x3D
	LDI  R17,LOW(4)
_0x2020077:
	CPI  R20,0
	BREQ _0x202007D
	ANDI R16,LOW(127)
	RJMP _0x202007E
_0x202007D:
	LDI  R20,LOW(1)
_0x202007E:
	SBRS R16,1
	RJMP _0x202007F
	CALL SUBOPT_0x39
	CALL SUBOPT_0x37
	ADIW R26,4
	CALL __GETD1P
	RJMP _0x2020115
_0x202007F:
	SBRS R16,2
	RJMP _0x2020081
	CALL SUBOPT_0x39
	CALL SUBOPT_0x3C
	CALL __CWD1
	RJMP _0x2020115
_0x2020081:
	CALL SUBOPT_0x39
	CALL SUBOPT_0x3C
	CLR  R22
	CLR  R23
_0x2020115:
	__PUTD1S 10
	SBRS R16,2
	RJMP _0x2020083
	LDD  R26,Y+13
	TST  R26
	BRPL _0x2020084
	CALL SUBOPT_0x3A
	CALL __ANEGD1
	CALL SUBOPT_0x38
	LDI  R30,LOW(45)
	STD  Y+21,R30
_0x2020084:
	LDD  R30,Y+21
	CPI  R30,0
	BREQ _0x2020085
	SUBI R17,-LOW(1)
	SUBI R20,-LOW(1)
	RJMP _0x2020086
_0x2020085:
	ANDI R16,LOW(251)
_0x2020086:
_0x2020083:
	MOV  R19,R20
_0x202006F:
	SBRC R16,0
	RJMP _0x2020087
_0x2020088:
	CP   R17,R21
	BRSH _0x202008B
	CP   R19,R21
	BRLO _0x202008C
_0x202008B:
	RJMP _0x202008A
_0x202008C:
	SBRS R16,7
	RJMP _0x202008D
	SBRS R16,2
	RJMP _0x202008E
	ANDI R16,LOW(251)
	LDD  R18,Y+21
	SUBI R17,LOW(1)
	RJMP _0x202008F
_0x202008E:
	LDI  R18,LOW(48)
_0x202008F:
	RJMP _0x2020090
_0x202008D:
	LDI  R18,LOW(32)
_0x2020090:
	CALL SUBOPT_0x33
	SUBI R21,LOW(1)
	RJMP _0x2020088
_0x202008A:
_0x2020087:
_0x2020091:
	CP   R17,R20
	BRSH _0x2020093
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x2020094
	CALL SUBOPT_0x3E
	BREQ _0x2020095
	SUBI R21,LOW(1)
_0x2020095:
	SUBI R17,LOW(1)
	SUBI R20,LOW(1)
_0x2020094:
	LDI  R30,LOW(48)
	ST   -Y,R30
	CALL SUBOPT_0x36
	CPI  R21,0
	BREQ _0x2020096
	SUBI R21,LOW(1)
_0x2020096:
	SUBI R20,LOW(1)
	RJMP _0x2020091
_0x2020093:
	MOV  R19,R17
	LDD  R30,Y+20
	CPI  R30,0
	BRNE _0x2020097
_0x2020098:
	CPI  R19,0
	BREQ _0x202009A
	SBRS R16,3
	RJMP _0x202009B
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	LPM  R18,Z+
	STD  Y+14,R30
	STD  Y+14+1,R31
	RJMP _0x202009C
_0x202009B:
	LDD  R26,Y+14
	LDD  R27,Y+14+1
	LD   R18,X+
	STD  Y+14,R26
	STD  Y+14+1,R27
_0x202009C:
	CALL SUBOPT_0x33
	CPI  R21,0
	BREQ _0x202009D
	SUBI R21,LOW(1)
_0x202009D:
	SUBI R19,LOW(1)
	RJMP _0x2020098
_0x202009A:
	RJMP _0x202009E
_0x2020097:
_0x20200A0:
	CALL SUBOPT_0x3F
	CALL __DIVD21U
	MOV  R18,R30
	CPI  R18,10
	BRLO _0x20200A2
	SBRS R16,3
	RJMP _0x20200A3
	SUBI R18,-LOW(55)
	RJMP _0x20200A4
_0x20200A3:
	SUBI R18,-LOW(87)
_0x20200A4:
	RJMP _0x20200A5
_0x20200A2:
	SUBI R18,-LOW(48)
_0x20200A5:
	SBRC R16,4
	RJMP _0x20200A7
	CPI  R18,49
	BRSH _0x20200A9
	__GETD2S 16
	__CPD2N 0x1
	BRNE _0x20200A8
_0x20200A9:
	RJMP _0x20200AB
_0x20200A8:
	CP   R20,R19
	BRSH _0x2020116
	CP   R21,R19
	BRLO _0x20200AE
	SBRS R16,0
	RJMP _0x20200AF
_0x20200AE:
	RJMP _0x20200AD
_0x20200AF:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x20200B0
_0x2020116:
	LDI  R18,LOW(48)
_0x20200AB:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x20200B1
	CALL SUBOPT_0x3E
	BREQ _0x20200B2
	SUBI R21,LOW(1)
_0x20200B2:
_0x20200B1:
_0x20200B0:
_0x20200A7:
	CALL SUBOPT_0x33
	CPI  R21,0
	BREQ _0x20200B3
	SUBI R21,LOW(1)
_0x20200B3:
_0x20200AD:
	SUBI R19,LOW(1)
	CALL SUBOPT_0x3F
	CALL __MODD21U
	CALL SUBOPT_0x38
	LDD  R30,Y+20
	__GETD2S 16
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __DIVD21U
	CALL SUBOPT_0x3D
	__GETD1S 16
	CALL __CPD10
	BREQ _0x20200A1
	RJMP _0x20200A0
_0x20200A1:
_0x202009E:
	SBRS R16,0
	RJMP _0x20200B4
_0x20200B5:
	CPI  R21,0
	BREQ _0x20200B7
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	ST   -Y,R30
	CALL SUBOPT_0x36
	RJMP _0x20200B5
_0x20200B7:
_0x20200B4:
_0x20200B8:
_0x2020054:
_0x2020114:
	LDI  R17,LOW(0)
_0x2020035:
	RJMP _0x2020030
_0x2020032:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	CALL __GETW1P
	CALL __LOADLOCR6
	ADIW R28,63
	ADIW R28,31
	RET
; .FEND
_sprintf:
; .FSTART _sprintf
	PUSH R15
	MOV  R15,R24
	SBIW R28,6
	CALL __SAVELOCR4
	CALL SUBOPT_0x40
	SBIW R30,0
	BRNE _0x20200B9
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20C0004
_0x20200B9:
	MOVW R26,R28
	ADIW R26,6
	CALL __ADDW2R15
	MOVW R16,R26
	CALL SUBOPT_0x40
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R30,LOW(0)
	STD  Y+8,R30
	STD  Y+8+1,R30
	MOVW R26,R28
	ADIW R26,10
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	LDI  R30,LOW(_put_buff_G101)
	LDI  R31,HIGH(_put_buff_G101)
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R28
	ADIW R26,10
	RCALL __print_G101
	MOVW R18,R30
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	MOVW R30,R18
_0x20C0004:
	CALL __LOADLOCR4
	ADIW R28,10
	POP  R15
	RET
; .FEND

	.CSEG
_ftoa:
; .FSTART _ftoa
	CALL SUBOPT_0x26
	LDI  R30,LOW(0)
	STD  Y+2,R30
	LDI  R30,LOW(63)
	STD  Y+3,R30
	ST   -Y,R17
	ST   -Y,R16
	LDD  R30,Y+11
	LDD  R31,Y+11+1
	CPI  R30,LOW(0xFFFF)
	LDI  R26,HIGH(0xFFFF)
	CPC  R31,R26
	BRNE _0x204000D
	CALL SUBOPT_0x41
	__POINTW2FN _0x2040000,0
	CALL _strcpyf
	RJMP _0x20C0003
_0x204000D:
	CPI  R30,LOW(0x7FFF)
	LDI  R26,HIGH(0x7FFF)
	CPC  R31,R26
	BRNE _0x204000C
	CALL SUBOPT_0x41
	__POINTW2FN _0x2040000,1
	CALL _strcpyf
	RJMP _0x20C0003
_0x204000C:
	LDD  R26,Y+12
	TST  R26
	BRPL _0x204000F
	__GETD1S 9
	CALL __ANEGF1
	CALL SUBOPT_0x42
	CALL SUBOPT_0x43
	LDI  R30,LOW(45)
	ST   X,R30
_0x204000F:
	LDD  R26,Y+8
	CPI  R26,LOW(0x7)
	BRLO _0x2040010
	LDI  R30,LOW(6)
	STD  Y+8,R30
_0x2040010:
	LDD  R17,Y+8
_0x2040011:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x2040013
	CALL SUBOPT_0x44
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x45
	RJMP _0x2040011
_0x2040013:
	CALL SUBOPT_0x46
	CALL __ADDF12
	CALL SUBOPT_0x42
	LDI  R17,LOW(0)
	__GETD1N 0x3F800000
	CALL SUBOPT_0x45
_0x2040014:
	CALL SUBOPT_0x46
	CALL __CMPF12
	BRLO _0x2040016
	CALL SUBOPT_0x44
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x45
	SUBI R17,-LOW(1)
	CPI  R17,39
	BRLO _0x2040017
	CALL SUBOPT_0x41
	__POINTW2FN _0x2040000,5
	CALL _strcpyf
	RJMP _0x20C0003
_0x2040017:
	RJMP _0x2040014
_0x2040016:
	CPI  R17,0
	BRNE _0x2040018
	CALL SUBOPT_0x43
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x2040019
_0x2040018:
_0x204001A:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x204001C
	CALL SUBOPT_0x44
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	CALL _floor
	CALL SUBOPT_0x45
	CALL SUBOPT_0x46
	CALL __DIVF21
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x43
	CALL SUBOPT_0x30
	LDI  R31,0
	CALL SUBOPT_0x44
	CALL __CWD1
	CALL __CDF1
	CALL __MULF12
	CALL SUBOPT_0x47
	CALL SUBOPT_0x31
	CALL SUBOPT_0x42
	RJMP _0x204001A
_0x204001C:
_0x2040019:
	LDD  R30,Y+8
	CPI  R30,0
	BREQ _0x20C0002
	CALL SUBOPT_0x43
	LDI  R30,LOW(46)
	ST   X,R30
_0x204001E:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x2040020
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x42
	__GETD1S 9
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x43
	CALL SUBOPT_0x30
	LDI  R31,0
	CALL SUBOPT_0x47
	CALL __CWD1
	CALL __CDF1
	CALL SUBOPT_0x31
	CALL SUBOPT_0x42
	RJMP _0x204001E
_0x2040020:
_0x20C0002:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x20C0003:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
; .FEND

	.DSEG

	.CSEG

	.CSEG

	.CSEG
_strcpyf:
; .FSTART _strcpyf
	ST   -Y,R27
	ST   -Y,R26
    ld   r30,y+
    ld   r31,y+
    ld   r26,y+
    ld   r27,y+
    movw r24,r26
strcpyf0:
	lpm  r0,z+
    st   x+,r0
    tst  r0
    brne strcpyf0
    movw r30,r24
    ret
; .FEND
_strlen:
; .FSTART _strlen
	ST   -Y,R27
	ST   -Y,R26
    ld   r26,y+
    ld   r27,y+
    clr  r30
    clr  r31
strlen0:
    ld   r22,x+
    tst  r22
    breq strlen1
    adiw r30,1
    rjmp strlen0
strlen1:
    ret
; .FEND
_strlenf:
; .FSTART _strlenf
	ST   -Y,R27
	ST   -Y,R26
    clr  r26
    clr  r27
    ld   r30,y+
    ld   r31,y+
strlenf0:
	lpm  r0,z+
    tst  r0
    breq strlenf1
    adiw r26,1
    rjmp strlenf0
strlenf1:
    movw r30,r26
    ret
; .FEND

	.CSEG
_ftrunc:
; .FSTART _ftrunc
	CALL __PUTPARD2
   ldd  r23,y+3
   ldd  r22,y+2
   ldd  r31,y+1
   ld   r30,y
   bst  r23,7
   lsl  r23
   sbrc r22,7
   sbr  r23,1
   mov  r25,r23
   subi r25,0x7e
   breq __ftrunc0
   brcs __ftrunc0
   cpi  r25,24
   brsh __ftrunc1
   clr  r26
   clr  r27
   clr  r24
__ftrunc2:
   sec
   ror  r24
   ror  r27
   ror  r26
   dec  r25
   brne __ftrunc2
   and  r30,r26
   and  r31,r27
   and  r22,r24
   rjmp __ftrunc1
__ftrunc0:
   clt
   clr  r23
   clr  r30
   clr  r31
   clr  r22
__ftrunc1:
   cbr  r22,0x80
   lsr  r23
   brcc __ftrunc3
   sbr  r22,0x80
__ftrunc3:
   bld  r23,7
   ld   r26,y+
   ld   r27,y+
   ld   r24,y+
   ld   r25,y+
   cp   r30,r26
   cpc  r31,r27
   cpc  r22,r24
   cpc  r23,r25
   bst  r25,7
   ret
; .FEND
_floor:
; .FSTART _floor
	CALL __PUTPARD2
	CALL __GETD2S0
	CALL _ftrunc
	CALL __PUTD1S0
    brne __floor1
__floor0:
	CALL __GETD1S0
	RJMP _0x20C0001
__floor1:
    brtc __floor0
	CALL __GETD1S0
	__GETD2N 0x3F800000
	CALL __SUBF12
_0x20C0001:
	ADIW R28,4
	RET
; .FEND

	.DSEG
_Weight_Shiwu:
	.BYTE 0x4
_Weight_Maopi:
	.BYTE 0x4
_Weight_Shiwu1:
	.BYTE 0x4
_Weight_Maopi1:
	.BYTE 0x4
_Weight_Shiwu2:
	.BYTE 0x4
_Weight_Maopi2:
	.BYTE 0x4
_HX711_Buffer:
	.BYTE 0x4
_HX711_Buffer1:
	.BYTE 0x4
_HX711_Buffer2:
	.BYTE 0x4
_scale:
	.BYTE 0x4
_scale1:
	.BYTE 0x4
_scale2:
	.BYTE 0x4
_scale_sum:
	.BYTE 0x4
_scale1_sum:
	.BYTE 0x4
_scale2_sum:
	.BYTE 0x4
_buffer:
	.BYTE 0x1E
_buffer_test:
	.BYTE 0x20
_str1:
	.BYTE 0xA
_str2:
	.BYTE 0xA
_str3:
	.BYTE 0xA
_rx_buffer:
	.BYTE 0x18
__base_y_G100:
	.BYTE 0x4
__seed_G102:
	.BYTE 0x4

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x0:
	LDS  R30,_scale
	LDS  R31,_scale+1
	LDS  R22,_scale+2
	LDS  R23,_scale+3
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x1:
	LDS  R30,_scale1
	LDS  R31,_scale1+1
	LDS  R22,_scale1+2
	LDS  R23,_scale1+3
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2:
	LDS  R30,_scale2
	LDS  R31,_scale2+1
	LDS  R22,_scale2+2
	LDS  R23,_scale2+3
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4:
	LDI  R30,LOW(0)
	__CLRD1S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x5:
	__GETD1S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6:
	CALL __LSLD1
	__PUTD1S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x7:
	RCALL SUBOPT_0x5
	__SUBD1N -1
	__PUTD1S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x8:
	__GETD2S 1
	__GETD1N 0x800000
	CALL __XORD12
	__PUTD1S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x9:
	LDI  R30,LOW(0)
	STS  _HX711_Buffer,R30
	STS  _HX711_Buffer+1,R30
	STS  _HX711_Buffer+2,R30
	STS  _HX711_Buffer+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xA:
	STS  _HX711_Buffer,R30
	STS  _HX711_Buffer+1,R31
	STS  _HX711_Buffer+2,R22
	STS  _HX711_Buffer+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xB:
	LDS  R26,_HX711_Buffer
	LDS  R27,_HX711_Buffer+1
	LDS  R24,_HX711_Buffer+2
	LDS  R25,_HX711_Buffer+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0xC:
	__GETD1N 0x64
	CALL __DIVD21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD:
	STS  _Weight_Shiwu,R30
	STS  _Weight_Shiwu+1,R31
	STS  _Weight_Shiwu+2,R22
	STS  _Weight_Shiwu+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE:
	LDS  R30,_Weight_Shiwu
	LDS  R31,_Weight_Shiwu+1
	LDS  R22,_Weight_Shiwu+2
	LDS  R23,_Weight_Shiwu+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF:
	RCALL SUBOPT_0xE
	CALL __CDF1U
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x10:
	__GETD1N 0x3EB8A04D
	CALL __DIVF21
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x11:
	CALL __CFD1U
	CALL __PUTDP1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x12:
	__GETD1N 0x41F00000
	CALL __DIVF21
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x13:
	LDI  R30,LOW(0)
	STS  _HX711_Buffer1,R30
	STS  _HX711_Buffer1+1,R30
	STS  _HX711_Buffer1+2,R30
	STS  _HX711_Buffer1+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x14:
	STS  _HX711_Buffer1,R30
	STS  _HX711_Buffer1+1,R31
	STS  _HX711_Buffer1+2,R22
	STS  _HX711_Buffer1+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x15:
	LDS  R26,_HX711_Buffer1
	LDS  R27,_HX711_Buffer1+1
	LDS  R24,_HX711_Buffer1+2
	LDS  R25,_HX711_Buffer1+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x16:
	STS  _Weight_Shiwu1,R30
	STS  _Weight_Shiwu1+1,R31
	STS  _Weight_Shiwu1+2,R22
	STS  _Weight_Shiwu1+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x17:
	LDS  R30,_Weight_Shiwu1
	LDS  R31,_Weight_Shiwu1+1
	LDS  R22,_Weight_Shiwu1+2
	LDS  R23,_Weight_Shiwu1+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x18:
	RCALL SUBOPT_0x17
	CALL __CDF1U
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x19:
	LDI  R30,LOW(0)
	STS  _HX711_Buffer2,R30
	STS  _HX711_Buffer2+1,R30
	STS  _HX711_Buffer2+2,R30
	STS  _HX711_Buffer2+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x1A:
	STS  _HX711_Buffer2,R30
	STS  _HX711_Buffer2+1,R31
	STS  _HX711_Buffer2+2,R22
	STS  _HX711_Buffer2+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x1B:
	LDS  R26,_HX711_Buffer2
	LDS  R27,_HX711_Buffer2+1
	LDS  R24,_HX711_Buffer2+2
	LDS  R25,_HX711_Buffer2+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1C:
	STS  _Weight_Shiwu2,R30
	STS  _Weight_Shiwu2+1,R31
	STS  _Weight_Shiwu2+2,R22
	STS  _Weight_Shiwu2+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1D:
	LDS  R30,_Weight_Shiwu2
	LDS  R31,_Weight_Shiwu2+1
	LDS  R22,_Weight_Shiwu2+2
	LDS  R23,_Weight_Shiwu2+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1E:
	RCALL SUBOPT_0x1D
	CALL __CDF1U
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1F:
	ST   -Y,R17
	ST   -Y,R16
	__GETWRN 16,17,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x20:
	__DELAY_USW 1000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x21:
	__GETD1N 0xA
	CALL __DIVD21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x22:
	CALL __lcd_write_data
	LDI  R26,LOW(3)
	LDI  R27,0
	JMP  _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x23:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOV  R17,R30
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x24:
	LDI  R26,LOW(48)
	CALL __lcd_write_nibble_G100
	__DELAY_USW 200
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x25:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x26:
	ST   -Y,R27
	ST   -Y,R26
	SBIW R28,4
	LDI  R30,LOW(0)
	ST   Y,R30
	STD  Y+1,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x27:
	__GETD2S 4
	__GETD1N 0x41200000
	CALL __MULF12
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x28:
	__GETD1S 4
	__GETD2S 12
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x29:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x2A:
	__GETD1N 0x3DCCCCCD
	CALL __MULF12
	__PUTD1S 12
	SUBI R19,-LOW(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2B:
	__GETD1N 0x41200000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2C:
	__PUTD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2D:
	__GETD2N 0x3F000000
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2E:
	__GETD1N 0x3DCCCCCD
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2F:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADIW R26,1
	STD  Y+8,R26
	STD  Y+8+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x30:
	MOV  R30,R16
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x31:
	CALL __SWAPD12
	CALL __SUBF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x32:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x33:
	ST   -Y,R18
	LDD  R26,Y+7
	LDD  R27,Y+7+1
	LDD  R30,Y+9
	LDD  R31,Y+9+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x34:
	__GETW1SX 90
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x35:
	SBIW R30,4
	__PUTW1SX 90
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x36:
	LDD  R26,Y+7
	LDD  R27,Y+7+1
	LDD  R30,Y+9
	LDD  R31,Y+9+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x37:
	__GETW2SX 90
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x38:
	__PUTD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x39:
	RCALL SUBOPT_0x34
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3A:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x3B:
	STD  Y+14,R30
	STD  Y+14+1,R31
	LDD  R26,Y+14
	LDD  R27,Y+14+1
	CALL _strlen
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x3C:
	RCALL SUBOPT_0x37
	ADIW R26,4
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x3D:
	__PUTD1S 16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x3E:
	ANDI R16,LOW(251)
	LDD  R30,Y+21
	ST   -Y,R30
	__GETW2SX 87
	__GETW1SX 89
	ICALL
	CPI  R21,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3F:
	__GETD1S 16
	__GETD2S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x40:
	MOVW R26,R28
	ADIW R26,12
	CALL __ADDW2R15
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x41:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x42:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x43:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x44:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x45:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x46:
	__GETD1S 2
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x47:
	__GETD2S 9
	RET


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x7D0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__ANEGF1:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __ANEGF10
	SUBI R23,0x80
__ANEGF10:
	RET

__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1U:
	SET
	RJMP __CFD1U0
__CFD1:
	CLT
__CFD1U0:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRTC __CFD19
	CPI  R21,9
__CFD19:
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	BLD  R23,7
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__SWAPACC:
	PUSH R20
	MOVW R20,R30
	MOVW R30,R26
	MOVW R26,R20
	MOVW R20,R22
	MOVW R22,R24
	MOVW R24,R20
	MOV  R20,R0
	MOV  R0,R1
	MOV  R1,R20
	POP  R20
	RET

__UADD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	RET

__NEGMAN1:
	COM  R30
	COM  R31
	COM  R22
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	RET

__SUBF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129
	LDI  R21,0x80
	EOR  R1,R21

	RJMP __ADDF120

__ADDF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129

__ADDF120:
	CPI  R23,0x80
	BREQ __ADDF128
__ADDF121:
	MOV  R21,R23
	SUB  R21,R25
	BRVS __ADDF1211
	BRPL __ADDF122
	RCALL __SWAPACC
	RJMP __ADDF121
__ADDF122:
	CPI  R21,24
	BRLO __ADDF123
	CLR  R26
	CLR  R27
	CLR  R24
__ADDF123:
	CPI  R21,8
	BRLO __ADDF124
	MOV  R26,R27
	MOV  R27,R24
	CLR  R24
	SUBI R21,8
	RJMP __ADDF123
__ADDF124:
	TST  R21
	BREQ __ADDF126
__ADDF125:
	LSR  R24
	ROR  R27
	ROR  R26
	DEC  R21
	BRNE __ADDF125
__ADDF126:
	MOV  R21,R0
	EOR  R21,R1
	BRMI __ADDF127
	RCALL __UADD12
	BRCC __ADDF129
	ROR  R22
	ROR  R31
	ROR  R30
	INC  R23
	BRVC __ADDF129
	RJMP __MAXRES
__ADDF128:
	RCALL __SWAPACC
__ADDF129:
	RCALL __REPACK
	POP  R21
	RET
__ADDF1211:
	BRCC __ADDF128
	RJMP __ADDF129
__ADDF127:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	BREQ __ZERORES
	BRCC __ADDF1210
	COM  R0
	RCALL __NEGMAN1
__ADDF1210:
	TST  R22
	BRMI __ADDF129
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVC __ADDF1210

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R0
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R17
	CLR  R18
	CLR  R25
	MUL  R22,R24
	MOVW R20,R0
	MUL  R24,R31
	MOV  R19,R0
	ADD  R20,R1
	ADC  R21,R25
	MUL  R22,R27
	ADD  R19,R0
	ADC  R20,R1
	ADC  R21,R25
	MUL  R24,R30
	RCALL __MULF126
	MUL  R27,R31
	RCALL __MULF126
	MUL  R22,R26
	RCALL __MULF126
	MUL  R27,R30
	RCALL __MULF127
	MUL  R26,R31
	RCALL __MULF127
	MUL  R26,R30
	ADD  R17,R1
	ADC  R18,R25
	ADC  R19,R25
	ADC  R20,R25
	ADC  R21,R25
	MOV  R30,R19
	MOV  R31,R20
	MOV  R22,R21
	MOV  R21,R18
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	POP  R0
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__MULF127:
	ADD  R17,R0
	ADC  R18,R1
	ADC  R19,R25
	RJMP __MULF128
__MULF126:
	ADD  R18,R0
	ADC  R19,R1
__MULF128:
	ADC  R20,R25
	ADC  R21,R25
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __DIVF219
	RJMP __MINRES
__DIVF219:
	RJMP __MAXRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R17
	CLR  R18
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,32
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R20,R17
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R20,R17
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R21
	ROL  R18
	ROL  R19
	ROL  R1
	ROL  R26
	ROL  R27
	ROL  R24
	ROL  R20
	DEC  R25
	BRNE __DIVF212
	MOVW R30,R18
	MOV  R22,R1
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	TST  R22
	BRMI __DIVF215
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__CMPF12:
	TST  R25
	BRMI __CMPF120
	TST  R23
	BRMI __CMPF121
	CP   R25,R23
	BRLO __CMPF122
	BRNE __CMPF121
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	BRLO __CMPF122
	BREQ __CMPF123
__CMPF121:
	CLZ
	CLC
	RET
__CMPF122:
	CLZ
	SEC
	RET
__CMPF123:
	SEZ
	CLC
	RET
__CMPF120:
	TST  R23
	BRPL __CMPF122
	CP   R25,R23
	BRLO __CMPF121
	BRNE __CMPF122
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	BRLO __CMPF122
	BREQ __CMPF123
	RJMP __CMPF121

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ADDD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	ADC  R23,R25
	RET

__SUBD12:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	SBC  R23,R25
	RET

__XORD12:
	EOR  R30,R26
	EOR  R31,R27
	EOR  R22,R24
	EOR  R23,R25
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__LSLD1:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	RET

__CBD1:
	MOV  R31,R30
	ADD  R31,R31
	SBC  R31,R31
	MOV  R22,R31
	MOV  R23,R31
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__DIVB21U:
	CLR  R0
	LDI  R25,8
__DIVB21U1:
	LSL  R26
	ROL  R0
	SUB  R0,R30
	BRCC __DIVB21U2
	ADD  R0,R30
	RJMP __DIVB21U3
__DIVB21U2:
	SBR  R26,1
__DIVB21U3:
	DEC  R25
	BRNE __DIVB21U1
	MOV  R30,R26
	MOV  R26,R0
	RET

__DIVB21:
	RCALL __CHKSIGNB
	RCALL __DIVB21U
	BRTC __DIVB211
	NEG  R30
__DIVB211:
	RET

__DIVD21U:
	PUSH R19
	PUSH R20
	PUSH R21
	CLR  R0
	CLR  R1
	CLR  R20
	CLR  R21
	LDI  R19,32
__DIVD21U1:
	LSL  R26
	ROL  R27
	ROL  R24
	ROL  R25
	ROL  R0
	ROL  R1
	ROL  R20
	ROL  R21
	SUB  R0,R30
	SBC  R1,R31
	SBC  R20,R22
	SBC  R21,R23
	BRCC __DIVD21U2
	ADD  R0,R30
	ADC  R1,R31
	ADC  R20,R22
	ADC  R21,R23
	RJMP __DIVD21U3
__DIVD21U2:
	SBR  R26,1
__DIVD21U3:
	DEC  R19
	BRNE __DIVD21U1
	MOVW R30,R26
	MOVW R22,R24
	MOVW R26,R0
	MOVW R24,R20
	POP  R21
	POP  R20
	POP  R19
	RET

__MODB21:
	CLT
	SBRS R26,7
	RJMP __MODB211
	NEG  R26
	SET
__MODB211:
	SBRC R30,7
	NEG  R30
	RCALL __DIVB21U
	MOV  R30,R26
	BRTC __MODB212
	NEG  R30
__MODB212:
	RET

__MODD21U:
	RCALL __DIVD21U
	MOVW R30,R26
	MOVW R22,R24
	RET

__CHKSIGNB:
	CLT
	SBRS R30,7
	RJMP __CHKSB1
	NEG  R30
	SET
__CHKSB1:
	SBRS R26,7
	RJMP __CHKSB2
	NEG  R26
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSB2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETD1P:
	LD   R30,X+
	LD   R31,X+
	LD   R22,X+
	LD   R23,X
	SBIW R26,3
	RET

__PUTDP1:
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	RET

__GETD1S0:
	LD   R30,Y
	LDD  R31,Y+1
	LDD  R22,Y+2
	LDD  R23,Y+3
	RET

__GETD2S0:
	LD   R26,Y
	LDD  R27,Y+1
	LDD  R24,Y+2
	LDD  R25,Y+3
	RET

__PUTD1S0:
	ST   Y,R30
	STD  Y+1,R31
	STD  Y+2,R22
	STD  Y+3,R23
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__PUTPARD2:
	ST   -Y,R25
	ST   -Y,R24
	ST   -Y,R27
	ST   -Y,R26
	RET

__SWAPD12:
	MOV  R1,R24
	MOV  R24,R22
	MOV  R22,R1
	MOV  R1,R25
	MOV  R25,R23
	MOV  R23,R1

__SWAPW12:
	MOV  R1,R27
	MOV  R27,R31
	MOV  R31,R1

__SWAPB12:
	MOV  R1,R26
	MOV  R26,R30
	MOV  R30,R1
	RET

__CPD10:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
