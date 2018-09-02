ARCH		:= x86_64
BOOT_TYPE	:= UEFI

CC			:= clang
CXX			:= clang++
LD			:= lld-link

CFLAGS		= -Wall -nostdlibinc
LDFLAGS		= -nodefaultlib

ifeq ($(BOOT_TYPE),UEFI)
	CFLAGS	+= -target x86_64-pc-win32-coff
	LDFLAGS	+= -subsystem:efi_application -dll
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CFLAGS) -c -o $@ $<
