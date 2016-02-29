#include <windows.h>
#include "SimpleDetour.h"
#include "Metamorphe.h"

DWORD			*morph_list;
unsigned int	morph_count = 0;

#define GET_EIP _asm	{					\
						_asm push eax		\
						_asm mov eax, [esp + 4]	\
						_asm push ecx		\
						_asm push edx		\
						_asm mov edx, [morph_list] \
						_asm mov ecx, 0		\
						_asm my_loop:		\
						_asm cmp eax, [edx + ecx * 4]	\
						_asm je my_leave	\
						_asm cmp [edx + ecx * 4], 0	\
						_asm je my_change	\
						_asm inc ecx		\
						_asm jmp my_loop	\
						_asm my_change:		\
						_asm mov [edx + ecx * 4], eax	\
						_asm my_leave:		\
						_asm pop edx		\
						_asm pop ecx		\
						_asm pop eax		\
						}

_declspec(naked) void _intern_protec()
{
	GET_EIP;
	_asm
	{
		push eax
		mov eax, [esp + 4]
		push ecx
		push edx
		mov edx, [morph_list]
		//add esp, 4
		ret
	}
}

_declspec(naked) void _my_hook()
{
	
	GET_EIP;
	_asm
	{
		mov eax, [ebp - 8]
		add eax, [ebp - 4]
		mov [ebp - 4], eax
		//Original instruction
		ret
	}
}

void		MainThread()
{
	Sleep(1500);
	morph_list = (DWORD*)malloc(sizeof(DWORD) * 10);
	morph_list[0] = 0;
	morph_list[1] = 0;
	spawn_metamorphe(0x00F81027, (DWORD)&_my_hook, 9);
	//spawn_metamorphe((DWORD)&_my_hook, (DWORD)&_intern_protec, 13);
	init_metamorphe();
}

BOOLEAN WINAPI		DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
	DisableThreadLibraryCalls(hDllHandle);
	if (nReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&MainThread, 0, 0, 0);
	return TRUE;
}