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

#include "m8cdis.h"
#include "mapfile.h"
#include "util.h"

//
//  Define for debugging to dump symbol list by section
//
#undef  OPT_DUMPSYMBOLLIST

//
//
//
symbol_t *mapfileAddToSymbolList (section_t *sl, symbol_t *symbol)
{
  symbol_t *newSymbol;

  assert (sl);

  newSymbol = calloc (1, sizeof (symbol_t));

  memcpy (newSymbol, symbol, sizeof (symbol_t));

  newSymbol->next = sl->symbolList;
  sl->symbolList = newSymbol;

  return newSymbol;
}

//
//
//
#if OPT_DUMPSYMBOLLIST
static void mapfileDumpSymbolList (section_t *section)
{
  symbol_t *slp = section->symbolList;

  while (slp)
  {
    printf ("%30s %04x\n", slp->label, slp->address);
    slp = slp->next;
  }
}
#endif

//
//
//
typedef struct compareList_s
{
  char *label;
  char  fillChar;
}
compareList_t;

static compareList_t compareList [] = 
{
  { "lit",          'L' },
  { "func_lit",     'L' },
  { "psoc_config",  'C' },
  { "UserModules",  'I' },
  { "text",         'I' },
  { "TOP",          'I' },
  { "E2PROM_AREA",  'E' },
};

static char mapfileSectionNameToFillChar (char *label)
{
  unsigned int i;

  for (i = 0; i < arrsizeof (compareList); i++)
    if (!strcasecmp (label, compareList [i].label))
      return compareList [i].fillChar;

  return 'U';
}

//
//
//
section_t *mapfileAddToSectionList (section_t **sl, section_t *section)
{
  section_t *slLast = *sl;

  assert (sl != NULL);

  if (!*sl)
  {
    if (!(*sl = slLast = calloc (1, sizeof (section_t))))
    {
      perror ("mapfileAddToSectionList: calloc");
      return NULL;
    }
  }
  else
  {
    while (slLast->next)
      slLast = slLast->next;

    slLast->next = calloc (1, sizeof (section_t));
    slLast = slLast->next;
  }

  memcpy (slLast, section, sizeof (section_t));

  return slLast;
}

//
//
//
typedef enum 
{
  STATE_WANT_AREA_TAG = 0,
  STATE_WANT_AREA_DASH_LINE_AS_ADDR_SIZE,
  STATE_WANT_AREA_DASH_LINE_AS_START_END,
  STATE_WANT_SECTION_AS_ADDR_SIZE,
  STATE_WANT_SECTION_AS_START_END,
  STATE_WANT_ADDRESS_TAG,
  STATE_WANT_ADDRESS_DASH_LINE,
  STATE_WANT_ADDRESS_ENTRY,
}
state_e;

static section_t *mapfileGet (FILE *fp)
{
  char buffer [512];
  state_e state = STATE_WANT_AREA_TAG;
  int lineNumber = 0;
  section_t *sectionList = NULL;
  section_t *sl = NULL;

  while (fgets (buffer, sizeof (buffer), fp))
  {
    ++lineNumber;

    utilDeleteLeadingWhiteSpace (buffer);
    utilDeleteTrailingWhiteSpace (buffer);

    switch (state)
    {
      case STATE_WANT_AREA_TAG :
        {
          if (!strncmp (buffer, "Area ", 5))
          {
            char *area, *startOrAddr, *sizeOrEnd;

            area = strtok (buffer, " \t");
            startOrAddr = strtok (NULL, " \t");
            sizeOrEnd = strtok (NULL, " \t");

            if (!strcasecmp (startOrAddr, "Addr") && !strcasecmp (sizeOrEnd, "Size"))
              state = STATE_WANT_AREA_DASH_LINE_AS_ADDR_SIZE;
            else if (!strcasecmp (startOrAddr, "Start") && !strcasecmp (sizeOrEnd, "End"))
              state = STATE_WANT_AREA_DASH_LINE_AS_START_END;
            else if (!strcasecmp (startOrAddr, "Definitions"))
              state = STATE_WANT_AREA_TAG;
            else
            {
              fprintf (stderr, "Eeek!  Can't figure out what sort of area line is at line %d\n", lineNumber);
              return 0;
            }
          }
          else
            state = STATE_WANT_AREA_TAG;
        }
        break;

      case STATE_WANT_AREA_DASH_LINE_AS_ADDR_SIZE :
        state = (!strncmp (buffer, "-----", 5) ? STATE_WANT_SECTION_AS_ADDR_SIZE : STATE_WANT_AREA_DASH_LINE_AS_ADDR_SIZE);
        break;

      case STATE_WANT_AREA_DASH_LINE_AS_START_END :
        state = (!strncmp (buffer, "-----", 5) ? STATE_WANT_SECTION_AS_START_END : STATE_WANT_AREA_DASH_LINE_AS_START_END);
        break;

      case STATE_WANT_SECTION_AS_ADDR_SIZE :
        {
          char *label, *addr, *size, *attr;
          section_t section;

          label = strtok (buffer, " \t");   // Get section name
          addr = strtok (NULL, " \t");      // Get section address
          size = strtok (NULL, " \t");      // Get section size
          strtok (NULL, " \t");             // Skip =
          strtok (NULL, " \t");             // Skip decimal
          strtok (NULL, " \t");             // Skip bytes
          attr = strtok (NULL, " \t");      // Get attribute list
          
          if (mapfileFindSection (sectionList, label))
          {
            fprintf (stderr, "Eeek!  Section \"%s\" is duplicated.  Aborting.\n", label);
            return NULL;
          }

          memset (&section, 0, sizeof (section));

          section.label = strdup (label);
          section.address = utilHtoi (addr);
          section.size = utilHtoi (size);
          section.mapFill = mapfileSectionNameToFillChar (label);

          if (strcasestr (attr, "rom"))
            section.memtype = MEMTYPE_ROM;
          else if (strcasestr (attr, "ram"))
            section.memtype = MEMTYPE_RAM;
          else
            section.memtype = MEMTYPE_UNKNOWN;

          if (!(sl = mapfileAddToSectionList (&sectionList, &section)))
            return NULL;

          state = STATE_WANT_ADDRESS_TAG;
        }
        break;

      case STATE_WANT_SECTION_AS_START_END :
        {
          char *label, *start, *end, *attr;
          section_t section;

          label = strtok (buffer, " \t");   // Get section name
          start = strtok (NULL, " \t");     // Get section start
          end = strtok (NULL, " \t");       // Get section end
          strtok (NULL, " \t");             // Skip =
          strtok (NULL, " \t");             // Skip decimal
          strtok (NULL, " \t");             // Skip bytes
          attr = strtok (NULL, " \t");      // Get attribute list
          
          if (mapfileFindSection (sectionList, label))
          {
            fprintf (stderr, "Eeek!  Section \"%s\" is duplicated.  Aborting.\n", label);
            return NULL;
          }

          memset (&section, 0, sizeof (section));

          section.label = strdup (label);
          section.address = utilHtoi (start);
          section.size = utilHtoi (end) - section.address;
          section.mapFill = mapfileSectionNameToFillChar (label);

          if (strcasestr (attr, "rom"))
            section.memtype = MEMTYPE_ROM;
          else if (strcasestr (attr, "ram"))
            section.memtype = MEMTYPE_RAM;
          else
            section.memtype = MEMTYPE_UNKNOWN;

          if (!(sl = mapfileAddToSectionList (&sectionList, &section)))
            return NULL;

          state = STATE_WANT_ADDRESS_TAG;
        }
        break;

      case STATE_WANT_ADDRESS_TAG :
        state = (!strncmp (buffer, "Addr ", 5) ? STATE_WANT_ADDRESS_DASH_LINE : STATE_WANT_ADDRESS_TAG);
        break;

      case STATE_WANT_ADDRESS_DASH_LINE :
        state = (!strncmp (buffer, "-----", 5) ? STATE_WANT_ADDRESS_ENTRY : STATE_WANT_ADDRESS_DASH_LINE);
        break;

      case STATE_WANT_ADDRESS_ENTRY :
        {
          if (strlen (buffer) > 1)
          {
            char *label, *addr;
            symbol_t symbol;

            addr = strtok (buffer, " \t");
            label = strtok (NULL, " \t");

            memset (&symbol, 0, sizeof (symbol_t));

            symbol.label = strdup (label);
            symbol.address = utilHtoi (addr);

            if (!mapfileAddToSymbolList (sl, &symbol))
              return NULL;

            state = STATE_WANT_ADDRESS_ENTRY;
          }
          else
            state = STATE_WANT_AREA_TAG;
        }
        break;

      default :
        fprintf (stderr, "mapfileGet: Unexpected state value %d.  Aborting.\n", state);
        return NULL;
    }
  }

  return sectionList;
}

//
//
//
section_t *mapfileParse (char *filename)
{
  FILE *fp;
  section_t *section = NULL;

  if (!(fp = fopen (filename, "r")))
  {
    printf ("Can't open %s: %d/%s\n", filename, errno, strerror (errno));
    return NULL;
  }

  section = mapfileGet (fp);

  fclose (fp);

  return section;
}

//
//
//
section_t *mapfileFindSection (section_t *sl, char *label)
{
  while (sl)
  {
    if (!strcmp (sl->label, label))
      return sl;

    sl = sl->next;
  }

  return NULL;
}

section_t *mapfileAddSpecialSymbols (section_t **sectionList, mapfileExtraSymbols_t *es, int numExtraSymbols)
{
  while (numExtraSymbols--)
  {
    symbol_t *symbol;

    if ((symbol = es->symbolList))
    {
      section_t section;
      section_t *sl;

      if ((sl = mapfileFindSection (*sectionList, es->label)) && ((*sectionList)->memtype != es->memtype))
      {
        printf ("Cannot add a section with the same name and a different type (%s)\n", es->label);
        return NULL;
      }
      else
      {
        memset (&section, 0, sizeof (section_t));

        section.label = strdup (es->label);
        section.address = es->address;
        section.size = es->size;
        section.memtype = es->memtype;
        section.mapFill = es->mapFill;

        if (!(sl = mapfileAddToSectionList (sectionList, &section)))
          return NULL;
      }

      while (symbol->label)
      {
        symbol_t tempSym;

        memset (&tempSym, 0, sizeof (symbol_t));

        tempSym.label = strdup (symbol->label);
        tempSym.address = symbol->address;

         if (!mapfileAddToSymbolList (sl, &tempSym))
           return NULL;

         symbol++;
      }
    }
    
    es++;
  }

  return *sectionList;
}

//
//
//
int mapfileFreeSections (section_t *sl)
{
  section_t *slTemp;

  while (sl)
  {
    while (sl->symbolList)
    {
      symbol_t *s;

      if (sl->symbolList->label)
        free (sl->symbolList->label);

      s = sl->symbolList->next;
      free (sl->symbolList);
      sl->symbolList = s;
    }

    if (sl->label)
      free (sl->label);

    slTemp = sl->next;
    free (sl);
    sl = slTemp;
  }

  return 1;
}

//
//
//
void mapfileDisplaySectionList (section_t *slp)
{
  printf ("Section listing\n");

  while (slp)
  {
    printf ("  %30s %04x %04x %s\n", slp->label, slp->address, slp->size, (slp->memtype == 1) ? "rom" : (slp->memtype == 2) ? "ram" : "Unknown!");
#if OPT_DUMPSYMBOLLIST
    mapfileDumpSymbolList (slp);
#endif
    slp = slp->next;
  }

  printf ("\n");
}

//
//
//
static void *mapfileCalloc (size_t bytes)
{
  void *p;

  if (!(p = calloc (1, bytes)))
  {
    perror ("mapfileCalloc: calloc");
    return NULL;
  }

  return p;
}

symbol_t *mapfileInsertSymbolSorted (symbol_t *symbolList, symbol_t *newSymbol)
{
  if (!symbolList)
  {
    if (!(symbolList = mapfileCalloc (sizeof (symbol_t))))
      return NULL;

    symbolList->label = newSymbol->label;
    symbolList->address = newSymbol->address;
  }
  else
  {
    symbol_t *sl = symbolList;
    symbol_t *slPrevious = NULL;

    while (sl)
    {
      if (newSymbol->address < sl->address)
      {
        if (sl == symbolList)
        {
          symbol_t *tal;

          if (!(tal = mapfileCalloc (sizeof (symbol_t))))
            return NULL;

          tal->label = newSymbol->label;
          tal->address = newSymbol->address;
          tal->next = symbolList;

          return tal;
        }
        else
        {
          symbol_t *tal;

          if (!(tal = mapfileCalloc (sizeof (symbol_t))))
            return NULL;

          tal->label = newSymbol->label;
          tal->address = newSymbol->address;
          tal->next = slPrevious->next;

          slPrevious->next = tal;

          return symbolList;
        }
      }

      slPrevious = sl;
      sl = sl->next;
    }

    if (!sl)
    {
      symbol_t *tal;

      if (!(tal = mapfileCalloc (sizeof (symbol_t))))
        return NULL;

      tal->label = newSymbol->label;
      tal->address = newSymbol->address;

      slPrevious->next = tal;
    }
  }

  return symbolList;
}

//
//
//
symbol_t *mapfileSectionToSymbols (section_t *section, memtype_e memtype)
{
  symbol_t *symbolList = NULL;

  while (section)
  {
    if ((section->memtype == memtype) && section->symbolList)
    {
      symbol_t *sl = section->symbolList;

      while (sl)
      {
        if (!(symbolList = mapfileInsertSymbolSorted (symbolList, sl)))
          return NULL;

        sl = sl->next;
      }
    }

    section = section->next;
  }

  return symbolList;
}

int mapfileFreeSymbols (symbol_t *symbolList)
{
  symbol_t *ap;

  while (symbolList)
  {
    ap = symbolList->next;
    free (symbolList);
    symbolList = ap;
  }

  return 1;
}

//
//
//
void mapfileDisplaySymbolList (symbol_t *slp, char *type)
{
  printf ("%s symbol table (sorted by address)\n", type);

  while (slp)
  {
    printf ("  %04x %s\n", slp->address, slp->label);
    slp = slp->next;
  }

  printf ("\n");
}
