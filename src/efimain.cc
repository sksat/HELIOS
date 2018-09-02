#include "efi.h"

EFI::GRAPHICS_OUTPUT_PROTOCOL *GOP;

void draw_pixel(uint64_t x, uint64_t y, EFI::GRAPHICS_OUTPUT_BLT_PIXEL &col){
	const auto& hr	= GOP->mode->info->horizontal_resolution;
	const auto& base= GOP->mode->frame_buf_base;
	EFI::GRAPHICS_OUTPUT_BLT_PIXEL *p = (EFI::GRAPHICS_OUTPUT_BLT_PIXEL*) (base + (hr * y) + x);
	p->red	= col.red;
	p->green= col.green;
	p->blue	= col.blue;
	p->reserved = col.reserved;
}

void efi_main(void *ImageHandle, EFI::SYSTEM_TABLE *system_table){
	auto con_out = system_table->con_out;
	con_out->clear_screen(con_out);
	con_out->output_string(con_out, (wchar_t*)L"booting HELIOS...\n\r");
	con_out->output_string(con_out, (wchar_t*)L"commit: " GIT_COMMIT_ID "(" GIT_COMMIT_DATE ")\n\r");

	EFI::GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38,
		{0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

	auto bootsrv = system_table->boot_services;
	bootsrv->locate_protocol(&gop_guid, 0, (void**)&GOP);

	EFI::GRAPHICS_OUTPUT_BLT_PIXEL red = { 0xff, 0x00, 0x00, 0x00 };
	for(size_t y=0;y<1200;y++){
		for(size_t x=0;x<400;x++){
			draw_pixel(x, y, red);
		}
	}

	con_out->output_string(con_out, (wchar_t*)L"loop");
	while(true);
}
