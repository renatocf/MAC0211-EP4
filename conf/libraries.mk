# LIBRARIES ############################################################
ARLIB := getopt.c
SOLIB := list.c 

SOFLAGS := -shared -Wl,-rpath=lib
ARFLAGS := -rcv

LIBS    := $(patsubst %.c,lib%.a,$(ARLIB)) \
           $(patsubst %.c,lib%.so,$(SOLIB))
