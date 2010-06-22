#
#  $Id$
#  $Revision$
#  $Date$
#  $Author$
#  $HeadURL$
#

#include <stdio.h>
MACH := $(shell uname | sed y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/)

#
#  What we want the target named, and our list of source files
#
BINARY	= m8cdis
SRCS		= m8cdis.c disassem.c hexdump.c hexfile.c hinter.c mapfile.c processors.c usagemap.c util.c
OBJS		= m8cdis.o disassem.o hexdump.o hexfile.o hinter.o mapfile.o processors.o usagemap.o util.o
LLIBS	  = 
LIBDIRS = 
LOBJS		=

#
#  The compiler we'll use and any flags
#
CC			= gcc
CFLAGS	= -O2 -W -Wall -g -fomit-frame-pointer

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

#
#  Dependency for the binary is .depend and all the .o files.
#
$(BINARY):	.depend $(OBJS)
	$(CC) -o $(BINARY) $(OBJS) $(LOBJS) $(LIBDIRS) $(LLIBS)
	ctags $(SRCS) *.h
ifneq (,$(findstring linux,$(MACH)))
	  strip $(BINARY)
endif
ifneq (,$(findstring cygwin,$(MACH)))
	  strip $(BINARY).exe
endif

#
#  The .depend files contains the list of header files that the
#  various source files depend on.  By doing this, we'll only
#  rebuild the .o's that are affected by header files changing.
#
.depend:
	$(CC) -M $(SRCS) > .depend

#
#  Utility targets
#
dep:
	$(CC) -M $(SRCS) > .depend

clean:
	rm -f $(BINARY) $(BINARY).exe .depend *.o core tags ctags $(BINARY).exe.stackdump

propset:
	svn propset svn:keywords "Id Revision Author Date HeadURL" *.c *.h Makefile README

#
#  Include the .depend file so we have the list of dependencies
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif
