#include "efi.h"

EFI::SYSTEM_TABLE *ST;
EFI::GRAPHICS_OUTPUT_PROTOCOL *GOP;

void draw_pixel(const size_t &x, const size_t &y, const EFI::GRAPHICS_OUTPUT_BLT_PIXEL &col){
	const auto& hr	= GOP->mode->info->horizontal_resolution;
	const auto& base= GOP->mode->frame_buf_base;
	EFI::GRAPHICS_OUTPUT_BLT_PIXEL *p = (EFI::GRAPHICS_OUTPUT_BLT_PIXEL*) (base + ((hr * y) + x)*4);
	p->red	= col.red;
	p->green= col.green;
	p->blue	= col.blue;
	p->reserved = col.reserved;
}

void boxfill(const size_t x0, size_t y0,
			const size_t x1, const size_t y1, const EFI::color_t &col){
	for(size_t y=y0; y<y1; y++){
		for(size_t x=x0; x<x1; x++)
			draw_pixel(x, y, col);
	}
}

void panic(){
	EFI::color_t col;
	col.red = 0x00;
	col.green= 0x00;
	col.blue = 0xff;
	boxfill(0, 0, GOP->mode->info->horizontal_resolution, GOP->mode->info->vertical_resolution, col);
}

template<typename T>
void puts(T *str){
	auto& con_out = ST->con_out;
	con_out->output_string(con_out, (wchar_t*)(str));
}

void efi_main(void *image_handle, EFI::SYSTEM_TABLE *system_table){
	using namespace EFI;

	ST = system_table;

	auto con_out = system_table->con_out;
	con_out->clear_screen(con_out);
	puts(L"booting HELIOS...\n\r");
	puts(L"commit: " GIT_COMMIT_ID "(" GIT_COMMIT_DATE ")\n\r");

	auto bootsrv = system_table->boot_services;
	bootsrv->locate_protocol(&PROTOCOL_GUID::GRAPHICS_OUTPUT, 0, (void**)&GOP);

	if(GOP->mode->info->pixel_format == GRAPHICS_OUTPUT_PROTOCOL::MODE::INFO::BGR){
		puts(L"BGR\r\n");
	}

	color_t col;
	col.red = 0xff;
	col.green=0x00;
	col.blue= 0x00;

//	boxfill(0, 0, 200, 200, col);

	LOADED_IMAGE_PROTOCOL *LIP;
	auto status = bootsrv->open_protocol(image_handle, &PROTOCOL_GUID::LOADED_IMAGE,
			(void**)&LIP, image_handle, 0, static_cast<uint32_t>(BOOT_SERVICES::OpenProtocol::GetProtocol));

	if(status) panic();

	DEVICE_PATH_TO_TEXT_PROTOCOL *DP2TP;
	status = bootsrv->locate_protocol(&PROTOCOL_GUID::DEVICE_PATH_TO_TEXT, 0, (void**)&DP2TP);
	if(status) panic();
	puts(L"LIP->file_path: ");
	puts(DP2TP->convert_devpath2text(LIP->file_path, 0, 0));
	puts(L"\r\n");

	DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
	status = bootsrv->locate_protocol(&PROTOCOL_GUID::DEVICE_PATH_FROM_TEXT, 0, (void**)&DPFTP);
	if(status) panic();

	auto dev_path = DPFTP->convert_text2devpath((char16*)L"\\test.efi");
	puts(L"dev_path: ");
	puts(DP2TP->convert_devpath2text(dev_path, 0, 0));

// fin
	while(true);
}
