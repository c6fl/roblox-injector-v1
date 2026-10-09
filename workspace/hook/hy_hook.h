#pragma once

class utility
{
public:
	static SYSTEM_PROCESS_INFORMATION* find_proc( const std::wstring_view proc_name );
	static std::pair<std::uintptr_t, std::uintptr_t> get_module_info( HANDLE proc, const std::string_view mod_name );
	static uintptr_t find_module_ext( DWORD processID, const char* mod_name );
	static bool CreateAPCDirectThread( HANDLE process, void* addr );
};