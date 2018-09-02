#include <stdint.h>

using char16	= uint16_t;
using efi_status= uint64_t;
using efi_handle= void*;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	uint64_t _buf;
	efi_status (*output_string)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, wchar_t*);
};

struct EFI_SYSTEM_TABLE {
	char a[52];
	efi_handle console_out_handle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *con_out;
};
