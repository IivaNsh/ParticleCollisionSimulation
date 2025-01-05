# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  particels_box_config = debug
endif
ifeq ($(config),release)
  particels_box_config = release
endif

PROJECTS := particels_box

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

particels_box:
ifneq (,$(particels_box_config))
	@echo "==== Building particels_box ($(particels_box_config)) ===="
	@${MAKE} --no-print-directory -C . -f particels_box.make config=$(particels_box_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f particels_box.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   particels_box"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"