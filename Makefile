#######################################################################
#							PREDIRECTIVES                             #
#######################################################################
ALL: all

#######################################################################
#                             OPTIONS                                 #
#######################################################################

# Binary name (your program's name):
BIN := Canoagem

# Flags for compilation and linkage
CFLAGS  += -ansi -Wall -pedantic -g
LDFLAGS += -lm -lallegro -lallegro_primitives

# Boolean options ('true' or 'false')
B_PROFILE = 'false'
B_INSTALL = 'true'
B_ANALYSE = 'true'
B_STATLIB = 'true'
B_SHRDLIB = 'true'

# Options to installation
ICON 	:= Canoagem.png
DESKTOP := Canoagem.desktop

# Own defined directives
EXCLUDE_SRC :=
-include compile.mk

########################################################################
#                        AUTOMATIC CONFIGS                             #
########################################################################

# Saving flags for names being used later
U_CFLAGS  := $(CFLAGS)
U_LDFLAGS := $(LDFLAGS)

# PROGRAMS #############################################################
# The following programs are defined with the base in common avaiable
# programs in a modern Linux distribuition (as Ubuntu) with GNU Project
# programs also installed (as flex and bison).

# If you desire to change some of them, redefine the following names
# in a file called 'programs.mk' in your conf dir (see below).

AR    := ar
CC    := gcc
CP    := cp -f
RM    := rm -f
SED   := sed
FMT   := fmt -1
CAT   := cat
LEX   := flex 
YACC  := bison -v -t
PROF  := gprof
FIND   = find $(FDIR) -type d
MKDIR := mkdir -p
RMDIR := rmdir --ignore-fail-on-non-empty

MAKE  += --no-print-directory
-include $(CONFDIR)/programs.mk


# DIRECTORIES ##########################################################
# To a multiple directory project, create a file 'derectories.mk' in
# your conf directory, redefining the following dir names by the ones   
# you desire. 

# We suggest the names:
# src, obj, bin, lib, doc, conf, test, include, install, data

SRCDIR  := .
OBJDIR  := .
BINDIR  := .
LIBDIR  := .
DOCDIR  := . 
CONFDIR := conf
TESTDIR := .
HEADDIR := .
INSTDIR := .
DATADIR := .

-include $(CONFDIR)/directories.mk
VPATH = $(CONFDIR):$(SRCDIR):$(LIBDIR):$(BINDIR):$(TESTDIR):$(HEADDIR)


# SOURCE ###############################################################
PARSER  := $(notdir $(shell ls $(SRCDIR)/*.y))
SCANNER := $(notdir $(shell ls $(SRCDIR)/*.l))

SRC := $(notdir $(shell ls $(SRCDIR)/*.c))
SRC += $(patsubst %.y,%.tab.c,$(PARSER))
SRC += $(patsubst %.l,%.yy.c, $(SCANNER))
LIB := $(CONFDIR)/libraries.mk
DEP := $(addprefix $(CONFDIR)/,$(SRC:.c=.d))

-include $(LIB)
OBJ := $(filter-out $(ARLIB) $(SOLIB),$(SRC)) # Tira bibliotecas
OBJ := $(filter-out $(EXCLUDE_SRC),$(SRC))    # Tira regras próprias
OBJ := $(patsubst %.c,%.o,$(OBJ))             # Substitui .c por .o
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))         # Adiciona diretório

# PARSER  := 
# SCANNER := $(notdir $(shell ls $(SRCDIR)/*.l))
# OBJ += $(addprefix $(OBJDIR)/,$(PARSER:.l=.yy.o))
# OBJ += $(addprefix $(OBJDIR)/,$(SCANNER:.y=.tab.o))


# COMPILATION ##########################################################
FDIR = $(HEADDIR) # Gerando diretórios
CLIBS  := -I. $(patsubst %,-I%,$(filter-out .%,$(shell $(FIND))))
CFLAGS := $(U_CFLAGS)


# PROFILE ##############################################################
ifeq ($(B_PROFILE),'true')
CFLAGS  += -pg -fprofile-arcs
LDFLAGS += -pg -fprofile-arcs
endif


# LINKAGE ##############################################################
FDIR = $(LIBDIR) # Gerando bibliotecas
LDLIBS   = -L. $(patsubst %,-L%,$(filter-out .%,$(shell $(FIND))))

# Flags para processo de ligação 
LDFLAGS := -Wl,-rpath,$(LIBDIR) 
LDFLAGS += $(filter -l%,$(patsubst lib%.a,-l%,$(LIBS))) \
 		   $(filter -l%,$(patsubst lib%.so,-l%,$(LIBS)))
LDFLAGS += $(U_LDFLAGS)


# INSTALL ##############################################################
ifeq ($(B_INSTALL),'true')

# Messages
INSTALL_SUCCESS   := "$(BIN) successfully installed!"
UNINSTALL_MESSAGE := "Removing $(BIN)..."

# Check whether user is root or not 
# (for local/system installation)
USER = $(shell whoami)

ifeq ($(USER),root) #global
INSTBIN = /usr/bin
INSTAPP = /usr/share/applications
INSTICO = /usr/share/icons
else 				#local
INSTBIN = $(HOME)/.local/bin
INSTAPP = $(HOME)/.local/share/applications
INSTICO = $(HOME)/.local/share/icons
endif

endif # B_INSTALL == 'true'

########################################################################
#                            INSTALLATION                              #
########################################################################

.PHONY: all
all: Makefile $(DEP) $(addprefix $(BINDIR)/,$(BIN))
-include $(DEP)

ifeq ($(B_INSTALL), 'true')
.PHONY: install
install:
	@ echo "Configuring executable..."
	@ chmod u+x $(BINDIR)/$(BIN)
	@ $(MKDIR) $(INSTBIN)
	@ $(CP) $(BINDIR)/$(BIN) $(INSTBIN)
	@ echo "Creating desktop icon..."
	@ $(MKDIR) $(INSTAPP)
	@ $(SED) -e 's|\$$(INSTBIN)|$(INSTBIN)|' \
	 	-e 's|\$$(BIN)|$(BIN)|' \
	 	-e 's|\$$(INSTICO)|$(INSTICO)|' \
	 	-e 's|\$$(ICON)|$(ICON)|' \
	 	< $(INSTDIR)/$(DESKTOP) \
	 	> $(INSTAPP)/$(DESKTOP)
	@ $(MKDIR) $(INSTICO)
	@ $(CP) $(INSTDIR)/$(ICON) $(INSTICO)/
	@ echo $(INSTALL_SUCCESS)
endif

ifeq ($(B_DEBUG),'true')
.PHONY: debug
debug:
	$(PROF) $(BIN) gmon.out > $(DATADIR)/$@.txt
endif

.PHONY: doc
doc:
	$(MAKE) -C $(DOCDIR)

########################################################################
#                           SYSTEM CLEAN                               #
########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so
	$(RM) $(BINDIR)/gmon.out $(DATADIR)/debug.txt $(OBJDIR)/*.gcda
	$(RM) $(SRCDIR)/*~ $(HEADDIR)/*~ 
	$(RM) $(SRCDIR)/*.yy.c $(HEADDIR)/*.yy.h
	$(RM) $(SRCDIR)/*.tab.c $(HEADDIR)/*.tab.h
	$(RM) $(CONFDIR)/*.d
	-$(RMDIR) $(OBJDIR) 2> /dev/null

.PHONY: distclean
distclean:
	$(RM) $(BINDIR)/$(BIN) $(CONFDIR)/*.d
	-$(RMDIR) $(BINDIR) $(LIBDIR) 2> /dev/null

ifeq ($(B_INSTALL), 'true')
.PHONY: uninstall
uninstall:
	@ echo $(UNINSTALL_MESSAGE)
	@ $(RM) $(INSTAPP)/$(DESKTOP)
	@ $(RM) $(INSTICO)/$(ICON)
	@ $(RM) $(INSTBIN)/$(BIN)
	@ echo "Done."
endif

########################################################################
#                               BUILD                                  #
########################################################################

# EXECUTABLE ###########################################################
$(BINDIR)/$(BIN): $(OBJ) | $(LIBS) $(BINDIR)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)


$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) $(CFLAGS) -c $< -o $@

$(OBJ): | $(OBJDIR)
$(LIBS): | $(LIBDIR)


# DEPENDENCIES #########################################################
$(CONFDIR)/%.d: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) -MM $< 1> $@
	@$(CP) $@ $@.tmp
	@$(SED) -e 's/.*:/$(OBJDIR)\/$*.o:/' -i $@
	@$(SED) -e 's/.*://' -e 's/\\$$//' < $@.tmp | $(FMT) | \
	  $(SED) -e 's/^ *//' -e 's/$$/:/' >> $@
	@$(RM) $@.tmp


# STATIC LIBRARIES #####################################################
ifeq ($(B_STATLIB), 'true')
lib%.a: $(OBJDIR)/$(notdir %.o)
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $<
endif


# SHARED LIBRARIES #####################################################
ifeq ($(B_SHRDLIB), 'true')
lib%.so: $(SRCDIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(CLIBS) -c $< -o $(OBJDIR)/$*.o
	$(CC) -o $(LIBDIR)/$@ $(SOFLAGS) $(OBJDIR)/$*.o 
endif


# SCANNER & PARSER #####################################################
ifeq ($(B_ANALYSE), 'true')
LDFLAGS += -lfl

%.yy.o: $(addprefix $(SRCDIR)/,%.yy.c) $(addprefix $(SRCDIR)/,%.tab.c)
	$(CC) $(CFLAGS) -c $<

%.tab.o: $(addprefix $(SRCDIR)/,%.tab.c)
	$(CC) $(CFLAGS) -c $<

$(SRCDIR)/%.yy.c: $(addprefix $(SRCDIR)/,%.l) $(addprefix $(HEADDIR)/,$(PARSER:.y=.tab.h))
	$(LEX) --header-file=$(HEADDIR)/$*.yy.h -o $@ $<

$(SRCDIR)/%.tab.c: $(addprefix $(SRCDIR)/,%.y)
	$(YACC) --defines=$(HEADDIR)/$*.tab.h -o $@ $<

$(HEADDIR)/%.yy.h: $(SRCDIR)/%.yy.c $(SRCDIR)/%.l
$(HEADDIR)/%.tab.h: $(SRCDIR)/%.tab.c $(SRCDIR)/%.y
endif


# TESTS ################################################################
%: $(TESTDIR)/%.c
	$(CC) $< $(CFLAGS) $(CLIBS) -o $(BINDIR)/test$* $(LDLIBS) $(LDFLAGS)

########################################################################
#                            GENERATE DIRS                             #
########################################################################

ifneq ($(SRCDIR),.)
$(SRCDIR):
	@ echo Creating directory for source "$@"
	-$(MKDIR) $@
endif

ifneq ($(OBJDIR),.)
$(OBJDIR):
	@ echo Creating directory for objects "$@"
	-$(MKDIR) $@
endif

ifneq ($(BINDIR),.)
$(BINDIR):
	@ echo Creating directory for binaries "$@"
	-$(MKDIR) $@
endif

ifneq ($(LIBDIR),.)
$(LIBDIR):
	@ echo Creating directory for libraries "$@"
	-$(MKDIR) $@
endif

# ifneq ($(DOCDIR),.)
# $(DOCDIR):
# 	@ echo Creating directory for documents "$@"
# 	-$(MKDIR) $@
# endif

ifneq ($(CONFDIR),.)
$(CONFDIR):
	@ echo Creating directory for config files "$@"
	-$(MKDIR) $@
endif

ifneq ($(TESTDIR),.)
$(TESTDIR):
	@ echo Creating directory for tests "$@"
	-$(MKDIR) $@
endif

ifneq ($(HEADDIR),.)
$(HEADDIR):
	@ echo Creating directory for headers "$@"
	-$(MKDIR) $@
endif

# ifneq ($(INSTDIR),.)
# $(INSTDIR):
# 	@ echo Creating directory with installation files "$@"
# 	-$(MKDIR) $@
# endif

ifneq ($(DATADIR),.)
$(DATADIR):
	@ echo Creating directory for headers "$@"
	-$(MKDIR) $@
endif
########################################################################
