#pragma once

extern "C" void ic_hook_callback( );
extern "C" std::uintptr_t ic_hook_callback_size;

typedef struct ic_ctx
{

	M128A xmm7;
	M128A xmm6;
	M128A xmm5;
	M128A xmm4;
	M128A xmm3;
	M128A xmm2;
	M128A xmm1;
	M128A xmm0;

	DWORD64 r15;
	DWORD64 r14;
	DWORD64 r13;
	DWORD64 r12;
	DWORD64 r11;
	DWORD64 r10;
	DWORD64 r9;
	DWORD64 r8;

	DWORD64 rdi;
	DWORD64 rsi;
	DWORD64 rbp;
	DWORD64 rbx;
	DWORD64 rdx;
	DWORD64 rcx;
	DWORD64 rax;

	DWORD64 returnaddr;
	DWORD64 rsp;
};

class ic_hook
{
public:
	static void instrumentation_callback( ic_ctx* ctx );
};