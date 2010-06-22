//
//  $Id$
//  $Revision$
//  $Date$
//  $Author$
//  $HeadURL$
//

#ifndef _MAPFILE_H_
#define _MAPFILE_H_

//
//
//
typedef enum
{
  MEMTYPE_UNKNOWN = 0,
  MEMTYPE_ROM,
  MEMTYPE_RAM,
}
memtype_e;

typedef struct symbol_s
{
  char *label;
  int   address;
  struct symbol_s *next;
}
symbol_t;

typedef struct section_s
{
  char *label;
  int   address;
  int   size;
  memtype_e memtype;
  char  mapFill;
  symbol_t *symbolList;
  struct section_s *next;
}
section_t;

typedef struct mapfileExtraSymbols_s
{
  char *label;
  int address;
  int size;
  memtype_e memtype;
  char mapFill;
  symbol_t *symbolList;
}
mapfileExtraSymbols_t;

//
//
//
symbol_t  *mapfileAddToSymbolList (section_t *sl, symbol_t *symbol);
section_t *mapfileAddToSectionList (section_t **sl, section_t *section);
section_t *mapfileParse (char *filename);
section_t *mapfileFindSection (section_t *sl, char *label);
section_t *mapfileAddSpecialSymbols (section_t **sectionList, mapfileExtraSymbols_t *es);
int        mapfileFreeSections (section_t *sectionList);
void       mapfileDisplaySectionList (section_t *slp);
symbol_t  *mapfileInsertSymbolSorted (symbol_t *symbolList, symbol_t *newSymbol);
symbol_t  *mapfileSectionToSymbols (section_t *section, memtype_e memtype);
int        mapfileFreeSymbols (symbol_t *symbolList);
void       mapfileDisplaySymbolList (symbol_t *alp, char *type);

#endif
