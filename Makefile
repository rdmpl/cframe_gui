# current project dir
PROJECT_DIR := $(shell pwd)

# add the modules you want to add to the project
MODULES     := gui

TESTDIR     := test/src
TESTMAIN    := $(wildcard $(TESTDIR)/*.c)
TESTOUT     := $(foreach f, $(TESTMAIN), $(basename $(notdir $(f))).out)
# virtual modules used for compile subdirs
LIBMOD      := $(patsubst %, lib%, $(MODULES))
CLEANMOD    := $(patsubst %, clean%, $(MODULES))

MAKESUBMOD   = if [ -f $(1)/Makefile ]; then \
	               make -C $(1) $(2) ; \
	           fi;
CFLAGS       = -I$(PROJECT_DIR)/gui/include/base
# CFLAGS      := $(foreach mod, $(MODULES), wildcard())
export CFLAGS
all: app

clean: $(CLEANMOD)
	@rm $(TESTOUT)
debug:
	@echo $(TESTMAIN)
	@echo $(TESTOUT)
app:$(LIBMOD) $(TESTOUT)
$(TESTOUT):%.out:$(TESTDIR)/%.c $(LIBMOD)
	$(CC) $(CFLAGS) -o $@ $< $(foreach mod, $(MODULES), -L./$(mod) -l$(mod))
# app: $(LIBMOD)
#	echo $(TESTMAIN)
#	$(foreach f, $(TESTMAIN), $(CC) $(CFLAGS) -o $(basename $(notdir $(f))) $(f) $(foreach mod, $(MODULES), -L./$(mod) -l$(mod)))
$(LIBMOD):lib%:%
#$(LIBMOD): lib%:%
	@$(call MAKESUBMOD, $<)

$(CLEANMOD): clean%:%
	@$(call MAKESUBMOD, $<, clean)

$(MODULES):
	@if [ ! -d $@ ]; then \
		mkdir $@; \
	fi
