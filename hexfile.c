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
#include <ctype.h>
#include <errno.h>

#include "m8cdis.h"
#include "hexfile.h"

//
//
//
static int hexfileGetBytes (int numBytes, char **hexline, int *hexval)
{
  int i;
  int val = 0;

  for (i = 0; i < numBytes * 2; i++, (*hexline)++)
  {
    char c = toupper ((int) **hexline);

    if (!c || !isxdigit ((int) c))
      return 0;

    val = (val << 4) + (c <= '9' ? c & 0x0f : (c - 0x07) & 0x0f);
  }

  *hexval = val;

  return 1;
}

//
//
//
static int hexfileHexToBin (unsigned char **memImage, char *hexline, int *segment, int *memImageLastByte, int unusedFill)
{
  int i;
  int len;
  int addr;
  int rectype;
  int checkbyte;
  unsigned int checksum = 0;
  unsigned char buffer [256];

  if (*hexline++ != ':')
    return 0;

  if (!hexfileGetBytes (1, &hexline, &len))
    return 0;

  if (!hexfileGetBytes (2, &hexline, &addr))
    return 0;

  if (!hexfileGetBytes (1, &hexline, &rectype))
    return 0;

  checksum -= len;
  checksum -= addr >> 8;
  checksum -= addr & 0xff;
  checksum -= rectype;

  addr += *segment;

  //
  //  Largest part is 128K  If we have a segment that's at that address or
  //  higher, just say we did something, and return.  Either we're at the end
  //  of the file, and all is well, or there will be another ELA after it, and
  //  more legitimate code.
  //
  if (*segment >= 0x20000)
    return 1;

  switch (rectype)
  {
    //
    //  Data record
    //
    case 0x00 :
      for (i = 0; i < len; i++)
      {
        int byte;

        if (!hexfileGetBytes (1, &hexline, &byte))
          return 0;

        buffer [i] = byte & 0xff;

        checksum -= byte;
      }
      break;

    //
    //  End of file
    //
    case 0x01:
      return 1;

    //
    //  Extended segment address (bits 4..19)
    //
    case 0x02:
      printf ("Can't handle ESA's yet\n");
      return 0;

    //
    //  Start address records are ignored
    //
    case 0x03:
      return 1;

    //
    //  Extended linear address (bits 16..31)
    //
    case 0x04:
      {
        int byte;

        if (!hexfileGetBytes (1, &hexline, &byte))
          return 0;

        *segment = byte << 8;
        checksum -= byte;

        if (!hexfileGetBytes (1, &hexline, &byte))
          return 0;

        *segment |= byte;
        *segment <<= 16;
        checksum -= byte;
      }
      break;

    default :
      printf ("Unknown record type encountered (%02x)", rectype);
      return 0;
  }

  if (rectype == 0x00)
  {
    if (addr + len > *memImageLastByte)
    {
      if ((*memImage = (unsigned char *) realloc (*memImage, addr + len)) == NULL)
      {
        printf ("Cannot reallocate memory: %s\n", strerror (errno));
        return 0;
      }

      memset (*memImage + *memImageLastByte, unusedFill, (addr + len) - *memImageLastByte);

      *memImageLastByte = addr + len;
    }

    memcpy (*memImage + addr, buffer, len);
  }

  if (!hexfileGetBytes (1, &hexline, &checkbyte))
    return 0;

  if (checkbyte != (int) (checksum & 0xff))
    return 0;

  return 1;
}

//
//
//
int hexfileLoad (char *filename, unsigned char **memory, int *memorySize, int unusedFill)
{
  FILE *fp;
  int linecnt = 0;
  char buffer [256];
  unsigned char *memImage = NULL;
  int memImageLen = 0;
  int segment = 0;
  int newSize = 0;

  *memory = NULL;
  *memorySize = 0;

  if ((fp = fopen (filename, "r")) == NULL)
  {
    printf ("Cannot open file \"%s\":%s\n", filename, strerror (errno));
    return 0;
  }

  while (fgets (buffer, sizeof (buffer), fp))
  {
    ++linecnt;

    if (!hexfileHexToBin (&memImage, buffer, &segment, &memImageLen, unusedFill))
    {
      printf ("Error in line %d of file \"%s\"\n", linecnt, filename);
      fclose (fp);
      free (memImage);
      return 0;
    }
  }

  fclose (fp);

  //
  //  After we load the image, round the image up to the next power of two boundary.
  //  I.e., if it's < 8K, round to 8K.  If it's > 8K, round to 16K, etc.
  //
  {
    int i;

    for (newSize = 0, i = 0x0001; i <= 0x8000; i <<= 1)
    {
      if (i == memImageLen)
        break;
      else if (i > memImageLen)
      {
        newSize = i;
        break;
      }
    }
  }

  if (newSize)
  {
    if ((memImage = (unsigned char *) realloc (memImage, newSize)) == NULL)
    {
      printf ("Cannot reallocate memory: %s\n", strerror (errno));
      return 0;
    }

    memset (memImage + memImageLen, unusedFill, newSize - memImageLen);

    memImageLen = newSize;
  }

  *memory = memImage;
  *memorySize = memImageLen;

  return 1;
}
