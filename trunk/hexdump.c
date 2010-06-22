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
#include <ctype.h>

#include "m8cdis.h"
#include "hexdump.h"

//
//
//
int hexdumpDisplay (unsigned char *memory, int memorySize)
{
  int i;

  printf ("Dump of memory loaded from hex file\n");

  for (i = 0; i < memorySize; i += 16)
  {
    unsigned char *p;
    int j;
    int l;

    printf ("  %04xh: ", i);

    l = (((memorySize - i) < 16) ? (memorySize - i) : 16);

    for (p = &memory [i], j = 0; j < l; j++, p++)
      printf ("%02x ", *p);

    for (j = 0; j < 16 - l; j++)
      printf ("   ");

    printf ("  ");

    for (p = &memory [i], j = 0; j < l; j++, p++)
      printf ("%c", isprint (*p) ? *p : '.');

    printf ("\n");
  }

  printf ("\n");

  return 1;
}
