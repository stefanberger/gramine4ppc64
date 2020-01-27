include ../../Scripts/Makefile.Arch
include ../../Scripts/Makefile.configs
include ../../Scripts/Makefile.rules

ifeq ($(ARCH),$(filter $(ARCH),x86_64 ppc64))
files_to_build += user_start.o user_shared_start.o
endif

.PHONY: all test
all: $(files_to_build)
test:

user_start.o: user_start-$(ARCH).S
	$(call cmd,as_o_S)

ASFLAGS-user_shared_start.o = -DSHARED -fPIC
user_shared_start.o: user_start-$(ARCH).S
	$(call cmd,as_o_S)

include $(wildcard *.d)

.PHONY: clean
clean:
	$(RM) $(files_to_build) *.d

.PHONY: distclean
distclean: clean
