#include "efi.h"

void efi_main(void *ImageHandle, EFI::SYSTEM_TABLE *system_table){
	auto con_out = system_table->con_out;
	con_out->clear_screen(con_out);
	con_out->output_string(con_out, (wchar_t*)L"Hello, UEFI!\n\rcommit: ");
	con_out->output_string(con_out, (wchar_t*)L"" GIT_COMMIT_ID "(" GIT_COMMIT_DATE ")");
	while(true);
}
