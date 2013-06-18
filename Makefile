#######################################################################
#                             OPTIONS                                 #
#######################################################################

# PROGRAMS #############################################################
AR := ar
CC := gcc
CP := cp -f
RM := rm -f
SED := sed
FMT := fmt -1
CAT := cat
LEX  := flex
YACC := bison
FIND = find $(FDIR) -type d
MAKE += --no-print-directory
MKDIR := mkdir -p
RMDIR := rmdir --ignore-fail-on-non-empty

# DIRECTORIES ##########################################################
SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib
DOCDIR := doc
CONFDIR := conf
TESTDIR := test
HEADDIR := include
INSTDIR := install

-include $(CONFDIR)/directories.mk
VPATH = $(CONFDIR):$(SRCDIR):$(LIBDIR):$(BINDIR):$(TESTDIR):$(HEADDIR)

# SOURCE ###############################################################
BIN := Canoagem
SRC := $(notdir $(shell ls $(SRCDIR)/*.c))
LIB := $(CONFDIR)/libraries.mk
DEP := $(addprefix $(CONFDIR)/,$(SRC:.c=.d))

-include $(LIB)
OBJ := $(filter-out $(ARLIB) $(SOLIB),$(SRC)) # Tira bibliotecas
OBJ := $(patsubst %.c,%.o,$(OBJ))             # Substitui .c por .o
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))         # Adiciona diretório

# INSTALL ##############################################################
USER     = $(shell whoami)
ICON 	:= Canoagem.png
DESKTOP := Canoagem.desktop

ifeq ($(USER),root)
INSTBIN = /usr/bin
INSTAPP = /usr/share/applications
INSTICO = /usr/share/icons
else
INSTBIN = $(HOME)/.local/bin
INSTAPP = $(HOME)/.local/share/applications
INSTICO = $(HOME)/.local/share/icons
endif

# COMPILATION ##########################################################
FDIR = $(HEADDIR) # Gerando diretórios
CLIBS  := -I. $(patsubst %,-I%,$(filter-out .%,$(shell $(FIND))))

# Flags para processo de compilação
CFLAGS := -ansi -Wall -pedantic -g -fPIC
CFLAGS  += -Wno-implicit-function-declaration

# LINKAGE ##############################################################
FDIR = $(LIBDIR) # Gerando bibliotecas
LDLIBS   = -L. $(patsubst %,-L%,$(filter-out .%,$(shell $(FIND))))

# Flags para processo de ligação
LDFLAGS := -lm
LDFLAGS += -lallegro -lallegro_primitives
LDFLAGS += -Wl,-rpath,$(LIBDIR)
LDFLAGS += $(filter -l%,$(patsubst lib%.a,-l%,$(LIBS))) \
 		   $(filter -l%,$(patsubst lib%.so,-l%,$(LIBS)))

########################################################################
#                            INSTALLATION                              #
########################################################################

.PHONY: all
all: $(DEP) $(addprefix $(BINDIR)/,$(BIN))
-include $(DEP)

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
	@ echo "Canoa successfully installed!"

.PHONY: doc
doc:
	$(MAKE) -C $(DOCDIR)

########################################################################
#                           SYSTEM CLEAN                               #
########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so
	$(RM) $(SRCDIR)/*~ $(HEADDIR)/*~
	$(RM) $(DEP)
	-$(RMDIR) $(OBJDIR) 2> /dev/null

.PHONY: distclean
distclean:
	$(RM) $(BINDIR)/$(BIN) $(CONFDIR)/*.d
	-$(RMDIR) $(BINDIR) $(LIBDIR) 2> /dev/null

.PHONY: uninstall
uninstall:
	@ echo "Removing Canoa..."
	@ $(RM) $(INSTAPP)/$(DESKTOP)
	@ $(RM) $(INSTICO)/$(ICON)
	@ $(RM) $(INSTBIN)/$(BIN)
	@ echo "Done."

########################################################################
#                               BUILD                                  #
########################################################################

# EXECUTABLE ###########################################################
$(BINDIR)/$(BIN): $(OBJ) | $(LIBS) $(BINDIR)
	echo $(LDLIBS)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) $(CFLAGS) -c $< -o $@

$(OBJ): | $(OBJDIR)
$(LIBS): | $(LIBDIR)

# DEPENDENCIES #########################################################
$(CONFDIR)/%.d: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) -MM $< 1> $@
	@$(CP)  $@ $@.tmp
	@$(SED) -e 's/.*:/$(OBJDIR)\/$*.o:/' -i $@
	@$(SED) -e 's/.*://' -e 's/\\$$//' < $@.tmp | $(FMT) | \
	  $(SED) -e 's/^ *//' -e 's/$$/:/' >> $@
	@$(RM) $@.tmp

# STATIC LIBRARIES #####################################################
lib%.a: $(OBJDIR)/$(notdir %.o)
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $<

# SHARED LIBRARIES #####################################################
lib%.so: $(SRCDIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(CLIBS) -c $< -o $(OBJDIR)/$*.o
	$(CC) -o $(LIBDIR)/$@ $(SOFLAGS) $(OBJDIR)/$*.o 

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
