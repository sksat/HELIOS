#include <stdint.h>

namespace EFI {
	using status = uint64_t;
	using handle = void*;
	using uintn_t= uint64_t;
	using intn_t = int64_t;
	using char16 = uint16_t;
	using paddr_t= uint64_t; // PHYSICAL_ADDRESS

	using BOOL = unsigned char;

	enum class ResetType {
		Cold,
		Warm,
		Shutdown,
		PlatformSpecific
	};

	enum class MemoryType {
		Reserved,
		LoaderCode,
		LoaderData,
		BootServicesCode,
		BootServicesData,
		RuntimeServicesCode,
		RuntimeServicesData,
		Conventional,
		Unusable,
		ACPIReclaim,
		ACPIMemoryNVS,
		MappedIO,
		MappedIOPortSpace,
		PalCode,
		Max
	};

	enum class TimerDelay {
		Cancel,
		Periodic,
		Relative
	};

	struct TABLE_HEADER;
	struct INPUT_KEY;
	struct SIMPLE_TEXT_INPUT_PROTOCOL;
	struct SIMPLE_TEXT_OUTPUT_PROTOCOL;
	struct RUNTIME_SERVICES;
	struct MEMORY_DESCRIPTOR;
	struct DEVICE_PATH_PROTOCOL;
	struct DEVICE_PATH_TO_TEXT_PROTOCOL;
	struct GUID;
	struct BOOT_SERVICES;
	struct SYSTEM_TABLE;

	struct GRAPHICS_OUTPUT_BLT_PIXEL;
	struct GRAPHICS_OUTPUT_PROTOCOL;

	struct LOADED_IMAGE_PROTOCOL;

	using IMAGE_UNLOAD = status (*)(handle image_handle);

	// using
	using color_t = GRAPHICS_OUTPUT_BLT_PIXEL;

	struct TABLE_HEADER {
		uint64_t signature;
		uint32_t revision;
		uint32_t size;
		uint32_t crc32;
		uint32_t reserved;
	};

	struct INPUT_KEY {
		uint16_t scan_code;
		uint16_t unicode_char;
	};

	struct SIMPLE_TEXT_INPUT_PROTOCOL {
		uint64_t _buf;
		status (*read_key_stroke)(SIMPLE_TEXT_INPUT_PROTOCOL*, INPUT_KEY*);
		void *wait_for_key;
	};

	struct SIMPLE_TEXT_OUTPUT_PROTOCOL {
		uint64_t _buf;
		status (*output_string)(SIMPLE_TEXT_OUTPUT_PROTOCOL*, wchar_t*);
		status (*test_string)(SIMPLE_TEXT_OUTPUT_PROTOCOL*, wchar_t*);
		status (*query_mode)(SIMPLE_TEXT_OUTPUT_PROTOCOL*, wchar_t*, uint64_t *columns, uint64_t *rows);
		status (*set_mode)(SIMPLE_TEXT_OUTPUT_PROTOCOL*, uint64_t);
		status (*set_attribute)(SIMPLE_TEXT_OUTPUT_PROTOCOL*, uint64_t attribute);
		status (*clear_screen)(SIMPLE_TEXT_OUTPUT_PROTOCOL*);
		uint64_t _buf2[2];
		struct MODE {
			int max_mode;
			int mode;
			int attribute;
			int cursor_colmn;
			int cursor_row;
			uint8_t cursor_visible;
		} *mode;
	};

	struct RUNTIME_SERVICES {
		char _buf_rs1[24];
		uint64_t _buf_rs2[4];
		uint64_t _buf_rs3[2];
		uint64_t _buf_rs4[3];
		uint64_t _buf_rs5;
		void (*reset_system)(ResetType, uint64_t reset_status, uint64_t data_size, void*);
	};

	struct MEMORY_DESCRIPTOR {
		unsigned int type;
		uint64_t physical_start;
		uint64_t virtual_start;
		uint64_t num_of_pages;
		uint64_t attribute;
	};

	struct DEVICE_PATH_PROTOCOL {
		unsigned char type;
		unsigned char sub_type;
		unsigned char length[2];
	};

	struct DEVICE_PATH_TO_TEXT_PROTOCOL {
		char16* (*convert_devnode2text)();
		char16* (*convert_devpath2text)(const DEVICE_PATH_PROTOCOL *device_path, BOOL display_only, BOOL allow_shortcuts);
	};

	struct GUID {
		unsigned int data1;
		unsigned short data2;
		unsigned short data3;
		unsigned char data4[8];
	};

	// GUID
	namespace PROTOCOL_GUID {
		inline GUID GRAPHICS_OUTPUT = {
			0x9042a9de, 0x23dc, 0x4a38,
            {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
		};
		inline GUID LOADED_IMAGE = {
			0x5b1b31a1, 0x9562, 0x11d2,
			{0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
		};
		inline GUID DEVICE_PATH_TO_TEXT = {
			0x8b843e20, 0x8132, 0x4852,
			{0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c}
		};
	}

	struct BOOT_SERVICES {
		char _buf[24];
		uint64_t _buf2[2];
		uint64_t _buf3[2];
		uint64_t (*get_memory_map)(uint64_t *memory_map_size, MEMORY_DESCRIPTOR*,
									uint64_t *map_key, uint64_t *descriptor_size,
									uint32_t *descriptor_version);
		uint64_t (*allocate_pool)(MemoryType, uint64_t, void**);
		uint64_t (*free_pool)(void *buf);
		using notify_func = void (*)(void *event, void *context);
		uint64_t (*create_event)(unsigned int type, uint64_t notify_tpl,
									notify_func, void *notify_context, void *event);
		uint64_t (*set_timer)(void *event, TimerDelay, uint64_t trigger_time);
		uint64_t (*wait_for_event)(uint64_t num_of_events, void **event, uint64_t *index);
		uint64_t _buf4[3];
		uint64_t _buf5[9];
		uint64_t (*load_image)(unsigned char boot_policy, void *parent_iamge_handle,
								DEVICE_PATH_PROTOCOL*, void *source_buf,
								uint64_t source_size, void **image_handle);
		uint64_t (*start_image)(void *image_handle, uint64_t *exit_data_size, unsigned short **exit_data);
		uint64_t _buf6[2];
		uint64_t (*exit_boot_services)(void *iamge_handle, uint64_t map_key);
		uint64_t _buf7[2];
		uint64_t (*set_watchdog_timer)(uint64_t timeout, uint64_t watchdog_code,
										uint64_t data_size, unsigned short *watchdog_data);
		uint64_t _buf8[2];
		enum class OpenProtocol : uint32_t {
			ByHandleProtocol	= 0x00000001,
			GetProtocol			= 0x00000002,
			TestProtocol		= 0x00000004,
			ByChildController	= 0x00000008,
			ByDriver			= 0x00000010,
			Exclusive			= 0x00000020
		};
		uint64_t (*open_protocol)(void *handle, GUID *protocol, void **interface,
									void *agent_handle, void *controller_handle, unsigned int attributes);
		uint64_t _buf9[2];
		uint64_t _buf10[2];
		uint64_t (*locate_protocol)(GUID *protocol, void *registration, void **interface);
		uint64_t _buf10_2[2];
		uint64_t _buf11;
		void (*copy_mem)(void *dest, void *src, uint64_t length);
		void (*set_mem)(void *buf, uint64_t size, unsigned char val);
		uint64_t _buf12;
	};

	struct SYSTEM_TABLE {
		TABLE_HEADER header;
		wchar_t *firmware_vendor;
		uint32_t firmware_revision;
		handle console_in_handle;
		SIMPLE_TEXT_INPUT_PROTOCOL *con_in;
		handle console_out_handle;
		SIMPLE_TEXT_OUTPUT_PROTOCOL *con_out;
		uint64_t _buf[2];
		RUNTIME_SERVICES *runtime_services;
		BOOT_SERVICES *boot_services;
	};

	struct GRAPHICS_OUTPUT_BLT_PIXEL {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t reserved;
	};

	enum class GRAPHICS_OUTPUT_BLT_OPERATION {
		VideoFill,
		VideoToBltBuffer,
		BufferToVideo,
		GraphicsBltOperationMax
	};

	struct GRAPHICS_OUTPUT_PROTOCOL {
		struct MODE {
			uint32_t max_mode;
			uint32_t mode;
			struct INFO {
				uint32_t version;
				uint32_t horizontal_resolution;
				uint32_t vertical_resolution;
				enum PIXEL_FORMAT {
					RGB, // PixelRedGreenBlueReserved8BitPerColor
					BGR, // PixelBlueGreenRedReserved8BitPerColor
					BitMask,
					BitOnly,
					PixelFormatMax
				} pixel_format;
				struct PIXEL_BITMASK {
					uint32_t red;
					uint32_t green;
					uint32_t blue;
					uint32_t reserved;
				} pixel_info;
				uint32_t pixels_per_scanline;
			} *info;
			uintn_t size_of_info;
			paddr_t frame_buf_base;
			uintn_t frame_buf_size;
		};

		status (*query_mode)(GRAPHICS_OUTPUT_PROTOCOL*,
							uint32_t mode_number, uintn_t *size_of_info,
							MODE::INFO **info);
		status (*set_mode)(GRAPHICS_OUTPUT_PROTOCOL*, uint32_t mode_number);
		status (*blt)(GRAPHICS_OUTPUT_PROTOCOL*, GRAPHICS_OUTPUT_BLT_PIXEL *buf,
						GRAPHICS_OUTPUT_BLT_OPERATION blt_operation,
						uintn_t src_x, uintn_t src_y,
						uintn_t dest_x, uintn_t dest_y,
						uintn_t width, uintn_t height,
						uintn_t delta);
		MODE *mode;
	};

	struct LOADED_IMAGE_PROTOCOL {
		uint32_t revision;
		handle parent_handle;
		SYSTEM_TABLE *system_table;
		handle device_handle;
		DEVICE_PATH_PROTOCOL *file_path;
		void *reserved;
		uint32_t load_option_size;
		void *load_options;
		void *image_base;
		uint64_t image_size;
		MemoryType image_code_type;
		MemoryType image_data_type;
		IMAGE_UNLOAD unload;
	};
}
