#include "efi.h"

void efi_main(void *ImageHandle, EFI_SYSTEM_TABLE *system_table){
	system_table->con_out->output_string(system_table->con_out, (wchar_t*)L"Hello, UEFI!");
	while(true);
}
