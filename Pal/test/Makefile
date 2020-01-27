include ../../Scripts/Makefile.Arch
include ../../Scripts/Makefile.configs
include ../../Scripts/Makefile.rules

CFLAGS += -fno-builtin -nostdlib -no-pie \
	  -I../include/pal -I../include/lib

executables = \
	HelloWorld \
	File \
	Failure \
	Thread \
	Event \
	Process \
	Memory \
	Pipe \
	Tcp \
	Udp \
	Yield \
	Server \
	Wait \
	HandleSend \
	Select \
	Sleep \
	Pie

ifeq ($(ARCH),x86_64)
executables += \
	Exception \
	Fork \
	Segment
endif

manifests = manifest

target = \
	$(executables) \
	$(manifests)

include ../../Scripts/Makefile.manifest

graphene_lib = .lib/graphene-lib.a
pal_lib = ../../Runtime/libpal-$(PAL_HOST).so

ifneq ($(findstring linux,$(SYS)),linux)
$(error unsupported platform. $(SYS))
endif

.PHONY: all
all: pal_loader $(target) $(call expand_target_to_sig,$(target)) $(call expand_target_to_sgx,$(target))

.PHONY: sgx-tokens
sgx-tokens: $(call expand_target_to_token,$(target))

pal_loader:
	ln -sf ../../Runtime/pal_loader

.PHONY: crt_init-recurse
crt_init-recurse:
	$(MAKE) -C ../crt_init all

../crt_init/user_start.o: crt_init-recurse
	@true

CFLAGS-Pie = -fPIC -pie
LDLIBS += $(graphene_lib) $(pal_lib) ../crt_init/user_start.o
$(executables): %: %.c $(LDLIBS)
	$(call cmd,csingle)

$(graphene_lib):
	$(MAKE) -C ../lib target=$(abspath .lib)/

ifeq ($(filter %clean,$(MAKECMDGOALS)),)
-include $(addsuffix .d, $(executables)) $(addsuffix .i.d, $(executables)) $(addsuffix .s.d, $(executables))
endif

.PHONY: clean
clean:
	$(RM) -r pal_loader $(target) .lib *.cached *.sig *.token *.manifest.sgx \
	       $(addsuffix .d, $(executables)) $(addsuffix .i.d, $(executables)) \
	       $(addsuffix .s.d, $(executables)) \
	       $(addsuffix .manifest.sgx.d,$(executables)) \
		   .output.*

.PHONY: distclean
distclean: clean
