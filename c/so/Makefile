#
# Makefile for libais.so
#
# Copyright 2009 by Brian C. Lane <bcl@brianlane.com>
# All Rights Reserved
#
# Please note that this Makefile *needs* GNU make. BSD make won't do.
#

VERSION = 1.9

CC	= gcc
CFLAGS	= -I../src -g -Wall -fPIC
#LIBS	=
SRC	= ../src/

OBJS		=	$(SRC)nmea.o $(SRC)vdm_parse.o $(SRC)sixbit.o $(SRC)imo.o $(SRC)seaway.o
HDRS		= 	$(SRC)nmea.h $(SRC)vdm_parse.h $(SRC)sixbit.h $(SRC)portable.h $(SRC)imo.h $(SRC)seaway.h


# -----------------------------------------------------------------------
# Sort out what operating system is being run and modify CFLAGS and LIBS
#
# If you add a new OSTYPE here please email it to me so that I can add
# it to the distribution in the next release
# -----------------------------------------------------------------------
SYSTYPE := $(shell uname -s)

ifeq ($(SYSTYPE), Linux)
  CFLAGS += -DLINUX
endif

ifeq ($(SYSTYPE), SunOS)
  CFLAGS += -DSOLARIS
  LIBS   += -lposix4
endif

ifeq ($(SYSTYPE), FreeBSD)
  CFLAGS += -DFREEBSD
endif

ifeq ($(SYSTYPE), OpenBSD)
  CFLAGS += -DOPENBSD
endif

# Untested, but should work.
ifeq ($(SYSTYPE), NetBSD)
  CFLAGS += -DNETBSD
endif

ifeq ($(SYSTYPE), Darwin)
  CFLAGS += -DDARWIN
endif

ifeq ($(SYSTYPE), AIX)
  CFLAGS += -DAIX
endif

ifeq ($(LOCK), yes)
  CFLAGS += -DLOCKDEV
  LIBS   += -llockdev
endif
# -----------------------------------------------------------------------

all:		help

help:
	@echo "  SYSTYPE = $(SYSTYPE)"
	@echo "  CFLAGS = $(CFLAGS)"
	@echo "  LIBS   = $(LIBS)"
	@echo ""
	@echo "make linux - build Linux shared library"
	@echo "make osx - build OSX dynamic library"
	@echo ""
	@echo "Please note: You must use GNU make to compile this project"
	@echo ""

linux:	$(OBJS) $(HDRS) $(OBJS)
		$(CC) -shared -Wl,-soname,libais.so.1 -o libais.so.$(VERSION) $(OBJS)

osx:	$(OBJS) $(HDRS) $(OBJS)
		$(CC) -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,libais.1.dylib -o libais.1.dylib $(OBJS)

# Clean up the object files and the sub-directory for distributions
clean:
		rm -f *~
		rm -f $(OBJS)
		rm -f core *.asc
		rm -f libais.so.$(VERSION)
		rm -f libais.1.dylib

