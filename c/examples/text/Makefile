#
# Makefile for ais_text
#
# Copyright 2006 by Brian C. Lane <bcl@brianlane.com>
# All Rights Reserved
#
# Please note that this Makefile *needs* GNU make. BSD make won't do.
#

VERSION = 0.1

CC	= gcc
CFLAGS	= -I../../src -g -Wall # -O2
#LIBS	=
SRC	= ../../src/

OBJS		=	main.o $(SRC)nmea.o $(SRC)vdm_parse.o $(SRC)sixbit.o
HDRS		= 	$(SRC)nmea.h $(SRC)vdm_parse.h $(SRC)sixbit.h $(SRC)portable.h


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


help:
	@echo "  SYSTYPE = $(SYSTYPE)"
	@echo "  CFLAGS = $(CFLAGS)"
	@echo "  LIBS   = $(LIBS)"
	@echo ""
	@echo "Pick one of the following targets:"
	@echo -e "\tmake ais_text\t- Build ais to text parser"
	@echo " "
	@echo ""
	@echo "Please note: You must use GNU make to compile this project"
	@echo ""

all:		help

ais_text:	$(OBJS) $(HDRS) $(OBJS)
		$(CC) $(OBJS) -o ais_text $(LIBS)

# Clean up the object files and the sub-directory for distributions
clean:
		rm -f *~
		rm -f $(OBJS)
		rm -f core *.asc
		rm -f ais_text

