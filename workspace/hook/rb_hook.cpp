#include "../includes.h"

// shade ud (ultra detected) IC method

void ic_hook::instrumentation_callback( ic_ctx* ctx ) 
{
	std::uintptr_t KiUserCallbackDispatcher = 0x6EE96EE96EE96EE9;
	std::uintptr_t KiUserApcDispatcher = 0x1337133713371337;

	if ( ctx->r10 == ( DWORD64 )KiUserCallbackDispatcher )
	{
		ctx->rcx = *( DWORD64* )( ctx->rsp + 0x20 );
		ctx->returnaddr = ctx->r10 + 5;
	}
	if ( ctx->r10 == ( DWORD64 )KiUserApcDispatcher )
	{
		ctx->rcx = *( DWORD64* )( ctx->rsp + 0x18 );
		ctx->returnaddr = ctx->r10 + 5;
	}

	//hooking exceptions
	std::uintptr_t KiUserExceptionDispatcher = 0xACABACABACABACAB;
	auto OurRtlRestoreContext = reinterpret_cast< OurRtlRestoreContext_ty >( 0x8888888888888888 );

	if ( ctx->r10 == ( DWORD64 )KiUserExceptionDispatcher )
	{
		PEXCEPTION_RECORD ExceptionRecord = ( PEXCEPTION_RECORD )( ctx->rsp + 0x4f0 );
		PCONTEXT Context = ( PCONTEXT )ctx->rsp;

		if ( ( ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE ) )
		{
			MEMORY_BASIC_INFORMATION binfo = {};
			auto NtQueryVirtualMemoryInline = reinterpret_cast< NtQueryVirtualMemory_t >( 0xDEADC0DEDEADC0DE );
			auto NtProtectVirtualMemoryInline = reinterpret_cast< NtProtectVirtualMemory_t >( 0xDEADC0CCDEADC0CC );
			NtQueryVirtualMemoryInline( ( HANDLE )0xffffffffffffffff, ( void* )Context->Rip, MemoryBasicInformation, &binfo, sizeof( MEMORY_BASIC_INFORMATION ), NULL );
			NtProtectVirtualMemoryInline( ( HANDLE )0xffffffffffffffff, &binfo.BaseAddress, &binfo.RegionSize, PAGE_EXECUTE_READ, NULL );
			OurRtlRestoreContext( Context, ExceptionRecord );
		}
	}

	// original IC address
	ctx->returnaddr = ( DWORD64 )0x9696969696969696;
	return;
}