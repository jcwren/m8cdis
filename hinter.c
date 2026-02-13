//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>

#include "m8cdis.h"
#include "mapfile.h"
#include "hinter.h"
#include "util.h"

//
//
//
static void *hinterError (FILE *fp, const char *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  vfprintf (fp, fmt, ap);
  va_end (ap);

  return NULL;
}

//
//
//
static section_t *hinterGet (section_t *sectionList, FILE *fp)
{
  char buffer [512];
  int lineNumber = 0;

  while (fgets (buffer, sizeof (buffer), fp))
  {
    char *action;
    section_t *sl;

    ++lineNumber;

    utilDeleteLeadingWhiteSpace (buffer);
    utilDeleteTrailingWhiteSpace (buffer);

    if (strlen (buffer) && (buffer [0] != '#'))
    {
      assert (action = strtok (buffer, " \t"));

      if (!strcasecmp (action, "label"))
      {
        //
        // <label> <ram|rom> <address> <label_name>
        //
        char *memtype = strtok (NULL, " \t");
        char *address = strtok (NULL, " \t");
        char *label = strtok (NULL, " \t");
        symbol_t symbol;

        memset (&symbol, 0, sizeof (symbol_t));

        symbol.label = strdup (label);
        symbol.address = utilHtoi (address);

        if (strcasecmp (memtype, "rom") && strcasecmp (memtype, "ram"))
          return hinterError (stderr, "Eeek!  Invalid memory type \"%s\" at line %d in hint file.  Aborting.\n", memtype, lineNumber);

        if (symbol.address == -1)
          return hinterError (stderr, "Eeek!  Invalid address \"%s\" at line %d in hint file.  Aborting.\n", address, lineNumber);

        if (!label || !strlen (label))
          return hinterError (stderr, "Eeek!  Missing label at line %d in hint file.  Aborting.\n", lineNumber);

        if (!(sl = mapfileFindSection (sectionList, !strcasecmp (memtype, "rom") ? "text" : "bss")))
          return hinterError (stderr, "Eeek!  There appears to be no text or no bss section in the map file.  Aborting.\n");

        if (!mapfileAddToSymbolList (sl, &symbol))
          return hinterError (stderr, "Eeek!  Couldn't add label \"%s\" to symbol list.  Aborting.\n");
      }
      else
      {
        //
        // <code|ram|literal|unknown|config|eeprom> <address-start> <address-end> <section_name>
        //
        char *startAddr = strtok (NULL, " \t");
        char *endAddr = strtok (NULL, " \t");
        char *label = strtok (NULL, " \t");
        section_t section;

        memset (&section, 0, sizeof (section_t));

        section.address = utilHtoi (startAddr);
        section.size = utilHtoi (endAddr);
        section.memtype = MEMTYPE_ROM;
        section.mapFill = 'U';

        if (!strcasecmp (action, "code"))
          section.mapFill = 'I';
				else if (!strcasecmp (action, "ram"))
				{
					section.mapFill = ' ';
					section.memtype = MEMTYPE_RAM;
				}
        else if (!strcasecmp (action, "literal"))
          section.mapFill = 'L';
        else if (!strcasecmp (action, "config"))
          section.mapFill = 'C';
        else if (!strcasecmp (action, "eeprom"))
          section.mapFill = 'E';
        else if (!strcasecmp (action, "unknown"))
          section.mapFill = 'U';
        else
          return hinterError (stderr, "Eeek!  Unknown keyword \"%s\" in hint file at line %d in hint file.  Aborting.\n", action, lineNumber);

        if (section.address == -1)
          return hinterError (stderr, "Eeek!  Invalid starting address \"%s\" at line %d in hint file.  Aborting.\n", startAddr, lineNumber);

        if (section.size == -1)
          return hinterError (stderr, "Eeek!  Invalid ending address \"%s\" at line %d in hint file.  Aborting.\n", endAddr, lineNumber);

        if (section.size < section.address)
          return hinterError (stderr, "Eeek!  Invalid ending address \"%s\" < starting address \"%s\" at line %d in hint file.  Aborting.\n", endAddr, startAddr, lineNumber);

        if (!label || !strlen (label))
          return hinterError (stderr, "Eeek!  Missing section name at line %d in hint file.  Aborting.\n", lineNumber);

        section.label = strdup (label);
        section.size -= (section.address - 1);

        if (!mapfileAddToSectionList (&sectionList, &section))
          return NULL;
      }
    }
  }

  return sectionList;
}

//
//
//
section_t *hinterLoad (section_t *sectionList, char *filename)
{
  FILE *fp;

  if (!(fp = fopen (filename, "r")))
    return sectionList;

  sectionList = hinterGet (sectionList, fp);

  fclose (fp);

  return sectionList;
}
