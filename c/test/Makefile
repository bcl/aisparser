#
# Makefile for aisparser test code
#
# Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
# All Rights Reserved
#
# Please note that this Makefile *needs* GNU make. BSD make won't do.
#

VERSION = 1.10

SRC	= ../src/
CC	= gcc
CFLAGS	= -I$(SRC) -g -Wall # -O2
#LIBS	=

OBJS		=	$(SRC)nmea.o $(SRC)vdm_parse.o $(SRC)sixbit.o $(SRC)seaway.o
OBJS		+=	$(SRC)imo.o $(SRC)access.o
OBJS		+=	$(SRC)test_nmea.o $(SRC)test_vdm_parse.o $(SRC)test_sixbit.o
OBJS		+=	$(SRC)test_seaway.o $(SRC)test_imo.o $(SRC)test_access.o
HDRS		=	$(SRC)nmea.h $(SRC)vdm_parse.h $(SRC)sixbit.h $(SRC)seaway.h
HDRS		+=  $(SRC)imo.h $(SRC)access.h
HDRS		+=	$(SRC)test_nmea.h $(SRC)test_vdm_parse.h $(SRC)test_sixbit.h
HDRS		+=	$(SRC)test_seaway.h $(SRC)test_imo.h $(SRC)test_access.h

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
	@echo -e "\tmake test\t- Build test version"
	@echo -e "\tmake speed\t- Build speed test version"
	@echo " "
	@echo ""
	@echo "Please note: You must use GNU make to compile this"
	@echo ""

all:		help

test:		$(OBJS) $(HDRS) $(OBJS) main.o
		$(CC) $(OBJS) main.o -o aisparse_test $(LIBS)

speed:		$(OBJS) $(HDRS) $(OBJS) speed.o
		$(CC) $(OBJS) speed.o -o speed_test $(LIBS)

# Clean up the object files and the sub-directory for distributions
clean:
		rm -f *~
		rm -f $(OBJS) main.o speed.o
		rm -f core *.asc
		rm -rf aisparse_test speed_test
