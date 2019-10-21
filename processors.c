//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#include <stdio.h>
#include <string.h>

#include "m8cdis.h"
#include "mapfile.h"
#include "processors.h"

static char *cy8c24894User [] = 
{
  "PRT0DR",        // 0x00
  "PRT0IE",        // 0x01
  "PRT0GS",        // 0x02
  "PRT0DM2",       // 0x03
  "PRT1DR",        // 0x04
  "PRT1IE",        // 0x05
  "PRT1GS",        // 0x06
  "PRT1DM2",       // 0x07
  "PRT2DR",        // 0x08
  "PRT2IE",        // 0x09
  "PRT2GS",        // 0x0a
  "PRT2DM2",       // 0x0b
  "PRT3DR",        // 0x0c
  "PRT3IE",        // 0x0d
  "PRT3GS",        // 0x0e
  "PRT3DM2",       // 0x0f
  "PRT4DR",        // 0x10
  "PRT4IE",        // 0x11
  "PRT4GS",        // 0x12
  "PRT4DM2",       // 0x13
  "PRT5DR",        // 0x14
  "PRT5IE",        // 0x15
  "PRT5GS",        // 0x16
  "PRT5DM2",       // 0x17
  "NOT_USED",      // 0x18
  "NOT_USED",      // 0x19
  "NOT_USED",      // 0x1a
  "NOT_USED",      // 0x1b
  "PRT7DR",        // 0x1c
  "PRT7IE",        // 0x1d
  "PRT7GS",        // 0x1e
  "PRT7DM2",       // 0x1f
  "DBB00DR0",      // 0x20
  "DBB00DR1",      // 0x21
  "DBB00DR2",      // 0x22
  "DBB00CR0",      // 0x23
  "DBB01DR0",      // 0x24
  "DBB01DR1",      // 0x25
  "DBB01DR2",      // 0x26
  "DBB01CR0",      // 0x27
  "DCB02DR0",      // 0x28
  "DCB02DR1",      // 0x29
  "DCB02DR2",      // 0x2a
  "DCB02CR0",      // 0x2b
  "DCB03DR0",      // 0x2c
  "DCB03DR1",      // 0x2d
  "DCB03DR2",      // 0x2e
  "DCB03CR0",      // 0x2f
  "NOT_USED",      // 0x30
  "NOT_USED",      // 0x31
  "NOT_USED",      // 0x32
  "NOT_USED",      // 0x33
  "NOT_USED",      // 0x34
  "NOT_USED",      // 0x35
  "NOT_USED",      // 0x36
  "NOT_USED",      // 0x37
  "NOT_USED",      // 0x38
  "NOT_USED",      // 0x39
  "NOT_USED",      // 0x3a
  "NOT_USED",      // 0x3b
  "NOT_USED",      // 0x3c
  "NOT_USED",      // 0x3d
  "NOT_USED",      // 0x3e
  "NOT_USED",      // 0x3f
  "PMA0_DR",       // 0x40
  "PMA1_DR",       // 0x41
  "PMA2_DR",       // 0x42
  "PMA3_DR",       // 0x43
  "PMA4_DR",       // 0x44
  "PMA5_DR",       // 0x45
  "PMA6_DR",       // 0x46
  "PMA7_DR",       // 0x47
  "USB_SOF0",      // 0x48
  "USB_SOF1",      // 0x49
  "USB_CR0",       // 0x4a
  "USBI/O_CR0",    // 0x4b
  "USBI/O_CR1",    // 0x4c
  "NOT_USED",      // 0x4d
  "EP1_CNT1",      // 0x4e
  "EP1_CNT",       // 0x4f
  "EP2_CNT1",      // 0x50
  "EP2_CNT",       // 0x51
  "EP3_CNT1",      // 0x52
  "EP3_CNT",       // 0x53
  "EP4_CNT1",      // 0x54
  "EP4_CNT",       // 0x55
  "EP0_CR",        // 0x56
  "EP0_CNT",       // 0x57
  "EP0_DR0",       // 0x58
  "EP0_DR1",       // 0x59
  "EP0_DR2",       // 0x5a
  "EP0_DR3",       // 0x5b
  "EP0_DR4",       // 0x5c
  "EP0_DR5",       // 0x5d
  "EP0_DR6",       // 0x5e
  "EP0_DR7",       // 0x5f
  "AMX_IN",        // 0x60
  "AMUXCFG",       // 0x61
  "NOT_USED",      // 0x62
  "ARF_CR",        // 0x63
  "CMP_CR0",       // 0x64
  "ASY_CR",        // 0x65
  "CMP_CR1",       // 0x66
  "NOT_USED",      // 0x67
  "NOT_USED",      // 0x68
  "NOT_USED",      // 0x69
  "NOT_USED",      // 0x6a
  "NOT_USED",      // 0x6b
  "TMP_DR0",       // 0x6c
  "TMP_DR1",       // 0x6d
  "TMP_DR2",       // 0x6e
  "TMP_DR3",       // 0x6f
  "ACB00CR3",      // 0x70
  "ACB00CR0",      // 0x71
  "ACB00CR1",      // 0x72
  "ACB00CR2",      // 0x73
  "ACB01CR3",      // 0x74
  "ACB01CR0",      // 0x75
  "ACB01CR1",      // 0x76
  "ACB01CR2",      // 0x77
  "NOT_USED",      // 0x78
  "NOT_USED",      // 0x79
  "NOT_USED",      // 0x7a
  "NOT_USED",      // 0x7b
  "NOT_USED",      // 0x7c
  "NOT_USED",      // 0x7d
  "NOT_USED",      // 0x7e
  "NOT_USED",      // 0x7f
  "ASC10CR0",      // 0x80
  "ASC10CR1",      // 0x81
  "ASC10CR2",      // 0x82
  "ASC10CR3",      // 0x83
  "ASD11CR0",      // 0x84
  "ASD11CR1",      // 0x85
  "ASD11CR2",      // 0x86
  "ASD11CR3",      // 0x87
  "NOT_USED",      // 0x88
  "NOT_USED",      // 0x89
  "NOT_USED",      // 0x8a
  "NOT_USED",      // 0x8b
  "NOT_USED",      // 0x8c
  "NOT_USED",      // 0x8d
  "NOT_USED",      // 0x8e
  "NOT_USED",      // 0x8f
  "ASD20CR0",      // 0x90
  "ASD20CR1",      // 0x91
  "ASD20CR2",      // 0x92
  "ASD20CR3",      // 0x93
  "ASC21CR0",      // 0x94
  "ASC21CR1",      // 0x95
  "ASC21CR2",      // 0x96
  "ASC21CR3",      // 0x97
  "NOT_USED",      // 0x98
  "NOT_USED",      // 0x99
  "NOT_USED",      // 0x9a
  "NOT_USED",      // 0x9b
  "NOT_USED",      // 0x9c
  "NOT_USED",      // 0x9d
  "NOT_USED",      // 0x9e
  "NOT_USED",      // 0x9f
  "NOT_USED",      // 0xa0
  "NOT_USED",      // 0xa1
  "NOT_USED",      // 0xa2
  "NOT_USED",      // 0xa3
  "NOT_USED",      // 0xa4
  "NOT_USED",      // 0xa5
  "NOT_USED",      // 0xa6
  "NOT_USED",      // 0xa7
  "MUL1_X",        // 0xa8
  "MUL1_Y",        // 0xa9
  "MUL1_DH",       // 0xaa
  "MUL1_DL",       // 0xab
  "ACC1_DR1",      // 0xac
  "ACC1_DR0",      // 0xad
  "ACC1_DR3",      // 0xae
  "ACC1_DR2",      // 0xaf
  "RDI0RI",        // 0xb0
  "RDI0SYN",       // 0xb1
  "RDI0IS",        // 0xb2
  "RDI0LT0",       // 0xb3
  "RDI0LT1",       // 0xb4
  "RDI0RO0",       // 0xb5
  "RDI0RO1",       // 0xb6
  "NOT_USED",      // 0xb7
  "NOT_USED",      // 0xb8
  "NOT_USED",      // 0xb9
  "NOT_USED",      // 0xba
  "NOT_USED",      // 0xbb
  "NOT_USED",      // 0xbc
  "NOT_USED",      // 0xbd
  "NOT_USED",      // 0xbe
  "NOT_USED",      // 0xbf
  "NOT_USED",      // 0xc0
  "NOT_USED",      // 0xc1
  "NOT_USED",      // 0xc2
  "NOT_USED",      // 0xc3
  "NOT_USED",      // 0xc4
  "NOT_USED",      // 0xc5
  "NOT_USED",      // 0xc6
  "NOT_USED",      // 0xc7
  "NOT_USED",      // 0xc8
  "NOT_USED",      // 0xc9
  "NOT_USED",      // 0xca
  "NOT_USED",      // 0xcb
  "NOT_USED",      // 0xcc
  "NOT_USED",      // 0xcd
  "NOT_USED",      // 0xce
  "NOT_USED",      // 0xcf
  "CUR_PP",        // 0xd0
  "STK_PP",        // 0xd1
  "NOT_USED",      // 0xd2
  "IDX_PP",        // 0xd3
  "MVR_PP",        // 0xd4
  "MVW_PP",        // 0xd5
  "I2C_CFG",       // 0xd6
  "I2C_SCR",       // 0xd7
  "I2C_DR",        // 0xd8
  "I2C_MSCR",      // 0xd9
  "INT_CLR0",      // 0xda
  "INT_CLR1",      // 0xdb
  "INT_CLR2",      // 0xdc
  "INT_CLR3",      // 0xdd
  "INT_MSK3",      // 0xde
  "INT_MSK2",      // 0xdf
  "INT_MSK0",      // 0xe0
  "INT_MSK1",      // 0xe1
  "INT_VC",        // 0xe2
  "RES_WDT",       // 0xe3
  "DEC_DH",        // 0xe4
  "DEC_DL",        // 0xe5
  "DEC_CR0",       // 0xe6
  "DEC_CR1",       // 0xe7
  "MUL0_X",        // 0xe8
  "MUL0_Y",        // 0xe9
  "MUL0_DH",       // 0xea
  "MUL0_DL",       // 0xeb
  "ACC0_DR1",      // 0xec
  "ACC0_DR0",      // 0xed
  "ACC0_DR3",      // 0xee
  "ACC0_DR2",      // 0xef
  "NOT_USED",      // 0xf0
  "NOT_USED",      // 0xf1
  "NOT_USED",      // 0xf2
  "NOT_USED",      // 0xf3
  "NOT_USED",      // 0xf4
  "NOT_USED",      // 0xf5
  "NOT_USED",      // 0xf6
  "CPU_F",         // 0xf7
  "NOT_USED",      // 0xf8
  "NOT_USED",      // 0xf9
  "NOT_USED",      // 0xfa
  "NOT_USED",      // 0xfb
  "NOT_USED",      // 0xfc
  "DAC_D",         // 0xfd
  "CPU_SCR1",      // 0xfe
  "CPU_SCR0",      // 0xff
};

static char *cy8c24894Config [] =
{
  "PRT0DM0",       // 0x00
  "PRT0DM1",       // 0x01
  "PRT0IC0",       // 0x02
  "PRT0IC1",       // 0x03
  "PRT1DM0",       // 0x04
  "PRT1DM1",       // 0x05
  "PRT1IC0",       // 0x06
  "PRT1IC1",       // 0x07
  "PRT2DM0",       // 0x08
  "PRT2DM1",       // 0x09
  "PRT2IC0",       // 0x0a
  "PRT2IC1",       // 0x0b
  "PRT3DM0",       // 0x0c
  "PRT3DM1",       // 0x0d
  "PRT3IC0",       // 0x0e
  "PRT3IC1",       // 0x0f
  "PRT4DM0",       // 0x10
  "PRT4DM1",       // 0x11
  "PRT4IC0",       // 0x12
  "PRT4IC1",       // 0x13
  "PRT5DM0",       // 0x14
  "PRT5DM1",       // 0x15
  "PRT5IC0",       // 0x16
  "PRT5IC1",       // 0x17
  "NOT_USED",      // 0x18
  "NOT_USED",      // 0x19
  "NOT_USED",      // 0x1a
  "NOT_USED",      // 0x1b
  "PRT7DM0",       // 0x1c
  "PRT7DM1",       // 0x1d
  "PRT7IC0",       // 0x1e
  "PRT7IC1",       // 0x1f
  "DBB00FN",       // 0x20
  "DBB00IN",       // 0x21
  "DBB00OU",       // 0x22
  "NOT_USED",      // 0x23
  "DBB01FN",       // 0x24
  "DBB01IN",       // 0x25
  "DBB01OU",       // 0x26
  "NOT_USED",      // 0x27
  "DCB02FN",       // 0x28
  "DCB02IN",       // 0x29
  "DCB02OU",       // 0x2a
  "NOT_USED",      // 0x2b
  "DCB03FN",       // 0x2c
  "DCB03IN",       // 0x2d
  "DCB03OU",       // 0x2e
  "NOT_USED",      // 0x2f
  "NOT_USED",      // 0x30
  "NOT_USED",      // 0x31
  "NOT_USED",      // 0x32
  "NOT_USED",      // 0x33
  "NOT_USED",      // 0x34
  "NOT_USED",      // 0x35
  "NOT_USED",      // 0x36
  "NOT_USED",      // 0x37
  "NOT_USED",      // 0x38
  "NOT_USED",      // 0x39
  "NOT_USED",      // 0x3a
  "NOT_USED",      // 0x3b
  "NOT_USED",      // 0x3c
  "NOT_USED",      // 0x3d
  "NOT_USED",      // 0x3e
  "NOT_USED",      // 0x3f
  "PMA0_WA",       // 0x40
  "PMA1_WA",       // 0x41
  "PMA2_WA",       // 0x42
  "PMA3_WA",       // 0x43
  "PMA4_WA",       // 0x44
  "PMA5_WA",       // 0x45
  "PMA6_WA",       // 0x46
  "PMA7_WA",       // 0x47
  "NOT_USED",      // 0x48
  "NOT_USED",      // 0x49
  "NOT_USED",      // 0x4a
  "NOT_USED",      // 0x4b
  "NOT_USED",      // 0x4c
  "NOT_USED",      // 0x4d
  "NOT_USED",      // 0x4e
  "NOT_USED",      // 0x4f
  "PMA0_RA",       // 0x50
  "PMA1_RA",       // 0x51
  "PMA2_RA",       // 0x52
  "PMA3_RA",       // 0x53
  "PMA4_RA",       // 0x54
  "PMA5_RA",       // 0x55
  "PMA6_RA",       // 0x56
  "PMA7_RA",       // 0x57
  "NOT_USED",      // 0x58
  "NOT_USED",      // 0x59
  "NOT_USED",      // 0x5a
  "NOT_USED",      // 0x5b
  "NOT_USED",      // 0x5c
  "NOT_USED",      // 0x5d
  "NOT_USED",      // 0x5e
  "NOT_USED",      // 0x5f
  "CLK_CR0",       // 0x60
  "CLK_CR1",       // 0x61
  "ABF_CR0",       // 0x62
  "AMD_CR0",       // 0x63
  "CMP_GO_EN",     // 0x64
  "NOT_USED",      // 0x65
  "AMD_CR1",       // 0x66
  "ALT_CR0",       // 0x67
  "NOT_USED",      // 0x68
  "NOT_USED",      // 0x69
  "NOT_USED",      // 0x6a
  "NOT_USED",      // 0x6b
  "TMP_DR0",       // 0x6c
  "TMP_DR1",       // 0x6d
  "TMP_DR2",       // 0x6e
  "TMP_DR3",       // 0x6f
  "ACB00CR3",      // 0x70
  "ACB00CR0",      // 0x71
  "ACB00CR1",      // 0x72
  "ACB00CR2",      // 0x73
  "ACB01CR3",      // 0x74
  "ACB01CR0",      // 0x75
  "ACB01CR1",      // 0x76
  "ACB01CR2",      // 0x77
  "NOT_USED",      // 0x78
  "NOT_USED",      // 0x79
  "NOT_USED",      // 0x7a
  "NOT_USED",      // 0x7b
  "NOT_USED",      // 0x7c
  "NOT_USED",      // 0x7d
  "NOT_USED",      // 0x7e
  "NOT_USED",      // 0x7f
  "ASC10CR0",      // 0x80
  "ASC10CR1",      // 0x81
  "ASC10CR2",      // 0x82
  "ASC10CR3",      // 0x83
  "ASD11CR0",      // 0x84
  "ASD11CR1",      // 0x85
  "ASD11CR2",      // 0x86
  "ASD11CR3",      // 0x87
  "NOT_USED",      // 0x88
  "NOT_USED",      // 0x89
  "NOT_USED",      // 0x8a
  "NOT_USED",      // 0x8b
  "NOT_USED",      // 0x8c
  "NOT_USED",      // 0x8d
  "NOT_USED",      // 0x8e
  "NOT_USED",      // 0x8f
  "NOT_USED",      // 0x90
  "ASD20CR1",      // 0x91
  "ASD20CR2",      // 0x92
  "ASD20CR3",      // 0x93
  "ASC21CR0",      // 0x94
  "ASC21CR1",      // 0x95
  "ASC21CR2",      // 0x96
  "ASC21CR3",      // 0x97
  "NOT_USED",      // 0x98
  "NOT_USED",      // 0x99
  "NOT_USED",      // 0x9a
  "NOT_USED",      // 0x9b
  "NOT_USED",      // 0x9c
  "NOT_USED",      // 0x9d
  "NOT_USED",      // 0x9e
  "NOT_USED",      // 0x9f
  "NOT_USED",      // 0xa0
  "NOT_USED",      // 0xa1
  "NOT_USED",      // 0xa2
  "NOT_USED",      // 0xa3
  "NOT_USED",      // 0xa4
  "NOT_USED",      // 0xa5
  "NOT_USED",      // 0xa6
  "NOT_USED",      // 0xa7
  "NOT_USED",      // 0xa8
  "NOT_USED",      // 0xa9
  "NOT_USED",      // 0xaa
  "NOT_USED",      // 0xab
  "NOT_USED",      // 0xac
  "NOT_USED",      // 0xad
  "NOT_USED",      // 0xae
  "NOT_USED",      // 0xaf
  "RDI0RI",        // 0xb0
  "RDI0SYN",       // 0xb1
  "RDI0IS",        // 0xb2
  "RDI0LT0",       // 0xb3
  "RDI0LT1",       // 0xb4
  "RDI0RO0",       // 0xb5
  "RDI0RO1",       // 0xb6
  "NOT_USED",      // 0xb7
  "NOT_USED",      // 0xb8
  "NOT_USED",      // 0xb9
  "NOT_USED",      // 0xba
  "NOT_USED",      // 0xbb
  "NOT_USED",      // 0xbc
  "NOT_USED",      // 0xbd
  "NOT_USED",      // 0xbe
  "NOT_USED",      // 0xbf
  "USBI/O_CR2",    // 0xc0
  "USB_CR1",       // 0xc1
  "NOT_USED",      // 0xc2
  "NOT_USED",      // 0xc3
  "EP1_CR0",       // 0xc4
  "EP2_CR0",       // 0xc5
  "EP3_CR0",       // 0xc6
  "EP4_CR0",       // 0xc7
  "NOT_USED",      // 0xc8
  "NOT_USED",      // 0xc9
  "NOT_USED",      // 0xca
  "NOT_USED",      // 0xcb
  "NOT_USED",      // 0xcc
  "NOT_USED",      // 0xcd
  "NOT_USED",      // 0xce
  "NOT_USED",      // 0xcf
  "GDI_O_IN",      // 0xd0
  "GDI_E_IN",      // 0xd1
  "GDI_O_OU",      // 0xd2
  "GDI_E_OU",      // 0xd3
  "NOT_USED",      // 0xd4
  "NOT_USED",      // 0xd5
  "NOT_USED",      // 0xd6
  "NOT_USED",      // 0xd7
  "MUX_CR0",       // 0xd8
  "MUX_CR1",       // 0xd9
  "MUX_CR2",       // 0xda
  "MUX_CR3",       // 0xdb
  "NOT_USED",      // 0xdc
  "OSC_GO_EN",     // 0xdd
  "OSC_CR4",       // 0xde
  "OSC_CR3",       // 0xdf
  "OSC_CR0",       // 0xe0
  "OSC_CR1",       // 0xe1
  "OSC_CR2",       // 0xe2
  "VLT_CR",        // 0xe3
  "VLT_CMP",       // 0xe4
  "NOT_USED",      // 0xe5
  "NOT_USED",      // 0xe6
  "NOT_USED",      // 0xe7
  "IMO_TR",        // 0xe8
  "ILO_TR",        // 0xe9
  "BDG_TR",        // 0xea
  "ECO_TR",        // 0xeb
  "MUX_CR4",       // 0xec
  "MUX_CR5",       // 0xed
  "NOT_USED",      // 0xee
  "NOT_USED",      // 0xef
  "NOT_USED",      // 0xf0
  "NOT_USED",      // 0xf1
  "NOT_USED",      // 0xf2
  "NOT_USED",      // 0xf3
  "NOT_USED",      // 0xf4
  "NOT_USED",      // 0xf5
  "NOT_USED",      // 0xf6
  "CPU_F",         // 0xf7
  "NOT_USED",      // 0xf8
  "NOT_USED",      // 0xf9
  "NOT_USED",      // 0xfa
  "NOT_USED",      // 0xfb
  "NOT_USED",      // 0xfc
  "DAC_CR",        // 0xfd
  "CPU_SCR1",      // 0xfe
  "CPU_SCR0",      // 0xff
};

static symbol_t cy8c24894SymbolsSROM [] = 
{
  { "SROM_Key1",      0xf8, NULL },
  { "SROM_Key2",      0xf9, NULL },
  { "SROM_BlockID",   0xfa, NULL },
  { "SROM_Pointer",   0xfb, NULL },
  { "SROM_Clock",     0xfc, NULL },
  { "SROM_Reserved1", 0xfd, NULL },
  { "SROM_DELAY",     0xfe, NULL },
  { "SROM_Reserved",  0xff, NULL },
  { NULL,             0x00, NULL }
};

static symbol_t cy8c24894SymbolsIntVectors [] =
{
  { "IntVec_Reset",      0x0000, NULL }, 
  { "IntVec_SVM",        0x0004, NULL }, 
  { "IntVec_AC0",        0x0008, NULL }, 
  { "IntVec_AC1",        0x000c, NULL }, 
  { "IntVec_VC3",        0x0018, NULL }, 
  { "IntVec_GPIo",       0x001c, NULL }, 
  { "IntVec_DBB00",      0x0020, NULL }, 
  { "IntVec_DBB01",      0x0024, NULL }, 
  { "IntVec_DCB02",      0x0028, NULL }, 
  { "IntVec_DCB03",      0x002c, NULL }, 
  { "IntVec_USB_Reset",  0x0040, NULL }, 
  { "IntVec_USB_SOF",    0x0044, NULL }, 
  { "IntVec_USB_EP0",    0x0048, NULL }, 
  { "IntVec_USB_EP1",    0x004c, NULL }, 
  { "IntVec_USB_EP2",    0x0050, NULL }, 
  { "IntVec_USB_EP3",    0x0054, NULL }, 
  { "IntVec_USB_EP4",    0x0058, NULL }, 
  { "IntVec_USB_Wakeup", 0x005c, NULL }, 
  { "IntVec_I2C",        0x0060, NULL }, 
  { "IntVec_Sleep",      0x0064, NULL }, 
  { NULL,                0x0000, NULL }
};

static mapfileExtraSymbols_t cy8c24894ExtraSymbols [] = 
{
  { "SROMRegisters", 0x00f8, 0x0008, MEMTYPE_RAM, ' ', cy8c24894SymbolsSROM },
  { "IntVectors",    0x0000, 0x0068, MEMTYPE_ROM, 'I', cy8c24894SymbolsIntVectors },
};

static processor_t processorCY8C24894 = 
{
  .name = "cy8c24894",
  .flash = 16384,
  .ram = 1024,
  .ioPins = 56,
  .digitalRows = 1,
  .digitalBlocks = 4,
  .analogPins = 48,
  .analogOutputs = 2,
  .analogColumns = 2,
  .analogBlocks = 6,
  .hasMAC = 1,
  .extraSymbols = cy8c24894ExtraSymbols,
  .extraSymbolsSize = arrsizeof (cy8c24894ExtraSymbols),
  .user = cy8c24894User,
  .config = cy8c24894Config,
};

static char *cy7c63923User [] = 
{
  "P0DATA",        // 0x00
  "P1DATA",        // 0x01
  "P2DATA",        // 0x02
  "P3DATA",        // 0x03
  "P4DATA",        // 0x04
  "P00CR",         // 0x05
  "P01CR",         // 0x06
  "P02CR",         // 0x07
  "P03CR",         // 0x08
  "P04CR",         // 0x09
  "P05CR",         // 0x0a
  "P06CR",         // 0x0b
  "P07CR",         // 0x0c
  "P10CR",         // 0x0d
  "P11CR",         // 0x0e
  "P12CR",         // 0x0f
  "P13CR",         // 0x10
  "P14CR",         // 0x11
  "P15CR",         // 0x12
  "P16CR",         // 0x13
  "P17CR",         // 0x14
  "P2CR",          // 0x15
  "P3CR",          // 0x16
  "P4CR",          // 0x17
  "NOT_USED",      // 0x18
  "NOT_USED",      // 0x19
  "NOT_USED",      // 0x1a
  "NOT_USED",      // 0x1b
  "NOT_USED",      // 0x1c
  "NOT_USED",      // 0x1d
  "NOT_USED",      // 0x1e
  "NOT_USED",      // 0x1f
  "FRTMRL",        // 0x20
  "FRTMRH",        // 0x21
  "TCAP0R",        // 0x22
  "TCAP1R",        // 0x23
  "TCAP0F",        // 0x24
  "TCAP1F",        // 0x25
  "PITMRL",        // 0x26
  "PITMRH",        // 0x27
  "PIRL",          // 0x28
  "PIRH",          // 0x29
  "TMRCR",         // 0x2a
  "TCAPINTE",      // 0x2b
  "TCAPINTS",      // 0x2c
  "NOT_USED",      // 0x2d
  "NOT_USED",      // 0x2e
  "NOT_USED",      // 0x2f
  "CPUCLKCR",      // 0x30
  "ITMRCLKCR",     // 0x31
  "CLKIOCR",       // 0x32
  "NOT_USED",      // 0x33
  "IOSCTR",        // 0x34
  "XOSCTR",        // 0x35
  "NOT_USED",      // 0x36
  "NOT_USED",      // 0x37
  "NOT_USED",      // 0x38
  "OSCLCKCR",      // 0x39
  "NOT_USED",      // 0x3a
  "NOT_USED",      // 0x3b
  "SPIDATA",       // 0x3c
  "SPICR",         // 0x3d
  "NOT_USED",      // 0x3e
  "NOT_USED",      // 0x3f
  "USBCR",         // 0x40
  "EP0CNT",        // 0x41
  "EP1CNT",        // 0x42
  "EP2CNT",        // 0x43
  "EP0MODE",       // 0x44
  "EP1MODE",       // 0x45
  "EP2MODE",       // 0x46
  "NOT_USED",      // 0x47
  "NOT_USED",      // 0x48
  "NOT_USED",      // 0x49
  "NOT_USED",      // 0x4a
  "NOT_USED",      // 0x4b
  "NOT_USED",      // 0x4c
  "NOT_USED",      // 0x4d
  "NOT_USED",      // 0x4e
  "NOT_USED",      // 0x4f
  "EP0DATA:0",     // 0x50
  "EP0DATA:1",     // 0x51
  "EP0DATA:2",     // 0x52
  "EP0DATA:3",     // 0x53
  "EP0DATA:4",     // 0x54
  "EP0DATA:5",     // 0x55
  "EP0DATA:6",     // 0x56
  "EP0DATA:7",     // 0x57
  "EP1DATA:0",     // 0x58
  "EP1DATA:1",     // 0x59
  "EP1DATA:2",     // 0x5a
  "EP1DATA:3",     // 0x5b
  "EP1DATA:4",     // 0x5c
  "EP1DATA:5",     // 0x5d
  "EP1DATA:6",     // 0x5e
  "EP1DATA:7",     // 0x5f
  "EP2DATA:0",     // 0x60
  "EP2DATA:1",     // 0x61
  "EP2DATA:2",     // 0x62
  "EP2DATA:3",     // 0x63
  "EP2DATA:4",     // 0x64
  "EP2DATA:5",     // 0x65
  "EP2DATA:6",     // 0x66
  "EP2DATA:7",     // 0x67
  "NOT_USED",      // 0x68
  "NOT_USED",      // 0x69
  "NOT_USED",      // 0x6a
  "NOT_USED",      // 0x6b
  "NOT_USED",      // 0x6c
  "NOT_USED",      // 0x6d
  "NOT_USED",      // 0x6e
  "NOT_USED",      // 0x6f
  "NOT_USED",      // 0x70
  "NOT_USED",      // 0x71
  "NOT_USED",      // 0x72
  "VREGCR",        // 0x73
  "USBXCR",        // 0x74
  "NOT_USED",      // 0x75
  "NOT_USED",      // 0x76
  "NOT_USED",      // 0x77
  "NOT_USED",      // 0x78
  "NOT_USED",      // 0x79
  "NOT_USED",      // 0x7a
  "NOT_USED",      // 0x7b
  "NOT_USED",      // 0x7c
  "NOT_USED",      // 0x7d
  "NOT_USED",      // 0x7e
  "NOT_USED",      // 0x7f
  "NOT_USED",      // 0x80
  "NOT_USED",      // 0x81
  "NOT_USED",      // 0x82
  "NOT_USED",      // 0x83
  "NOT_USED",      // 0x84
  "NOT_USED",      // 0x85
  "NOT_USED",      // 0x86
  "NOT_USED",      // 0x87
  "NOT_USED",      // 0x88
  "NOT_USED",      // 0x89
  "NOT_USED",      // 0x8a
  "NOT_USED",      // 0x8b
  "NOT_USED",      // 0x8c
  "NOT_USED",      // 0x8d
  "NOT_USED",      // 0x8e
  "NOT_USED",      // 0x8f
  "NOT_USED",      // 0x90
  "NOT_USED",      // 0x91
  "NOT_USED",      // 0x92
  "NOT_USED",      // 0x93
  "NOT_USED",      // 0x94
  "NOT_USED",      // 0x95
  "NOT_USED",      // 0x96
  "NOT_USED",      // 0x97
  "NOT_USED",      // 0x98
  "NOT_USED",      // 0x99
  "NOT_USED",      // 0x9a
  "NOT_USED",      // 0x9b
  "NOT_USED",      // 0x9c
  "NOT_USED",      // 0x9d
  "NOT_USED",      // 0x9e
  "NOT_USED",      // 0x9f
  "NOT_USED",      // 0xa0
  "NOT_USED",      // 0xa1
  "NOT_USED",      // 0xa2
  "NOT_USED",      // 0xa3
  "NOT_USED",      // 0xa4
  "NOT_USED",      // 0xa5
  "NOT_USED",      // 0xa6
  "NOT_USED",      // 0xa7
  "NOT_USED",      // 0xa8
  "NOT_USED",      // 0xa9
  "NOT_USED",      // 0xaa
  "NOT_USED",      // 0xab
  "NOT_USED",      // 0xac
  "NOT_USED",      // 0xad
  "NOT_USED",      // 0xae
  "NOT_USED",      // 0xaf
  "NOT_USED",      // 0xb0
  "NOT_USED",      // 0xb1
  "NOT_USED",      // 0xb2
  "NOT_USED",      // 0xb3
  "NOT_USED",      // 0xb4
  "NOT_USED",      // 0xb5
  "NOT_USED",      // 0xb6
  "NOT_USED",      // 0xb7
  "NOT_USED",      // 0xb8
  "NOT_USED",      // 0xb9
  "NOT_USED",      // 0xba
  "NOT_USED",      // 0xbb
  "NOT_USED",      // 0xbc
  "NOT_USED",      // 0xbd
  "NOT_USED",      // 0xbe
  "NOT_USED",      // 0xbf
  "NOT_USED",      // 0xc0
  "NOT_USED",      // 0xc1
  "NOT_USED",      // 0xc2
  "NOT_USED",      // 0xc3
  "NOT_USED",      // 0xc4
  "NOT_USED",      // 0xc5
  "NOT_USED",      // 0xc6
  "NOT_USED",      // 0xc7
  "NOT_USED",      // 0xc8
  "NOT_USED",      // 0xc9
  "NOT_USED",      // 0xca
  "NOT_USED",      // 0xcb
  "NOT_USED",      // 0xcc
  "NOT_USED",      // 0xcd
  "NOT_USED",      // 0xce
  "NOT_USED",      // 0xcf
  "NOT_USED",      // 0xd0
  "NOT_USED",      // 0xd1
  "NOT_USED",      // 0xd2
  "NOT_USED",      // 0xd3
  "NOT_USED",      // 0xd4
  "NOT_USED",      // 0xd5
  "NOT_USED",      // 0xd6
  "NOT_USED",      // 0xd7
  "NOT_USED",      // 0xd8
  "NOT_USED",      // 0xd9
  "INT_CLR0",      // 0xda
  "INT_CLR1",      // 0xdb
  "INT_CLR2",      // 0xdc
  "NOT_USED",      // 0xdd
  "INT_MSK3",      // 0xde
  "INT_MSK2",      // 0xdf
  "INT_MSK1",      // 0xe0
  "INT_MSK0",      // 0xe1
  "INT_VC",        // 0xe2
  "RESWDT",        // 0xe3
  "NOT_USED",      // 0xe4
  "NOT_USED",      // 0xe5
  "NOT_USED",      // 0xe6
  "NOT_USED",      // 0xe7
  "NOT_USED",      // 0xe8
  "NOT_USED",      // 0xe9
  "NOT_USED",      // 0xea
  "NOT_USED",      // 0xeb
  "NOT_USED",      // 0xec
  "NOT_USED",      // 0xed
  "NOT_USED",      // 0xee
  "NOT_USED",      // 0xef
  "NOT_USED",      // 0xf0
  "NOT_USED",      // 0xf1
  "NOT_USED",      // 0xf2
  "NOT_USED",      // 0xf3
  "NOT_USED",      // 0xf4
  "NOT_USED",      // 0xf5
  "NOT_USED",      // 0xf6
  "NOT_USED",      // 0xf7
  "NOT_USED",      // 0xf8
  "NOT_USED",      // 0xf9
  "NOT_USED",      // 0xfa
  "NOT_USED",      // 0xfb
  "NOT_USED",      // 0xfc
  "NOT_USED",      // 0xfd
  "NOT_USED",      // 0xfe
  "CPU_SCR",       // 0xff
};

static char *cy7c63923Config [] =
{
  "NOT_USED",      // 0x00
  "NOT_USED",      // 0x01
  "NOT_USED",      // 0x02
  "NOT_USED",      // 0x03
  "NOT_USED",      // 0x04
  "NOT_USED",      // 0x05
  "NOT_USED",      // 0x06
  "NOT_USED",      // 0x07
  "NOT_USED",      // 0x08
  "NOT_USED",      // 0x09
  "NOT_USED",      // 0x0a
  "NOT_USED",      // 0x0b
  "NOT_USED",      // 0x0c
  "NOT_USED",      // 0x0d
  "NOT_USED",      // 0x0e
  "NOT_USED",      // 0x0f
  "NOT_USED",      // 0x10
  "NOT_USED",      // 0x11
  "NOT_USED",      // 0x12
  "NOT_USED",      // 0x13
  "NOT_USED",      // 0x14
  "NOT_USED",      // 0x15
  "NOT_USED",      // 0x16
  "NOT_USED",      // 0x17
  "NOT_USED",      // 0x18
  "NOT_USED",      // 0x19
  "NOT_USED",      // 0x1a
  "NOT_USED",      // 0x1b
  "NOT_USED",      // 0x1c
  "NOT_USED",      // 0x1d
  "NOT_USED",      // 0x1e
  "NOT_USED",      // 0x1f
  "NOT_USED",      // 0x20
  "NOT_USED",      // 0x21
  "NOT_USED",      // 0x22
  "NOT_USED",      // 0x23
  "NOT_USED",      // 0x24
  "NOT_USED",      // 0x25
  "NOT_USED",      // 0x26
  "NOT_USED",      // 0x27
  "NOT_USED",      // 0x28
  "NOT_USED",      // 0x29
  "NOT_USED",      // 0x2a
  "NOT_USED",      // 0x2b
  "NOT_USED",      // 0x2c
  "NOT_USED",      // 0x2d
  "NOT_USED",      // 0x2e
  "NOT_USED",      // 0x2f
  "NOT_USED",      // 0x30
  "NOT_USED",      // 0x31
  "NOT_USED",      // 0x32
  "NOT_USED",      // 0x33
  "NOT_USED",      // 0x34
  "NOT_USED",      // 0x35
  "NOT_USED",      // 0x36
  "NOT_USED",      // 0x37
  "NOT_USED",      // 0x38
  "NOT_USED",      // 0x39
  "NOT_USED",      // 0x3a
  "NOT_USED",      // 0x3b
  "NOT_USED",      // 0x3c
  "NOT_USED",      // 0x3d
  "NOT_USED",      // 0x3e
  "NOT_USED",      // 0x3f
  "NOT_USED",      // 0x40
  "NOT_USED",      // 0x41
  "NOT_USED",      // 0x42
  "NOT_USED",      // 0x43
  "NOT_USED",      // 0x44
  "NOT_USED",      // 0x45
  "NOT_USED",      // 0x46
  "NOT_USED",      // 0x47
  "NOT_USED",      // 0x48
  "NOT_USED",      // 0x49
  "NOT_USED",      // 0x4a
  "NOT_USED",      // 0x4b
  "NOT_USED",      // 0x4c
  "NOT_USED",      // 0x4d
  "NOT_USED",      // 0x4e
  "NOT_USED",      // 0x4f
  "NOT_USED",      // 0x50
  "NOT_USED",      // 0x51
  "NOT_USED",      // 0x52
  "NOT_USED",      // 0x53
  "NOT_USED",      // 0x54
  "NOT_USED",      // 0x55
  "NOT_USED",      // 0x56
  "NOT_USED",      // 0x57
  "NOT_USED",      // 0x58
  "NOT_USED",      // 0x59
  "NOT_USED",      // 0x5a
  "NOT_USED",      // 0x5b
  "NOT_USED",      // 0x5c
  "NOT_USED",      // 0x5d
  "NOT_USED",      // 0x5e
  "NOT_USED",      // 0x5f
  "NOT_USED",      // 0x60
  "NOT_USED",      // 0x61
  "NOT_USED",      // 0x62
  "NOT_USED",      // 0x63
  "NOT_USED",      // 0x64
  "NOT_USED",      // 0x65
  "NOT_USED",      // 0x66
  "NOT_USED",      // 0x67
  "NOT_USED",      // 0x68
  "NOT_USED",      // 0x69
  "NOT_USED",      // 0x6a
  "NOT_USED",      // 0x6b
  "NOT_USED",      // 0x6c
  "NOT_USED",      // 0x6d
  "NOT_USED",      // 0x6e
  "NOT_USED",      // 0x6f
  "NOT_USED",      // 0x70
  "NOT_USED",      // 0x71
  "NOT_USED",      // 0x72
  "NOT_USED",      // 0x73
  "NOT_USED",      // 0x74
  "NOT_USED",      // 0x75
  "NOT_USED",      // 0x76
  "NOT_USED",      // 0x77
  "NOT_USED",      // 0x78
  "NOT_USED",      // 0x79
  "NOT_USED",      // 0x7a
  "NOT_USED",      // 0x7b
  "NOT_USED",      // 0x7c
  "NOT_USED",      // 0x7d
  "NOT_USED",      // 0x7e
  "NOT_USED",      // 0x7f
  "NOT_USED",      // 0x80
  "NOT_USED",      // 0x81
  "NOT_USED",      // 0x82
  "NOT_USED",      // 0x83
  "NOT_USED",      // 0x84
  "NOT_USED",      // 0x85
  "NOT_USED",      // 0x86
  "NOT_USED",      // 0x87
  "NOT_USED",      // 0x88
  "NOT_USED",      // 0x89
  "NOT_USED",      // 0x8a
  "NOT_USED",      // 0x8b
  "NOT_USED",      // 0x8c
  "NOT_USED",      // 0x8d
  "NOT_USED",      // 0x8e
  "NOT_USED",      // 0x8f
  "NOT_USED",      // 0x90
  "NOT_USED",      // 0x91
  "NOT_USED",      // 0x92
  "NOT_USED",      // 0x93
  "NOT_USED",      // 0x94
  "NOT_USED",      // 0x95
  "NOT_USED",      // 0x96
  "NOT_USED",      // 0x97
  "NOT_USED",      // 0x98
  "NOT_USED",      // 0x99
  "NOT_USED",      // 0x9a
  "NOT_USED",      // 0x9b
  "NOT_USED",      // 0x9c
  "NOT_USED",      // 0x9d
  "NOT_USED",      // 0x9e
  "NOT_USED",      // 0x9f
  "NOT_USED",      // 0xa0
  "NOT_USED",      // 0xa1
  "NOT_USED",      // 0xa2
  "NOT_USED",      // 0xa3
  "NOT_USED",      // 0xa4
  "NOT_USED",      // 0xa5
  "NOT_USED",      // 0xa6
  "NOT_USED",      // 0xa7
  "NOT_USED",      // 0xa8
  "NOT_USED",      // 0xa9
  "NOT_USED",      // 0xaa
  "NOT_USED",      // 0xab
  "NOT_USED",      // 0xac
  "NOT_USED",      // 0xad
  "NOT_USED",      // 0xae
  "NOT_USED",      // 0xaf
  "NOT_USED",      // 0xb0
  "NOT_USED",      // 0xb1
  "NOT_USED",      // 0xb2
  "NOT_USED",      // 0xb3
  "NOT_USED",      // 0xb4
  "NOT_USED",      // 0xb5
  "NOT_USED",      // 0xb6
  "NOT_USED",      // 0xb7
  "NOT_USED",      // 0xb8
  "NOT_USED",      // 0xb9
  "NOT_USED",      // 0xba
  "NOT_USED",      // 0xbb
  "NOT_USED",      // 0xbc
  "NOT_USED",      // 0xbd
  "NOT_USED",      // 0xbe
  "NOT_USED",      // 0xbf
  "NOT_USED",      // 0xc0
  "NOT_USED",      // 0xc1
  "NOT_USED",      // 0xc2
  "NOT_USED",      // 0xc3
  "NOT_USED",      // 0xc4
  "NOT_USED",      // 0xc5
  "NOT_USED",      // 0xc6
  "NOT_USED",      // 0xc7
  "NOT_USED",      // 0xc8
  "NOT_USED",      // 0xc9
  "NOT_USED",      // 0xca
  "NOT_USED",      // 0xcb
  "NOT_USED",      // 0xcc
  "NOT_USED",      // 0xcd
  "NOT_USED",      // 0xce
  "NOT_USED",      // 0xcf
  "NOT_USED",      // 0xd0
  "NOT_USED",      // 0xd1
  "NOT_USED",      // 0xd2
  "NOT_USED",      // 0xd3
  "NOT_USED",      // 0xd4
  "NOT_USED",      // 0xd5
  "NOT_USED",      // 0xd6
  "NOT_USED",      // 0xd7
  "NOT_USED",      // 0xd8
  "NOT_USED",      // 0xd9
  "NOT_USED",      // 0xda
  "NOT_USED",      // 0xdb
  "NOT_USED",      // 0xdc
  "NOT_USED",      // 0xdd
  "NOT_USED",      // 0xde
  "NOT_USED",      // 0xdf
  "OSC_CR0",       // 0xe0
  "NOT_USED",      // 0xe1
  "NOT_USED",      // 0xe2
  "LVDCR",         // 0xe3
  "VLTCMP",        // 0xe4
  "NOT_USED",      // 0xe5
  "NOT_USED",      // 0xe6
  "NOT_USED",      // 0xe7
  "NOT_USED",      // 0xe8
  "NOT_USED",      // 0xe9
  "NOT_USED",      // 0xea
  "ECO_TR",        // 0xeb
  "NOT_USED",      // 0xec
  "NOT_USED",      // 0xed
  "NOT_USED",      // 0xee
  "NOT_USED",      // 0xef
  "NOT_USED",      // 0xf0
  "NOT_USED",      // 0xf1
  "NOT_USED",      // 0xf2
  "NOT_USED",      // 0xf3
  "NOT_USED",      // 0xf4
  "NOT_USED",      // 0xf5
  "NOT_USED",      // 0xf6
  "NOT_USED",      // 0xf7
  "NOT_USED",      // 0xf8
  "NOT_USED",      // 0xf9
  "NOT_USED",      // 0xfa
  "NOT_USED",      // 0xfb
  "NOT_USED",      // 0xfc
  "NOT_USED",      // 0xfd
  "NOT_USED",      // 0xfe
  "NOT_USED",      // 0xff
};

static symbol_t cy7c63923SymbolsSROM [] = 
{
  { "SROM_Key1",      0xf8, NULL },
  { "SROM_Key2",      0xf9, NULL },
  { "SROM_BlockID",   0xfa, NULL },
  { "SROM_Pointer",   0xfb, NULL },
  { "SROM_Clock",     0xfc, NULL },
  { "SROM_Mode",      0xfd, NULL },
  { "SROM_Delay",     0xfe, NULL },
  { "SROM_PCL",       0xff, NULL },
  { NULL,             0x00, NULL }
};

static symbol_t cy7c63923SymbolsIntVectors [] =
{
  { "IntVec_Reset",      0x0000, NULL }, 
  { "IntVec_POR_LVD",    0x0004, NULL }, 
  { "IntVec_INT0",       0x0008, NULL }, 
  { "IntVec_SPI_Empty",  0x000c, NULL }, 
  { "IntVec_SPI_Full",   0x0010, NULL }, 
  { "IntVec_GPIO0",      0x0014, NULL }, 
  { "IntVec_GPIO1",      0x0018, NULL }, 
  { "IntVec_INT1",       0x001c, NULL }, 
  { "IntVec_USB_EP0",    0x0020, NULL }, 
  { "IntVec_USB_EP1",    0x0024, NULL }, 
  { "IntVec_USB_EP2",    0x0028, NULL }, 
  { "IntVec_USB_Reset",  0x002c, NULL }, 
  { "IntVec_USB_Active", 0x0030, NULL }, 
  { "IntVec_MsecTimer",  0x0034, NULL }, 
  { "IntVec_ProgTimer",  0x0038, NULL }, 
  { "IntVec_TimerCap0",  0x003c, NULL }, 
  { "IntVec_TimerCap1",  0x0040, NULL }, 
  { "IntVec_TimerWrap",  0x0044, NULL }, 
  { "IntVec_INT2",       0x0048, NULL }, 
  { "IntVec_PS2DataLow", 0x004c, NULL }, 
  { "IntVec_GPIO2",      0x0050, NULL }, 
  { "IntVec_GPIO3",      0x0054, NULL }, 
  { "IntVec_GPIO4",      0x0058, NULL }, 
  { "IntVec_Sleep",      0x0064, NULL }, 
  { NULL,                0x0000, NULL }
};

static mapfileExtraSymbols_t cy7c63923ExtraSymbols [] = 
{
  { "SROMRegisters", 0x00f8, 0x0008, MEMTYPE_RAM, ' ', cy7c63923SymbolsSROM },
  { "IntVectors",    0x0000, 0x0068, MEMTYPE_ROM, 'I', cy7c63923SymbolsIntVectors },
};

static processor_t processorCY7C63923 =
{
  .name = "cy7c63923",
  .flash = 8192,
  .ram = 256,
  .ioPins = 36,
  .digitalRows = 1,
  .digitalBlocks = 4,
  .analogPins = 0, // ??
  .analogOutputs = 0, // ??
  .analogColumns = 0, // ??
  .analogBlocks = 0, // ??
  .hasMAC = 0, // ??
  .extraSymbols = cy7c63923ExtraSymbols,
  .extraSymbolsSize = arrsizeof (cy7c63923ExtraSymbols),
  .user = cy7c63923User,
  .config = cy7c63923Config,
};

//
//
//
static processor_t *processors [] = 
{
  &processorCY8C24894,
  &processorCY7C63923,
  NULL
};

//
//
//
processor_t *processorsSelect (char *processorName)
{
  unsigned int i;

  for (i = 0; processors [i]; i++)
    if (!strcasecmp (processors [i]->name, processorName))
      return processors [i];

  return NULL;
}

processor_t **processorsGetList (void)
{
  return processors;
}
