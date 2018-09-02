OVMF		= OVMF.fd
OVMF_ZIP	= OVMF-X64-r15214.zip
OVMF_ZIP_URL= http://downloads.sourceforge.net/project/edk2/OVMF/$(OVMF_ZIP)

ovmf: $(OVMF_ZIP) $(OVMF)

$(OVMF_ZIP):
	wget $(OVMF_ZIP_URL)

$(OVMF):
	unzip $(OVMF_ZIP) $@
