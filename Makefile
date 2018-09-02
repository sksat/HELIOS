include common.mk

MNT			:= mnt/

# emulator
QEMU		:= qemu-system-$(ARCH)
QEMU_ARGS	= -monitor stdio

# bios
OVMF		= bios/OVMF.fd
ifeq ($(BOOT_TYPE),UEFI)
	BIOS = $(OVMF)
	QEMU_ARGS += -drive file=fat:ro:$(MNT)
endif

ifdef BIOS
	QEMU_ARGS += -bios $(BIOS)
endif

default:
	make -C src
ifeq ($(BOOT_TYPE),UEFI)
	mkdir -p $(MNT)EFI/BOOT
	cp src/BOOTX64.EFI $(MNT)EFI/BOOT
endif

info:
	@echo "BOOT TYPE: $(BOOT_TYPE)"
	@echo "BIOS: $(BIOS)"

run:
	make
	$(QEMU) $(QEMU_ARGS)

clean:
	make -C src clean
	rm -rf $(MNT)
