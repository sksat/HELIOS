using CHAR16	= unsigned short;
using EFI_STATUS= long long;
using EFI_HANDLE= void*;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
using EFI_TEXT_STRING = EFI_STATUS (*)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void			*a;
    EFI_TEXT_STRING	OutputString;
};

struct EFI_SYSTEM_TABLE {
	char							a[52];
    EFI_HANDLE						ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*ConOut;
};

void efi_main(void *ImageHandle, EFI_SYSTEM_TABLE *system_table){
	system_table->ConOut->OutputString(system_table->ConOut, (CHAR16*)L"Hello, UEFI!");
	while(true);
}
