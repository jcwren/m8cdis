//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "m8cdis.h"
#include "mapfile.h"
#include "processors.h"
#include "disassem.h"

//
//
//
typedef enum
{
  PGMODE_PG0_PG0 = 0,   // Direct access mode referred to page 0, index address mode referred to page 0
  PGMODE_PG0_STK,       // Direct access mode referred to page 0, index address mode referred to STK_PP page
  PGMODE_CUR_IDX,       // Direct access mode referred to CUR_PP page, index address mode referred to IDX_PP page
  PGMODE_CUR_STK,       // Direct access mode referred to CUR_PP page, index address mode referred to STK_PP page
}
pgMode_e;

//
//
//
typedef union PSW_s
{
  struct
  {
    unsigned int gie     : 1;
    unsigned int zero    : 1;
    unsigned int carry   : 1;
    unsigned int notUsed : 1;
    unsigned int xio     : 1;
    unsigned int binc    : 1;
    unsigned int pgMode  : 2;
  } bits ;

  unsigned char byte;
}
__attribute__ ((packed)) PSW_t;

typedef struct instruction_s
{
  char *text;
  int cycles;
  int insLength;
  int opcode;
  int (*disassemHandler) (unsigned char *memory, int pc, struct instruction_s *ins);
}
instruction_t;

//
//
//
static int disassemHandlerNoOperand (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_srcIMM (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_srcDIR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_srcIDX (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstDIR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstIDX (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstDIR_srcIMM (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstIDX_srcIMM (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstDIR_srcDIR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_srcINDPI (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandler_dstINDPI (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_srcDIR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_srcIDX (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_dstDIR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_dstIDX (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_dstDIR_srcIMM (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_dstDIR_srcIMM_MOV (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerREG_dstIDX_srcIMM (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerPSW_AND (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerPSW_OR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerPSW_XOR (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerRET (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerAbs16 (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerRelJump (unsigned char *memory, int pc, instruction_t *ins);
static int disassemHandlerRelCall (unsigned char *memory, int pc, instruction_t *ins);

//
//
//
static instruction_t instructions [] = 
{
  { "ssc",                        15, 1, 0x00, disassemHandlerNoOperand },
  { "add   A,%03xh",               4, 2, 0x01, disassemHandler_srcIMM },
  { "add   A,[%s]",                6, 2, 0x02, disassemHandler_srcDIR },
  { "add   A,[X+%s]",              7, 2, 0x03, disassemHandler_srcIDX },
  { "add   [%s],A",                7, 2, 0x04, disassemHandler_dstDIR },
  { "add   [X+%s],A",              8, 2, 0x05, disassemHandler_dstIDX },
  { "add   [%s],%03xh",            9, 3, 0x06, disassemHandler_dstDIR_srcIMM },
  { "add   [X+%s],%03xh",         10, 3, 0x07, disassemHandler_dstIDX_srcIMM },
  { "push  A",                     4, 1, 0x08, disassemHandlerNoOperand },

  { "adc   A,%03xh",               4, 2, 0x09, disassemHandler_srcIMM },
  { "adc   A,[%s]",                6, 2, 0x0a, disassemHandler_srcDIR },
  { "adc   A,[X+%s]",              7, 2, 0x0b, disassemHandler_srcIDX },
  { "adc   [%s],A",                7, 2, 0x0c, disassemHandler_dstDIR },
  { "adc   [X+%s],A",              8, 2, 0x0d, disassemHandler_dstIDX },
  { "adc   [%s],%03xh",            9, 3, 0x0e, disassemHandler_dstDIR_srcIMM },
  { "adc   [X+%s],%03xh",         10, 3, 0x0f, disassemHandler_dstIDX_srcIMM },

  { "push  X",                     4, 1, 0x10, disassemHandlerNoOperand },
  { "sub   A,%03xh",               4, 2, 0x11, disassemHandler_srcIMM },
  { "sub   A,[%s]",                6, 2, 0x12, disassemHandler_srcDIR },
  { "sub   A,[X+%s]",              7, 2, 0x13, disassemHandler_srcIDX },
  { "sub   [%s],A",                7, 2, 0x14, disassemHandler_dstDIR },
  { "sub   [%s],A",                8, 2, 0x15, disassemHandler_dstIDX },
  { "sub   [%s],%03xh",            9, 3, 0x16, disassemHandler_dstDIR_srcIMM },
  { "sub   [X+%s],%03xh",         10, 3, 0x17, disassemHandler_dstIDX_srcIMM },

  { "pop   A",                     5, 1, 0x18, disassemHandlerNoOperand },
  { "sbb   A,%03xh",               4, 2, 0x19, disassemHandler_srcIMM },
  { "sbb   A,[%s]",                6, 2, 0x1a, disassemHandler_srcDIR },
  { "sbb   A,[X+%s]",              7, 2, 0x1b, disassemHandler_srcIDX },
  { "sbb   [%s],A",                7, 2, 0x1c, disassemHandler_dstDIR },
  { "sbb   [X+%s],A",              8, 2, 0x1d, disassemHandler_dstIDX },
  { "sbb   [%s],%03xh",            9, 3, 0x1e, disassemHandler_dstDIR_srcIMM },
  { "sbb   [X+%s],%03xh",         10, 3, 0x1f, disassemHandler_dstIDX_srcIMM },

  { "pop   X",                     5, 1, 0x20, disassemHandlerNoOperand },
  { "and   A,%03xh",               4, 2, 0x21, disassemHandler_srcIMM },
  { "and   A,[%s]",                6, 2, 0x22, disassemHandler_srcDIR },
  { "and   A,[X+%s]",              7, 2, 0x23, disassemHandler_srcIDX },
  { "and   [%s],A",                7, 2, 0x24, disassemHandler_dstDIR },
  { "and   [X+%s],A",              8, 2, 0x25, disassemHandler_dstIDX },
  { "and   [%s],%03xh",            9, 3, 0x26, disassemHandler_dstDIR_srcIMM },
  { "and   [X+%s],%03xh",         10, 3, 0x27, disassemHandler_dstIDX_srcIMM },

  { "romx",                       11, 1, 0x28, disassemHandlerNoOperand },
  { "or    A,%03xh",               4, 2, 0x29, disassemHandler_srcIMM },
  { "or    A,[%s]",                6, 2, 0x2a, disassemHandler_srcDIR },
  { "or    A,[X+%s]",              7, 2, 0x2b, disassemHandler_srcIDX },
  { "or    [%s],A",                7, 2, 0x2c, disassemHandler_dstDIR },
  { "or    [X+%s],A",              8, 2, 0x2d, disassemHandler_dstIDX },
  { "or    [%s],%03xh",            9, 3, 0x2e, disassemHandler_dstDIR_srcIMM },
  { "or    [X+%s],%03xh",         10, 3, 0x2f, disassemHandler_dstIDX_srcIMM },

  { "halt",                        9, 1, 0x30, disassemHandlerNoOperand },
  { "xor   A,%03xh",               4, 2, 0x31, disassemHandler_srcIMM },
  { "xor   A,[%s]",                6, 2, 0x32, disassemHandler_srcDIR },
  { "xor   A,[X+%s]",              7, 2, 0x33, disassemHandler_srcIDX },
  { "xor   [%s],A",                7, 2, 0x34, disassemHandler_dstDIR },
  { "xor   [X+%s],A",              8, 2, 0x35, disassemHandler_dstIDX },
  { "xor   [%s],%03xh",            9, 3, 0x36, disassemHandler_dstDIR_srcIMM },
  { "xor   [X+%s],%03xh",         10, 3, 0x37, disassemHandler_dstIDX_srcIMM },

  { "add   SP,%03xh",              5, 2, 0x38, disassemHandler_srcIMM },
  { "cmp   A,%03xh",               5, 2, 0x39, disassemHandler_srcIMM },
  { "cmp   A,[%s]",                7, 2, 0x3a, disassemHandler_srcDIR },
  { "cmp   A,[X+%s]",              8, 2, 0x3b, disassemHandler_srcIDX },
  { "cmp   [%s],%03xh",            8, 3, 0x3c, disassemHandler_dstDIR_srcIMM },
  { "cmp   [X+%s],%03xh",          9, 3, 0x3d, disassemHandler_dstIDX_srcIMM },
  { "mvi   A,[[%s]++]",           10, 2, 0x3e, disassemHandler_srcINDPI },
  { "mvi   [[%s]++],A",           10, 2, 0x3f, disassemHandler_dstINDPI },

  { "nop",                         4, 1, 0x40, disassemHandlerNoOperand },
  { "and   reg[%s],%03xh",         9, 3, 0x41, disassemHandlerREG_dstDIR_srcIMM },
  { "and   reg[X+%s],%03xh",      10, 3, 0x42, disassemHandlerREG_dstIDX_srcIMM },
  { "or    reg[%s],%03xh",         9, 3, 0x43, disassemHandlerREG_dstDIR_srcIMM },
  { "or    reg[X+%s],%03xh",      10, 3, 0x44, disassemHandlerREG_dstIDX_srcIMM },
  { "xor   reg[%s],%03xh",         9, 3, 0x45, disassemHandlerREG_dstDIR_srcIMM },
  { "xor   reg[X+%s],%03xh",      10, 3, 0x46, disassemHandlerREG_dstIDX_srcIMM },
  { "tst   [%s],%03xh",            7, 3, 0x47, disassemHandler_dstDIR_srcIMM },

  { "tst   [X+%s],%03xh",          9, 3, 0x48, disassemHandler_dstIDX_srcIMM },
  { "tst   reg[%s],%03xh",         9, 3, 0x49, disassemHandlerREG_dstDIR_srcIMM },
  { "tst   reg[X+%s],%03xh",      10, 3, 0x4a, disassemHandlerREG_dstIDX_srcIMM },
  { "swap  A,X",                   5, 1, 0x4b, disassemHandlerNoOperand },
  { "swap  A,[%s]",                7, 2, 0x4c, disassemHandler_srcDIR },
  { "swap  X,[%s]",                7, 2, 0x4d, disassemHandler_srcDIR },
  { "swap  A,SP",                  5, 1, 0x4e, disassemHandlerNoOperand },
  { "mov   X,SP",                  4, 1, 0x4f, disassemHandlerNoOperand },

  { "mov   A,%03xh",               4, 2, 0x50, disassemHandler_srcIMM },
  { "mov   A,[%s]",                5, 2, 0x51, disassemHandler_srcDIR },
  { "mov   A,[X+%s]",              6, 2, 0x52, disassemHandler_srcDIR },
  { "mov   [%s],A",                5, 2, 0x53, disassemHandler_srcDIR },
  { "mov   [X+%s],A",              6, 2, 0x54, disassemHandler_srcDIR },
  { "mov   [%s],%03xh",            8, 3, 0x55, disassemHandler_dstDIR_srcIMM },
  { "mov   [X+%s],%03xh",          9, 3, 0x56, disassemHandler_dstDIR_srcIMM },
  { "mov   X,%03xh",               4, 2, 0x57, disassemHandler_srcIMM },

  { "mov   X,[%s]",                6, 2, 0x58, disassemHandler_srcDIR },
  { "mov   X,[X+%s]",              7, 2, 0x59, disassemHandler_srcIDX },
  { "mov   [%s],X",                5, 2, 0x5a, disassemHandler_dstDIR },
  { "mov   A,X",                   4, 1, 0x5b, disassemHandlerNoOperand },
  { "mov   X,A",                   4, 1, 0x5c, disassemHandlerNoOperand },
  { "mov   A,reg[%s]",             6, 2, 0x5d, disassemHandlerREG_srcDIR },
  { "mov   A,reg[X+%s]",           7, 2, 0x5e, disassemHandlerREG_srcIDX },
  { "mov   [%s],[%s]",            10, 3, 0x5f, disassemHandler_dstDIR_srcDIR },

  { "mov   reg[%s],A",             5, 2, 0x60, disassemHandlerREG_dstDIR },
  { "mov   reg[X+%s],A",           6, 2, 0x61, disassemHandlerREG_dstIDX },
  { "mov   reg[%s],%03xh",         8, 3, 0x62, disassemHandlerREG_dstDIR_srcIMM_MOV },
  { "mov   reg[X+%s],%03xh",       9, 3, 0x63, disassemHandlerREG_dstIDX_srcIMM },
  { "asl   A",                     4, 1, 0x64, disassemHandlerNoOperand },
  { "asl   [%s]",                  7, 2, 0x65, disassemHandler_srcDIR },
  { "asl   [X+%s]",                8, 2, 0x66, disassemHandler_srcIDX },
  { "asr   A",                     4, 1, 0x67, disassemHandlerNoOperand },

  { "asr   [%s]",                  7, 2, 0x68, disassemHandler_srcDIR },
  { "asr   [X+%s]",                8, 2, 0x69, disassemHandler_srcIDX },
  { "rlc   A",                     4, 1, 0x6a, disassemHandlerNoOperand },
  { "rlc   [%s]",                  7, 2, 0x6b, disassemHandler_srcDIR },
  { "rlc   [X+%s]",                8, 2, 0x6c, disassemHandler_srcIDX },
  { "rrc   A",                     4, 1, 0x6d, disassemHandlerNoOperand },
  { "rrc   [%s]",                  7, 2, 0x6e, disassemHandler_srcDIR },
  { "rrc   [X+%s]",                8, 2, 0x6f, disassemHandler_srcIDX },

  { "and   F,%03xh",               4, 2, 0x70, disassemHandlerPSW_AND },
  { "or    F,%03xh",               4, 2, 0x71, disassemHandlerPSW_OR },
  { "xor   F,%03xh",               4, 2, 0x72, disassemHandlerPSW_XOR },
  { "cpl   A",                     4, 1, 0x73, disassemHandlerNoOperand },
  { "inc   A",                     4, 1, 0x74, disassemHandlerNoOperand },
  { "inc   X",                     4, 1, 0x75, disassemHandlerNoOperand },
  { "inc   [%s]",                  7, 2, 0x76, disassemHandler_srcDIR },
  { "inc   [X+%s]",                8, 2, 0x77, disassemHandler_srcDIR },

  { "dec   A",                     4, 1, 0x78, disassemHandlerNoOperand },
  { "dec   X",                     4, 1, 0x79, disassemHandlerNoOperand },
  { "dec   [%s]",                  7, 2, 0x7a, disassemHandler_srcDIR },
  { "dec   [X+%s]",                8, 2, 0x7b, disassemHandler_srcIDX },
  { "lcall %s",                   13, 3, 0x7c, disassemHandlerAbs16 },
  { "ljmp  %s",                    7, 3, 0x7d, disassemHandlerAbs16 },
  { "reti",                       10, 1, 0x7e, disassemHandlerRET },
  { "ret",                         8, 1, 0x7f, disassemHandlerRET },

  { "jmp   %s",                    5, 2, 0x80, disassemHandlerRelJump },
  { "call  %s",                   11, 2, 0x90, disassemHandlerRelCall },
  { "jz    %s",                    5, 2, 0xa0, disassemHandlerRelJump },
  { "jnz   %s",                    5, 2, 0xb0, disassemHandlerRelJump },
  { "jc    %s",                    5, 2, 0xc0, disassemHandlerRelJump },
  { "jnc   %s",                    5, 2, 0xd0, disassemHandlerRelJump },
  { "jacc  %s",                    7, 2, 0xe0, disassemHandlerRelJump },
  { "index %s",                   13, 2, 0xf0, disassemHandlerRelCall },
};

static unsigned char curPP = 0;
static unsigned char stkPP = 0;
static unsigned char idxPP = 0;
static unsigned char mvrPP = 0;
static unsigned char mvwPP = 0;
static PSW_t gPSW;
static PSW_t gPSWOld;
static int gBlankAfterRET = 0;
static symbol_t *gSymbolsRAM = NULL;
static symbol_t *gSymbolsROM = NULL;
static processor_t *gProcessor = NULL;
static char *gComment = NULL;

//
//
//
static void disassemAppendComment (char *comment)
{
  if (!gComment)
    gComment = strdup (comment);
  else
  {
    gComment = realloc (gComment, strlen (gComment) + strlen (comment) + 1);
    strcat (gComment, comment);
  }
}

static void disassemClearComments (void)
{
  if (gComment)
  {
    free (gComment);
    gComment = NULL;
  }
}

static void disassemPrintComments (void)
{
  if (gComment)
  {
    printf (" ; %s", gComment);
    disassemClearComments ();
  }

  printf ("\n");
}

//
//
//
static void disassemTrackPgmodeChange (void)
{
  char buffer [128];

  if (gPSW.bits.pgMode != gPSWOld.bits.pgMode)
  {
    snprintf (buffer, sizeof (buffer), "Paging mode changed from %d to %d", gPSWOld.bits.pgMode, gPSW.bits.pgMode);
    disassemAppendComment (buffer);
  }

  gPSWOld.byte = gPSW.byte;
}

//
//
//
static void disassemPrintAddrPlusBytes (unsigned char *memory, int pc, int bytes)
{
  int i;

  printf ("  %04x:", pc);

  for (i = 0; i < 3; i++)
  {
    if (i < bytes)
      printf (" %02x", memory [pc + i]);
    else
      printf ("   ");
  }

  printf ("  ");
}

//
//
//
static char *disassemSearchSymbolListROM (int address)
{
  symbol_t *sl = gSymbolsROM;

  while (sl && (sl->address <= address))
  {
    if (address == sl->address)
      return sl->label;

    sl = sl->next;
  }

  return NULL;
}

static char *disassemSearchSymbolListRAM (int address)
{
  symbol_t *sl = gSymbolsRAM;
  char *match = NULL;
  int partialMatches = 0;

  while (sl)
  {
    if (address == sl->address)
      match = sl->label;
    else if ((address & 0xff) == (sl->address & 0xff))
    {
      if (!partialMatches++)
        disassemAppendComment ("or possibly ");
      else
        disassemAppendComment (", ");

      disassemAppendComment (sl->label);
    }

    sl = sl->next;
  }

  return match;
}

//
//
//
static char *disassemConvertToRamSymbolOrHex (int page, int address)
{
  char *s;
  static char operand [16];
  
  if (!(s = disassemSearchSymbolListRAM ((page << 8) | address)))
  {
    snprintf (operand, sizeof (operand), "%03xh", address);
    s = operand;
  }

  return s;
}

//
//
//
static int disassemHandlerNoOperand (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  printf ("%s", ins->text);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_srcIMM (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  printf (ins->text, memory [pc + 1]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_srcDIR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]));
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_srcIDX (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode == 2) ? idxPP : (gPSW.bits.pgMode & 1) ? stkPP : 0, memory [pc + 1]));
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstDIR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]));
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstIDX (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode == 2) ? idxPP : (gPSW.bits.pgMode & 1) ? stkPP : 0, memory [pc + 1]));
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstDIR_srcIMM (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]), memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstIDX_srcIMM (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode == 2) ? idxPP : (gPSW.bits.pgMode & 1) ? stkPP : 0, memory [pc + 1]), memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstDIR_srcDIR (unsigned char *memory, int pc, instruction_t *ins)
{
  char operand2 [16];

  strncpy (operand2, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 2]), sizeof (operand2));

  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]), operand2);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_srcINDPI (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]), memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandler_dstINDPI (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, disassemConvertToRamSymbolOrHex ((gPSW.bits.pgMode & 2) ? curPP : 0, memory [pc + 1]), memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_srcDIR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_srcIDX (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_dstDIR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_dstIDX (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_dstDIR_srcIMM (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]], memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_dstDIR_srcIMM_MOV (unsigned char *memory, int pc, instruction_t *ins)
{
  //
  //  FIXME: Stupid, using hardcoded constants for these registers in user
  //  space.
  //
  if (!gPSW.bits.xio)
  {
    switch (memory [pc + 1])
    {
      case 0xd0 : curPP = memory [pc + 2]; break;
      case 0xd1 : stkPP = memory [pc + 2]; break;
      case 0xd3 : idxPP = memory [pc + 2]; break;
      case 0xd4 : mvrPP = memory [pc + 2]; break;
      case 0xd5 : mvwPP = memory [pc + 2]; break;
      default   : break;
    }
  }

  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]], memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerREG_dstIDX_srcIMM (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf (ins->text, gPSW.bits.xio ? gProcessor->config [memory [pc + 1]] : gProcessor->user [memory [pc + 1]], memory [pc + 2]);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerPSW_AND (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  gPSW.byte &= memory [pc + 1];

  printf (ins->text, memory [pc + 1]);
  disassemTrackPgmodeChange ();
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerPSW_OR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  gPSW.byte |= memory [pc + 1];

  printf (ins->text, memory [pc + 1]);
  disassemTrackPgmodeChange ();
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerPSW_XOR (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  gPSW.byte ^= memory [pc + 1];

  printf (ins->text, memory [pc + 1]);
  disassemTrackPgmodeChange ();
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerRET (unsigned char *memory, int pc, instruction_t *ins)
{
  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);
  printf ("%s", ins->text);
  disassemPrintComments ();

  if (gBlankAfterRET)
    printf ("\n");

  return 1;
}

static int disassemHandlerAbs16 (unsigned char *memory, int pc, instruction_t *ins)
{
  char *s;
  char buffer [16];
  int address = (memory [pc + 1] << 8) | memory [pc + 2];

  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  if (!(s = disassemSearchSymbolListROM (address)))
  {
    snprintf (buffer, sizeof (buffer), "%04xh", address);
    s = buffer;
  }

  printf (ins->text, s);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerRelJump (unsigned char *memory, int pc, instruction_t *ins)
{
  char *s;
  char buffer [16];
  int offset = ((memory [pc] & 0x0f) << 8) + memory [pc + 1];

  if (offset > 2047)
    offset = -(0x1000 - offset);

  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  if (!(s = disassemSearchSymbolListROM (pc + 1 + offset)))
  {
    snprintf (buffer, sizeof (buffer), "%04xh", pc + 1 + offset);
    s = buffer;
  }

  printf (ins->text, s);
  disassemPrintComments ();

  return 1;
}

static int disassemHandlerRelCall (unsigned char *memory, int pc, instruction_t *ins)
{
  char *s;
  char buffer [16];
  int offset = ((memory [pc] & 0x0f) << 8) + memory [pc + 1];

  if (offset > 2047)
    offset = -(0x1000 - offset);

  disassemPrintAddrPlusBytes (memory, pc, ins->insLength);

  if (!(s = disassemSearchSymbolListROM (pc + 2 + offset)))
  {
    snprintf (buffer, sizeof (buffer), "%04xh", pc + 2 + offset);
    s = buffer;
  }

  printf (ins->text, s);
  disassemPrintComments ();

  return 1;
}

//
//
//
static symbol_t *disassemPrintSymbolIfAddressMatches (int pc, symbol_t *sl)
{
  if (sl)
  {
    while (sl && (sl->address < pc))
      sl = sl->next;

    while (sl && (sl->address == pc) && sl->label)
    {
      printf ("%s:\n", sl->label);
      sl = sl->next;
    }
  }

  return sl;
}

//
//
//
static int disassemRawBytes (int pc, unsigned char *memory, int blockEnd, char *comment)
{
  int blockLength = blockEnd - pc;

  assert (blockLength > 0);

  if (blockLength <= 16)
  {
    int needComma = 0;

    printf ("  %04x:           db    ", pc);

    while (pc < blockEnd)
    {
      if (needComma)
        printf (", ");

      printf ("%03xh", memory [pc++]);
      needComma = 1;
    }

    if (comment)
      printf (" ; %s", comment);

    printf ("\n");
  }
  else
  {
    while (pc < blockEnd)
    {
      int lineBoundary = pc | 0x0f;
      int lineEnd = min (lineBoundary, blockEnd);
      int needComma = 0;

      assert (lineEnd <= blockEnd);

      printf ("  %04x:           db    ", pc);

      while (pc <= lineEnd)
      {
        if (needComma)
          printf (", ");

        printf ("%03xh", memory [pc++]);
        needComma = 1;
      }

      if (comment)
        printf (" ; %s", comment);

      printf ("\n");
    }
  }

  printf ("\n");

  return blockLength;
}

//
//
//
int disassemDisassemble (unsigned char *memory, int memorySize, char *map, symbol_t *symbolsROM, symbol_t *symbolsRAM, processor_t *processor, int blankAfterRET)
{
  int pc;
  int insLength;
  symbol_t *romSymbols = symbolsROM;

  gSymbolsROM = symbolsROM;
  gSymbolsRAM = symbolsRAM;
  gProcessor = processor;
  gBlankAfterRET = blankAfterRET;

  for (insLength = 1, pc = 0; pc < memorySize; pc += insLength)
  {
    switch (map [pc])
    {
      case 'C' :
        {
          romSymbols = disassemPrintSymbolIfAddressMatches (pc, romSymbols);
          insLength = disassemRawBytes (pc, memory, romSymbols ? romSymbols->address : memorySize, "configuration table");
        }
        break;

      case 'E' :
        {
          romSymbols = disassemPrintSymbolIfAddressMatches (pc, romSymbols);
          insLength = disassemRawBytes (pc, memory, romSymbols ? romSymbols->address : memorySize, "EEPROM");
        }
        break;

      case 'I' :
        {
          unsigned opcode = memory [pc];

          if (opcode & 0x80)
            opcode = 0x78 + (opcode >> 4);

          if (opcode > arrsizeof (instructions))
          {
            fprintf (stderr, "Eeek!  Opcode is greater than table entries (opcode=%d, table size=%ld)\n", opcode, arrsizeof (instructions));
            return 0;
          }

          if (!instructions [opcode].disassemHandler)
          {
            fprintf (stderr, "Eeek!  No disassemHandler installed for opcode 0x%02x\n", memory [pc]);
            return 0;
          }

          disassemClearComments ();
          romSymbols = disassemPrintSymbolIfAddressMatches (pc, romSymbols);

          if (!(*instructions [opcode].disassemHandler) (memory, pc, &instructions [opcode]))
            return 0;

          insLength = instructions [opcode].insLength;
        }
        break;

      case 'L' :
        {
          romSymbols = disassemPrintSymbolIfAddressMatches (pc, romSymbols);
          insLength = disassemRawBytes (pc, memory, romSymbols ? romSymbols->address : memorySize, "literal");
        }
        break;

      default :
        insLength = 1;
        break;
    }

    assert (insLength);
  }

  return 1;
}
