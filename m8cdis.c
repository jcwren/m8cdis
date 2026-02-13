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
#include <getopt.h>

#include "m8cdis.h"
#include "processors.h"
#include "mapfile.h"
#include "hexfile.h"
#include "usagemap.h"
#include "disassem.h"
#include "hexdump.h"
#include "hinter.h"

//
//
//
static int gDisplaySymbols;
static int gDisplaySections;
static int gDisplayUsageMap;
static int gDisplayHexDump;
static int gDisplayDisassembly = 1;
static int gBlankAfterRET;
static int gExtraSymbols;
static char *gInputFile;
static char *gOutputFile;
static processor_t *gProcessor;

//
//
//
static char *m8cdisMakeFilename (char *directory, char *basename, char *extension)
{
  int d = strlen (directory);
  int b = strlen (basename);
  int e = strlen (extension);
  char *fn;

  if (!(fn = calloc (d + b + e + 1, sizeof (char))))
  {
    perror ("m8cdisMakeFilename: calloc");
    return NULL;
  }

  strcpy (fn, directory);
  strcat (fn, basename);
  strcat (fn, extension);

  return fn;
}

//
//
//
static void m8sdisUsage (void)
{
  printf ("\n");
  printf ("m8cdis [options] -i <filename>\n");
  printf ("\n");
  printf ("  -a | --symbol-table      Display symbol table\n");
  printf ("  -b | --blank-after-ret   Add blank line after RET/RETI instructions\n");
  printf ("  -d | --no-disassembly    Suppress disassembly listing\n");
  printf ("  -e | --extra-symbols     Add extra symbols to symbol table (*1)\n");
  printf ("  -i | --input <filename>  Input file name\n");
  printf ("  -o | --output <filename> Output file name\n");
  printf ("  -p | --processor         Select processor\n");
  printf ("  -s | --section-map       Display section map\n");
  printf ("  -u | --usage-map         Display memory usage map\n");
  printf ("  -v | --version           Display program version\n");
  printf ("  -x | --hex-dump version  Display memory usage map\n");
  printf ("\n");
  printf ("To show supported processors use '-p list' option.  If no processor\n");
  printf ("is selected, the default will be cy8c24894\n");
  printf ("\n");
  printf ("*1 - Extra symbols are the register definitions for SSC calls and\n");
  printf ("     the interrupt vector names\n");
  printf ("\n");
}

//
//
//
static void m8cdisShowSupportedProcessors (void)
{
  int i;
  processor_t **p = processorsGetList ();

  printf ("Supported processor names are:\n\n");

  for (i = 0; p [i]; i++)
    printf ("  %s\n", p [i]->name);

  printf ("\n");
}

//
//
//
static int m8cdisGetargs (int argc, char **argv)
{
  int c;
  int argsFound = 0;

  while (1)
  {
    int option_index = 0;
    static struct option long_options [] =
    {
      {"symbol-table",     0, 0, 'a'}, // -a | --symbol-table         
      {"blank-after-ret",  0, 0, 'b'}, // -b | --blank-after-ret
      {"no-disassembly",   0, 0, 'd'}, // -d | --no-disassembly
      {"extra-symbols",    0, 0, 'e'}, // -e | --extra-symbols
      {"help",             0, 0, 'h'}, // -h | --help
      {"input",            1, 0, 'i'}, // -i | --input <filename>
      {"output",           1, 0, 'o'}, // -o | --output <filename>
      {"processor",        1, 0, 'p'}, // -p | --processor <cy8c24894>
      {"section-map",      0, 0, 's'}, // -s | --section-map
      {"usage-map",        0, 0, 'u'}, // -u | --usage-map
      {"version",          0, 0, 'v'}, // -v | --version
      {"hex-dump",         0, 0, 'x'}, // -x | --hex-dump
      {0, 0, 0, 0}
    };

    opterr = 0;

    c = getopt_long (argc, argv, "abdehi:o:p:suvx?", long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 'a' :
        gDisplaySymbols = 1;
        argsFound++;
        break;

      case 'b' :
        gBlankAfterRET = 1;
        argsFound++;
        break;

      case 'd' :
        gDisplayDisassembly = 0;
        argsFound++;
        break;

      case 'e' :
        gExtraSymbols = 1;
        argsFound++;
        break;

      case 'h' : 
        m8sdisUsage ();
        return 0;

      case 'i' :
        {
          if (!gInputFile)
            gInputFile = optarg;
          else
          {
            printf ("Input file already specified, can't use -i twice.\n");
            return 0;
          }

          argsFound++;
        }
        break;

      case 'o' :
        {
          if (!gOutputFile)
            gOutputFile = optarg;
          else
          {
            printf ("Output file already specified, can't use -o twice.\n");
            return 0;
          }

          argsFound++;
        }
        break;

      case 'p' :
        {
          if (!gProcessor)
          {
            if (!strcasecmp (optarg, "list"))
            {
              m8cdisShowSupportedProcessors ();
              return 0;
            }
            else
            {
              if (!(gProcessor = processorsSelect (optarg)))
              {
                printf ("Invalid processor \"%s\", use '-p list' to show supported processors\n", optarg);
                return 0;
              }
            }
          }
          else
          {
            printf ("Processor already specified, can't use -p twice.\n");
            return 0;
          }

          argsFound++;
        }
        break;

      case 's' :
        gDisplaySections = 1;
        argsFound++;
        break;

      case 'u' :
        gDisplayUsageMap = 1;
        argsFound++;
        break;

      case 'v' :
        printf ("m8cdis Version 1.00 2010/06/15 by JCWren\n");
        return 0;

      case 'x' :
        gDisplayHexDump = 1;
        argsFound++;
        break;

      case '?':
        printf ("Unrecognized option.  Use -h or --help option for help\n");
        return 0;

      default:
        printf ("Illegal argument(s) encountered.  Use -h or --help for help\n");
        return 0;
    }
  }

  if (optind < argc)
  {
    printf ("Illegal argument(s) encountered: ");

    while (optind < argc)
      printf ("%s ", argv [optind++]);

    printf ("\n");
  }

  if (!argsFound)
  {
    printf ("No arguments found.  Use -h or --help option for help\n");
    return 0;
  }

  if (!gInputFile)
  {
    printf ("No input file specified.  Use -i to specify or -? for help\n");
    return 0;
  }
  else
  {
    char *s;

    if ((s = strrchr (gInputFile, '.')))
      *s = 0;
  }

  if (!gProcessor)
    gProcessor = processorsSelect ("cy8c24894");

  return 1;
}

//
//
//
int main (int argc, char **argv)
{
  section_t *sectionList = NULL;
  symbol_t *romSymbolTable = NULL;
  symbol_t *ramSymbolTable = NULL;
  char *memoryMap = NULL;
  unsigned char *memory = NULL;
  int memorySize;
  char *filenameDotMp = NULL;
  char *filenameDotHex = NULL;
  char *filenameDotHint = NULL;
  int ec = 0;

  if (!ec && !m8cdisGetargs (argc, argv))
    ec = 1;

  if (gOutputFile)
  {
    if (!freopen (gOutputFile, "w", stdout))
    {
      perror ("main: freopen");
      ec = 1;
    }
  }

  if (!ec && !(filenameDotMp = m8cdisMakeFilename ("./", gInputFile, ".mp")))
    ec = 2;
  if (!ec && !(filenameDotHex = m8cdisMakeFilename ("./", gInputFile, ".hex")))
    ec = 3;
  if (!ec && !(filenameDotHint = m8cdisMakeFilename ("./", gInputFile, ".hint")))
    ec = 3;

  if (!ec && !(sectionList = mapfileParse (filenameDotMp)))
    ec = 4;

  if (!ec && gExtraSymbols && gProcessor && gProcessor->extraSymbols && !(sectionList = mapfileAddSpecialSymbols (&sectionList, gProcessor->extraSymbols, gProcessor->extraSymbolsSize)))
    ec = 5;

  if (!ec && !(sectionList = hinterLoad (sectionList, filenameDotHint)))
    ec = 8;

  if (!ec && gDisplaySections)
    mapfileDisplaySectionList (sectionList);

  if (!ec && !(romSymbolTable = mapfileSectionToSymbols (sectionList, MEMTYPE_ROM)))
    ec = 6;
  if (!ec && !(ramSymbolTable = mapfileSectionToSymbols (sectionList, MEMTYPE_RAM)))
    ec = 7;


  if (!ec && gDisplaySymbols)
  {
    mapfileDisplaySymbolList (romSymbolTable, "ROM");
    mapfileDisplaySymbolList (ramSymbolTable, "RAM");
  }

  if (!ec && !usagemapMap (sectionList, &memoryMap, gProcessor->flash))
    ec = 8;

  if (!ec && gDisplayUsageMap && !usagemapDisplay (memoryMap, gProcessor->flash))
    ec = 9;

  if (!ec && !hexfileLoad (filenameDotHex, &memory, &memorySize, 0x30))
    ec = 10;

  if (!ec && !hexdumpDisplay (memory, memorySize))
    ec = 11;

  if (!ec && gDisplayDisassembly && !disassemDisassemble (memory, memorySize, memoryMap, romSymbolTable, ramSymbolTable, gProcessor, gBlankAfterRET))
    ec = 12;
  
  if (ramSymbolTable)
    mapfileFreeSymbols (ramSymbolTable);
  if (romSymbolTable)
    mapfileFreeSymbols (romSymbolTable);
  if (sectionList)
    mapfileFreeSections (sectionList);
  if (memoryMap)
    free (memoryMap);
  if (memory)
    free (memory);
  if (filenameDotMp)
    free (filenameDotMp);
  if (filenameDotHex)
    free (filenameDotHex);
  if (filenameDotHint)
    free (filenameDotHint);

  exit (ec);
}
