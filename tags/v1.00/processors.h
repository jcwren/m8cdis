//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#ifndef _PROCESSORS_H_
#define _PROCESSORS_H_

#include "mapfile.h" 

typedef struct processor_s
{
  char *name;
  int  flash;
  int  ram;
  int  ioPins;
  int  digitalRows;
  int  digitalBlocks;
  int  analogPins;
  int  analogOutputs;
  int  analogColumns;
  int  analogBlocks;
  int  hasMAC;
  mapfileExtraSymbols_t *extraSymbols;
  int extraSymbolsSize;
  char **user;
  char **config;
}
processor_t;

processor_t *processorsSelect (char *processorName);
processor_t **processorsGetList (void);

#endif
