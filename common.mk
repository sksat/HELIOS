ARCH		:= x86_64
BOOT_TYPE	:= UEFI

GIT_COMMIT_ID  := $(shell git log -1 --format='%H')
GIT_COMMIT_DATE:= $(shell git log -1 --format='%ad')

CC			:= clang
CXX			:= clang++
LD			:= lld-link

CFLAGS		= -Wall -nostdlibinc \
			  -DGIT_COMMIT_ID="\"$(GIT_COMMIT_ID)\"" \
			  -DGIT_COMMIT_DATE="\"$(GIT_COMMIT_DATE)\""
LDFLAGS		= -nodefaultlib

ifeq ($(BOOT_TYPE),UEFI)
	CFLAGS	+= -target x86_64-pc-win32-coff
	LDFLAGS	+= -subsystem:efi_application -dll
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) -std=c++1z $(CFLAGS) -c -o $@ $<
