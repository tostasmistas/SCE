# Mostly written by Jonathan Larmour, Red Hat, Inc.
# Reference to ecos.mak added by John Dallaway, eCosCentric Limited, 2003-01-20
# This file is in the public domain and may be used for any purpose

# Usage:   make INSTALL_DIR=/path/to/ecos/install

INSTALL_DIR=$$(INSTALL_DIR) # override on make command line

include $(INSTALL_DIR)/include/pkgconf/ecos.mak

XCC           = $(ECOS_COMMAND_PREFIX)gcc
XCXX          = $(XCC)
XLD           = $(XCC)

CFLAGS        = -I$(INSTALL_DIR)/include
CXXFLAGS      = $(CFLAGS)
LDFLAGS       = -nostartfiles -L$(INSTALL_DIR)/lib -Ttarget.ld

#####
OBJS= main.o threads.o threadInterface.o threadCommunication.o commandFunctions.o threadProcessing.o

# RULES

.PHONY: all clean

all: main

clean:
	-rm -f main $(OBJS)

%.o: %.c
	$(XCC) -c -o $*.o $(CFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cxx
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.C
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cc
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

main: $(OBJS)
	$(XLD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) -o $@ $(OBJS)
