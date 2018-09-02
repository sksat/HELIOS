include ../common.mk

BOOTX64.EFI: efimain.o
	$(LD) \
		-subsystem:efi_application -nodefaultlib -dll \
		-entry:efi_main $^ -out:$@

clean:
	rm -f *.o *.lib *.EFI
