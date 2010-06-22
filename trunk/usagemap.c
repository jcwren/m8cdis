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

#include "m8cdis.h"
#include "mapfile.h"
#include "usagemap.h"

//
//
//
int usagemapMap (section_t *sectionList, char **mapPtr, int mapSize)
{
  char *map;

  if (!(map = malloc (mapSize)))
  {
    perror ("usageMap: malloc");
    return 0;
  }

  memset (map, 'U', mapSize);

  while (sectionList)
  {
    if (sectionList->memtype == MEMTYPE_ROM)
      memset (map + sectionList->address, sectionList->mapFill, sectionList->size);

    sectionList = sectionList->next;
  }

  *mapPtr = map;

  return 1;
}

//
//
//
int usagemapDisplay (char *map, int mapSize)
{
  int i;

  printf ("Memory usage map\n");
  printf ("  C = Config\n");
  printf ("  E = EEPROM\n");
  printf ("  I = Instructions\n");
  printf ("  L = Literal\n");
  printf ("  U = Unknown\n");
  printf ("\n");

  for (i = 0; i < mapSize - 1; i += 64)
    printf ("  %04xh: %.64s\n", i, map + i);

  printf ("\n");

  return 1;
}
