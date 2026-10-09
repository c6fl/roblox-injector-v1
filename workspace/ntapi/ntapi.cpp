#include "../includes.h"

NTSTATUS ntapi::NtQuerySystemInformation( SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength )
{
	static auto ntdll_base = GetModuleHandleA( "ntdll.dll" );
	if ( !ntdll_base ) return 0xDEADDEAD;

	static auto query_sysinfo = reinterpret_cast< decltype( NtQuerySystemInformation )* >( GetProcAddress( ntdll_base, "NtQuerySystemInformation" ) );
	if ( !query_sysinfo )  return 0xBADF00D;

	return query_sysinfo( SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength );
}

NTSTATUS ntapi::NtQueryInformationThread( HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength )
{
	static auto ntdll_base = GetModuleHandleA( "ntdll.dll" );
	if ( !ntdll_base ) return 0xDEADB00B;

	static auto query_threadinfo = reinterpret_cast< decltype( NtQueryInformationThread )* >( GetProcAddress( ntdll_base, "NtQueryInformationThread" ) );
	if ( !query_threadinfo )  return 0xF00DC0CC;

	return query_threadinfo( ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength );
}