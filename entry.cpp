#include "workspace/includes.h"

// RAIN EXECUTOR - VER1 INJECTOR

// roka was here @r.oka
// verbum was here
// sufix was here

struct import_directory_table_entry
{
	std::uint32_t import_lookup_table_rva, time_date_stamp, forwarder_chain, name_, import_address_table_rva;
};

struct export_directory_table_entry
{
	std::uint32_t function_rva;
	std::uint16_t name_ordinal;
	std::uint32_t name_rva;
};

DWORD pasted_characteristic_conv( DWORD characteristics )
{
	bool executable = ( characteristics & IMAGE_SCN_MEM_EXECUTE ) != 0;
	bool readable = ( characteristics & IMAGE_SCN_MEM_READ ) != 0;
	bool writable = ( characteristics & IMAGE_SCN_MEM_WRITE ) != 0;

	if ( executable )
	{
		if ( writable && readable ) return PAGE_EXECUTE_READWRITE;
		if ( writable ) return PAGE_EXECUTE_WRITECOPY;
		if ( readable ) return PAGE_EXECUTE_READ;
		return PAGE_EXECUTE;
	}
	else
	{
		if ( writable && readable ) return PAGE_READWRITE;
		if ( writable ) return PAGE_WRITECOPY;
		if ( readable ) return PAGE_READONLY;
		return PAGE_NOACCESS;
	}
}

void shady_callback( std::uint32_t pid )
{
	HANDLE proc = OpenProcess( SYNCHRONIZE, FALSE, pid );
	DWORD waitResult = WaitForSingleObject( proc, INFINITE );
	TerminateProcess( GetCurrentProcess( ), 0 );
}

int _main( int argc, const char** argv )
{
	if ( argc != 2 ) return std::printf( "no module.\r\n" ), 'BILL';

	printf( "RAIN Injector V1 loaded" );
	
	std::string dll_path = argv[ 1 ];
	if ( _stricmp( dll_path.substr( dll_path.size( ) - 4, 4 ).c_str( ), ".dll" ) != 0 ) return std::printf( "Module is not a dll file.\r\n" ), 'BILL';

	std::ifstream file( argv[ 1 ], std::ios::binary );
	if ( !file ) return std::printf( "File not found.\r\n" ), 'SEXY';

	std::vector<std::uint8_t> buffer( std::istreambuf_iterator<char>( file ), {} );
	const auto buff_start = buffer.data( );

	const auto rbx_proc = utility::find_proc( L"RobloxPlayerBeta.exe" );
	if ( !rbx_proc || !rbx_proc->ProcessId ) return std::printf( "Roblox is not open, try again when it is.\r\n" ), 'NIGG';

	std::printf( "Roblox found at PID: %02lX\r\n", rbx_proc->ProcessId );
	std::thread{ shady_callback, rbx_proc->ProcessId }.detach( );

	const auto rbx_handle = OpenProcess( PROCESS_ALL_ACCESS, 0, rbx_proc->ProcessId );
	const auto image_dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( buff_start );
	const auto image_nt_header = reinterpret_cast< PIMAGE_NT_HEADERS >( buff_start + image_dos_header->e_lfanew );
	std::vector<IMAGE_SECTION_HEADER> section_headers( image_nt_header->FileHeader.NumberOfSections );
	const auto* headers_ = IMAGE_FIRST_SECTION( image_nt_header );
	std::generate_n( section_headers.begin( ), section_headers.size( ), [ &, i = 0 ]( ) mutable { return headers_[ i++ ]; } );

	// max size:
	const auto virtual_size = section_headers.back( ).VirtualAddress + section_headers.back( ).SizeOfRawData;
	auto local_dll_alloc = reinterpret_cast< std::uint8_t* >( VirtualAlloc( NULL, virtual_size, MEM_COMMIT, PAGE_READWRITE ) );

	// map in sections, fuck the nt headers. nobody needs that lithuanian garbage
	for ( const auto& header : section_headers )
	{
		memcpy( local_dll_alloc + header.VirtualAddress, buff_start + header.PointerToRawData, header.SizeOfRawData );
		if ( !std::memcmp( header.Name, ".rdata", 6 ) )
		{
			std::uintptr_t sec_cookie_value = 0x2B992DDFA232;
			for ( auto i = reinterpret_cast< std::uint8_t* >( local_dll_alloc + header.VirtualAddress ); i < local_dll_alloc + header.VirtualAddress + header.SizeOfRawData; i++ )
			{
				if ( !std::memcmp( i, &sec_cookie_value, 8 ) )
				{
					std::printf( "Overwrote security cookie %02llX", ( uintptr_t )i );
					*( uintptr_t* )i = 0xB00BB00BC0CC0CC; // are we being gay enough
				}
			}
		}
	}

	const auto idt_start = image_nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress;
	const auto idt_count = image_nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].Size / sizeof( import_directory_table_entry );

	for ( int i = 0; i < idt_count - 1; i++ )
	{
		const auto idt_entry = *reinterpret_cast< const import_directory_table_entry* >( local_dll_alloc + idt_start + ( i * sizeof( import_directory_table_entry ) ) ); // trust me it's not null
		auto module_name = reinterpret_cast< const char* >( local_dll_alloc + idt_entry.name_ );
		const auto rbx_import_base = utility::find_module_ext( rbx_proc->ProcessId, module_name );
		const auto local_import_handle = LoadLibraryExA( module_name, NULL, DONT_RESOLVE_DLL_REFERENCES );
		const auto local_import_base = ( uintptr_t )local_import_handle;

		if ( !rbx_import_base || !local_import_base ) return std::printf( "Module %s not present, dependency mapping not implemented.", module_name ), 'GAME';
		int k = 0;

		for ( auto j = reinterpret_cast< std::uint64_t* >( local_dll_alloc + idt_entry.import_lookup_table_rva ); *j != 0; j++ )
		{
			const auto import_location = rbx_import_base + ( ( uintptr_t )GetProcAddress( local_import_handle, ( const char* )( local_dll_alloc + 2 + *j ) ) - local_import_base );
			memcpy( local_dll_alloc + idt_entry.import_address_table_rva + ( k * 8 ), &import_location, 8 );
			k++;
		}
	}

#define RELOC_FLAG(rel_info) ((rel_info >> 0xC) == IMAGE_REL_BASED_DIR64)
	auto rbx_dll_alloc = reinterpret_cast< std::uint8_t* >( VirtualAllocEx( rbx_handle, NULL, virtual_size, MEM_COMMIT, PAGE_READWRITE ) );

	const auto export_dir_table = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >( local_dll_alloc + image_nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );
	auto func_addr_entry = reinterpret_cast< std::uint32_t* >( local_dll_alloc + export_dir_table->AddressOfFunctions );
	auto func_name_entry = reinterpret_cast< std::uint32_t* >( local_dll_alloc + export_dir_table->AddressOfNames );
	std::uintptr_t dllmain_offset = 0;

	for ( int i = 0; i < export_dir_table->NumberOfFunctions; i++, func_name_entry++, func_addr_entry++ )
	{
		const char* export_name = reinterpret_cast< const char* >( local_dll_alloc + *func_name_entry );
		if ( !_stricmp( "callback_function", export_name ) )
		{
			dllmain_offset = ( uintptr_t )rbx_dll_alloc + *func_addr_entry;
		}
	}

	if ( !dllmain_offset ) return std::printf( "Failed to find Entry Point\r\n" ), 'POOP';

	std::printf( "Allocated dll in roblox at %02llX\r\n", rbx_dll_alloc );
	const auto locdelta = ( uint64_t )rbx_dll_alloc - image_nt_header->OptionalHeader.ImageBase;
	if ( locdelta )
	{
		if ( image_nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].Size )
		{
			auto* reloc_data = reinterpret_cast< IMAGE_BASE_RELOCATION* >( local_dll_alloc + image_nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].VirtualAddress );
			while ( reloc_data->VirtualAddress )
			{
				std::uint32_t entry_count = ( reloc_data->SizeOfBlock - 8 ) / 2;
				auto rel_info = reinterpret_cast< std::uint16_t* >( reloc_data + 1 );
				for ( uint32_t i = 0; i != entry_count; i++, rel_info++ )
				{
					if ( RELOC_FLAG( *rel_info ) )
					{
						*( uintptr_t* )( local_dll_alloc + reloc_data->VirtualAddress + ( ( *rel_info ) & 0xFFF ) ) += ( uintptr_t )locdelta;
					}
				}
				reloc_data = reinterpret_cast< IMAGE_BASE_RELOCATION* >( reinterpret_cast< uint8_t* >( reloc_data ) + reloc_data->SizeOfBlock );
			}
		}
	}

	WriteProcessMemory( rbx_handle, rbx_dll_alloc, local_dll_alloc, virtual_size, NULL );
	DWORD shit = 0;
	VirtualProtectEx( rbx_handle, rbx_dll_alloc, virtual_size, PAGE_READWRITE, &shit );
	for ( const auto& header : section_headers )
	{
		DWORD shit = 0;
		VirtualProtectEx( rbx_handle, rbx_dll_alloc + header.VirtualAddress, header.SizeOfRawData, pasted_characteristic_conv( header.Characteristics ), &shit );
	}

	const auto [rbx_dll_base, rbx_dll_size] = utility::get_module_info( rbx_handle, "RobloxPlayerBeta.dll" );
	if ( !rbx_dll_base || !rbx_dll_size ) return std::printf( "Hyperion cannot be found, maybe close AWP v2.\r\n" ), 'BOOB';

	std::printf( "Found hyperion base: %02llX, %02llX\r\n", rbx_dll_base, rbx_dll_size );
	std::vector<std::uintptr_t> thread_list;

	for ( std::uint32_t i = 0; i < rbx_proc->ThreadCount; i++ )
	{
		HANDLE thread = OpenThread( THREAD_ALL_ACCESS, 0, rbx_proc->ThreadInfos[ i ].Client_Id.UniqueThread );
		std::uint64_t start_address = 0;
		if ( !ntapi::NtQueryInformationThread( thread, ThreadQuerySetWin32StartAddress, &start_address, 8, NULL ) && start_address >= ( uintptr_t )rbx_dll_base && start_address < ( uintptr_t )rbx_dll_base + rbx_dll_size )
		{
			thread_list.push_back( rbx_proc->ThreadInfos[ i ].Client_Id.UniqueThread );
			SuspendThread( thread );
		}
		CloseHandle( thread );
	}

	// Setup local syscalls, just bc i dont want to spam wpm when it's not necessary
	// WPM is also incredibly slow so it's best to just do it at once.
	auto local_syscall_space = reinterpret_cast< std::uint8_t* >( VirtualAlloc( NULL, 4096, MEM_COMMIT, PAGE_READWRITE ) );
	std::printf( "Allocated page in local process, mapping in code hooks.\r\n" );
	// first is NtQueryVM
	memcpy( local_syscall_space, GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "NtQueryVirtualMemory" ), 8 );
	// 8
	// SYSCALL C3
	memcpy( local_syscall_space + 8, "\x0F\x05\xC3\xCC\xCC\xCC\xCC\xCC", 8 );
	// 16
	// Second is NtProtectVM
	memcpy( local_syscall_space + 16, GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "NtProtectVirtualMemory" ), 8 );
	// 24
	// SYSCALL C3
	memcpy( local_syscall_space + 24, "\x0F\x05\xC3\xCC\xCC\xCC\xCC\xCC", 8 );
	// 32
	// now we can copy over our stub
	memcpy( local_syscall_space + 32, &ic_hook_callback, ic_hook_callback_size );

	std::uint64_t new_idx = 32 + ic_hook_callback_size + 16 - ( ic_hook_callback_size % 16 );
	std::uintptr_t ic_func_sz = 0;

	for ( auto i = ( uint8_t* )&ic_hook::instrumentation_callback; i < ( uint8_t* )&ic_hook::instrumentation_callback + 0x500; i++ )
		if ( !std::memcmp( i, "\xCC\xCC", 2 ) ) ic_func_sz = ( uintptr_t )i - ( uintptr_t )&ic_hook::instrumentation_callback + 2;

	if ( !ic_func_sz ) return std::printf( "Function size of IC could not be determined.\r\n" ), 'AHHH';
	memcpy( local_syscall_space + new_idx, &ic_hook::instrumentation_callback, ic_func_sz );

	std::uintptr_t orig_ic_offset{ 0 };
	// Find original IC
	for ( std::uintptr_t i = rbx_dll_base; i < rbx_dll_base + rbx_dll_size;)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if ( !VirtualQueryEx( rbx_handle, ( void* )i, &mbi, sizeof( mbi ) ) )
		{
			i += 16;
			continue;
		}
		if ( mbi.AllocationProtect != PAGE_EXECUTE_READ )
		{
			i += mbi.RegionSize;
			continue;
		}
		auto page_data = reinterpret_cast< std::uint8_t* >( VirtualAlloc( NULL, mbi.RegionSize, MEM_COMMIT, PAGE_READWRITE ) );
		ReadProcessMemory( rbx_handle, mbi.BaseAddress, page_data, mbi.RegionSize, NULL );
		for ( auto j = page_data; j < page_data + mbi.RegionSize - 8; j++ )
		{
			if ( !std::memcmp( j, "\x41\x52\x50\x9C\x53\x48\x89\xE3", 8 ) )
			{
				orig_ic_offset = i;
			}
		}
		i += mbi.RegionSize;

	}

	if ( !orig_ic_offset )
		return std::printf( "IC could not be found.\r\n" ), 'BLDM';

	std::printf( "Located IC\r\n" );

	/*
		std::uintptr_t KiUserExceptionDispatcher = 0xACABACABACABACAB;
	auto OurRtlRestoreContext = reinterpret_cast<OurRtlRestoreContext_ty>(0x8888888888888888);  // ud right?
		std::uintptr_t KiUserCallbackDispatcher = 0x6EE96EE96EE96EE9;
	std::uintptr_t KiUserApcDispatcher = 0x1337133713371337;
	*/
	// QVM & PVM off from shellcode
	std::uintptr_t qvm_off{ 0 }, pvm_off{ 0 }, orig_ic_off{ 0 }, new_ic_handler_off{ 0 }, ued_off{ 0 }, rsc_off{ 0 }, ucd_off{ 0 }, uad_off{ 0 };
	for ( auto addr = local_syscall_space; addr < local_syscall_space + 0x400; addr++ )
	{
		if ( !std::memcmp( addr, "\xDE\xC0\xAD\xDE\xDE\xC0\xAD\xDE", 8 ) )
			qvm_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\xCC\xC0\xAD\xDE\xCC\xC0\xAD\xDE", 8 ) )
			pvm_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\x96\x96\x96\x96\x96\x96\x96\x96", 8 ) )
			orig_ic_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\x69\x69\x69\x69\x69\x69\x69\x69", 8 ) )
			new_ic_handler_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\xE9\x6E\xE9\x6E\xE9\x6E\xE9\x6E", 8 ) )
			ucd_off = ( uintptr_t )( addr - local_syscall_space );

		if ( !std::memcmp( addr, "\x37\x13\x37\x13\x37\x13\x37\x13", 8 ) )
			uad_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\xAB\xAC\xAB\xAC\xAB\xAC\xAB\xAC", 8 ) )
			ued_off = ( uintptr_t )( addr - local_syscall_space );
		if ( !std::memcmp( addr, "\x88\x88\x88\x88\x88\x88\x88\x88", 8 ) )
			rsc_off = ( uintptr_t )( addr - local_syscall_space );

	}

	// allocate space for syscalls, apc queue
	// this is a small precaution, maybe readwrite mem is gonna not be flagged? who knows
	// when we need it to execute, switch it to RX.
	auto syscall_space = reinterpret_cast< uint8_t* >( VirtualAllocEx( rbx_handle, NULL, 4096, MEM_COMMIT, PAGE_READWRITE ) );
	std::uintptr_t new_qvm = ( uintptr_t )syscall_space;
	std::uintptr_t new_pvm = new_qvm + qvm_off;
	std::uintptr_t new_ic_handler = new_qvm + new_idx;

	std::uintptr_t rbx_ntdll_base = utility::find_module_ext( rbx_proc->ProcessId, "ntdll.dll" );
	const auto local_ntdll_handle = GetModuleHandleA( "ntdll.dll" );
	const auto local_ntdll_base = ( uintptr_t )local_ntdll_handle;

	const auto new_rsc = rbx_ntdll_base + ( ( uintptr_t )GetProcAddress( local_ntdll_handle, "RtlRestoreContext" ) - local_ntdll_base );
	const auto new_ucd = rbx_ntdll_base + ( ( uintptr_t )GetProcAddress( local_ntdll_handle, "KiUserCallbackDispatcher" ) - local_ntdll_base );
	const auto new_uad = rbx_ntdll_base + ( ( uintptr_t )GetProcAddress( local_ntdll_handle, "KiUserApcDispatcher" ) - local_ntdll_base );
	const auto new_ued = rbx_ntdll_base + ( ( uintptr_t )GetProcAddress( local_ntdll_handle, "KiUserExceptionDispatcher" ) - local_ntdll_base );

	memcpy( local_syscall_space + qvm_off, &new_qvm, 8 );
	memcpy( local_syscall_space + pvm_off, &new_pvm, 8 );
	memcpy( local_syscall_space + orig_ic_off, &orig_ic_offset, 8 );
	memcpy( local_syscall_space + new_ic_handler_off, &new_ic_handler, 8 );

	memcpy( local_syscall_space + rsc_off, &new_rsc, 8 );
	memcpy( local_syscall_space + ucd_off, &new_ucd, 8 );
	memcpy( local_syscall_space + uad_off, &new_uad, 8 );
	memcpy( local_syscall_space + ued_off, &new_ued, 8 );

	WriteProcessMemory( rbx_handle, syscall_space, local_syscall_space, 4096, NULL );
	std::printf( "Allocated stub in roblox\r\n", ( uintptr_t )syscall_space );
	DWORD shit2 = 0;

	VirtualProtectEx( rbx_handle, syscall_space, 4096, PAGE_EXECUTE_READ, &shit2 );
	utility::CreateAPCDirectThread( rbx_handle, ( void* )dllmain_offset );

	std::printf( "Injected! Make sure to keep this process open, or else you will get detected.\r\n" );
	for ( const auto tid : thread_list )
	{
		HANDLE thread = OpenThread( THREAD_ALL_ACCESS, 0, tid );
		ResumeThread( thread );
		CloseHandle( thread );
	}

	HWND hwnd = GetConsoleWindow( );
	HMENU hmenu = GetSystemMenu( hwnd, FALSE );

	EnableMenuItem( hmenu, SC_CLOSE, MF_GRAYED );
	PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION inscinfowb = {};
	memset( &inscinfowb, 0, sizeof( PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION ) );

	inscinfowb.Callback = syscall_space + 32;
	auto NtSetInformationProcess = reinterpret_cast< NtSetInformationProcess_t >( GetProcAddress( local_ntdll_handle, "NtSetInformationProcess" ) );
	while ( 1 )
	{
		NtSetInformationProcess( rbx_handle, ProcessInstrumentationCallback, &inscinfowb, sizeof( PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION ) );
	}

}

int main( int argc, const char** argv )
{
	int retval = _main( argc, argv );
	std::cin.get( );
	return retval;
}
