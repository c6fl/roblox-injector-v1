#include "../includes.h"

SYSTEM_PROCESS_INFORMATION* utility::find_proc( const std::wstring_view proc_name )
{
	unsigned long process_alloc_sz = 0;
	ntapi::NtQuerySystemInformation( SystemProcessInformation, NULL, 0, &process_alloc_sz );

	auto procs = reinterpret_cast< SYSTEM_PROCESS_INFORMATION* >( malloc( process_alloc_sz ) );
	auto curr_proc = procs;

	if ( ntapi::NtQuerySystemInformation( SystemProcessInformation, procs, process_alloc_sz, NULL ) )
	{
		goto free_shit;
	}

	while ( curr_proc )
	{
		if ( curr_proc->ImageName.Buffer != 0 && !_wcsicmp( curr_proc->ImageName.Buffer, proc_name.data( ) ) )
		{
			auto found_proc = static_cast< SYSTEM_PROCESS_INFORMATION* >( malloc( sizeof( SYSTEM_PROCESS_INFORMATION ) ) );
			if ( found_proc )
			{
				*found_proc = *curr_proc;
			}

			free( procs );
			return found_proc;
		}

		if ( !curr_proc->NextOffset ) goto free_shit;
		curr_proc = reinterpret_cast< SYSTEM_PROCESS_INFORMATION* >( reinterpret_cast< std::uint8_t* >( curr_proc ) + curr_proc->NextOffset );
	}

free_shit:
	free( procs );
	return 0;
}

std::pair<std::uintptr_t, std::uintptr_t> utility::get_module_info( HANDLE proc, const std::string_view mod_name )
{
	HMODULE modules[ 1024 ]; // yeah seems good, 1024 of these please.
	DWORD space_needed = 0;

	if ( K32EnumProcessModulesEx( proc, modules, sizeof( modules ), &space_needed, LIST_MODULES_ALL ) )
	{
		for ( std::size_t i = 0; i < ( space_needed / sizeof( HMODULE ) ); i++ )
		{
			CHAR mod_name1[ 260 ];
			if ( K32GetModuleBaseNameA( proc, modules[ i ], mod_name1, 260 ) )
			{
				if ( !_stricmp( mod_name1, mod_name.data( ) ) )
				{
					MODULEINFO mod_info;
					if ( K32GetModuleInformation( proc, modules[ i ], &mod_info, sizeof( mod_info ) ) )
					{
						return { reinterpret_cast< std::uintptr_t >( mod_info.lpBaseOfDll ), mod_info.SizeOfImage };
					}
				}
			}
		}
	}

	return { 0,0 };
}

uintptr_t utility::find_module_ext( DWORD processID, const char* mod_name )
{
	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID );
	if ( snapshot == INVALID_HANDLE_VALUE ) return 0;

	MODULEENTRY32 mod_entry;
	mod_entry.dwSize = sizeof( MODULEENTRY32 );

	if ( Module32First( snapshot, &mod_entry ) )
	{
		do
		{
			char unfucked_string[ MAX_PATH ];
			WideCharToMultiByte( CP_ACP, 0, mod_entry.szModule, -1, unfucked_string, MAX_PATH, nullptr, nullptr );

			if ( _stricmp( unfucked_string, mod_name ) == 0 )
			{
				CloseHandle( snapshot );
				return ( uintptr_t )mod_entry.modBaseAddr;
			}
		} 
		
		while ( Module32Next( snapshot, &mod_entry ) );
	}

	CloseHandle( snapshot );
	return 0;
}

bool utility::CreateAPCDirectThread( HANDLE process, void* addr )
{
	using NtQueryObject_ty = NTSTATUS( * )( HANDLE Handle, std::uint32_t ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength );
	using ZwSetIoCompletion_ty = NTSTATUS( * )( HANDLE a1, void* a2, void* a3, NTSTATUS a4, uintptr_t a5 );
	auto NtQueryObject = reinterpret_cast< NtQueryObject_ty >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "NtQueryObject" ) );
	auto ZwSetIoCompletion = reinterpret_cast< ZwSetIoCompletion_ty >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "ZwSetIoCompletion" ) );

	PROCESS_HANDLE_SNAPSHOT_INFORMATION* handles = ( PROCESS_HANDLE_SNAPSHOT_INFORMATION* )( new BYTE[ 100000 ] );
	goto sk;
err:
	delete handles;
	return 0;
sk:
	NTSTATUS stat = ntapi::NtQuerySystemInformation( ( SYSTEM_INFORMATION_CLASS )ProcessHandleInformation, handles, 100000, NULL );
	if ( stat != 0 ) { goto err; }

	DWORD handlen = 0;
	OBJECT_TYPE_INFORMATION* hndtype = ( OBJECT_TYPE_INFORMATION* )( new BYTE[ 10000 ] );
	HANDLE iocomp = NULL;

	while ( handlen < handles->NumberOfHandles )
	{
		if ( DuplicateHandle( process, ( HANDLE )handlen, GetCurrentProcess( ), &iocomp, 0, 0, DUPLICATE_SAME_ACCESS ) != 0 )
		{
			NtQueryObject( iocomp, 2, hndtype, 10000, NULL );
			//std::wcerr << hndtype->TypeName.Buffer << "\n";
			if ( wcscmp( L"IoCompletion", hndtype->TypeName.Buffer ) == 0 )
			{
				goto iocompfound;
			}
			CloseHandle( iocomp );
		}
		handlen += 1;
	}
	goto err;
iocompfound:
	APC_DIRECT Direct{ 0 };
	Direct.Callback = ( APC_DIRECT* )( addr );
	PAPC_DIRECT RemoteDirectAddress = ( PAPC_DIRECT )( VirtualAllocEx( process, NULL, sizeof( APC_DIRECT ), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE ) );
	
	if ( RemoteDirectAddress == NULL ) { goto err; }
	if ( WriteProcessMemory( process, RemoteDirectAddress, &Direct, sizeof( APC_DIRECT ), NULL ) == 0 ) { goto err; };
	if ( ( DWORD )ZwSetIoCompletion( iocomp, RemoteDirectAddress, 0, 0, 0 ) != 0 ) { goto err; };

	delete handles;
	return 1;
}