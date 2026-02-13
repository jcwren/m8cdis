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
#include <ctype.h>
#include <assert.h>
#include "m8cdis.h"
#include "util.h"

//
//
//
int utilHtoi (char *s)
{
  char *t = s;
  int x = 0;
  int y = 1;

  if (*s == '0' && (s [1] == 'x' || s [1] == 'X'))
    t += 2;

  s += strlen (s);

  while (t <= --s) 
  {
    if ((tolower ((int) *s) == 'h') && !*(s + 1))
      continue;
    else if ('0' <= *s && *s <= '9')
      x += y * (*s - '0');
    else if ('a' <= *s && *s <= 'f')
      x += y * (*s - 'a' + 10);
    else if ('A' <= *s && *s <= 'F')
      x += y * (10 + *s - 'A');
    else
      return -1;

    y <<= 4;
  }

  return x;
}

//
//
//
void utilDeleteLeadingWhiteSpace (char *s)
{
  char *p = s;

  while (*p && isspace ((int) *p))
    p++;

  if (*p)
    memcpy (s, p, strlen(p) + 1);
}

void utilDeleteTrailingWhiteSpace (char *s)
{
  char *p = s + (strlen (s) - 1);

  while (p >= s && isspace ((int) *p))
    *p-- = '\0';
}
