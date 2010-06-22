//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#ifndef _DISASSEM_H_
#define _DISASSEM_H_

int disassemDisassemble (unsigned char *memory, int memorySize, char *map, symbol_t *romSymbols, symbol_t *ramSymbols, processor_t *processor, int blankAfterRET);

#endif
