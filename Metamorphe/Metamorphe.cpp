#include <windows.h>
#include <math.h>
#include "SimpleDetour.h"
#include "Metamorphe.h"

/*

	OPCODE						SIZE

origin:
	call	hook				5
 __	jmp		+2					2
 |	nop		*					1
 |	nop		*					1
 ->	...							9

hook:
	push	eax					1
	mov		eax, [esp]			3
	mov		[eip], eax			5
	pop		eax					1
	push	eip					5
	call	func_change_bytes	5
	* Original 9 bytes *

*/


/*
**	Spawn metamorphe on 9 bytes
*/
void	spawn_metamorphe(DWORD address, DWORD hook, int size)
{
	int	i = 9;

	DetourCallFunction((BYTE*)address, (BYTE*)hook, 5);
	WriteASM(address + 0x5, (DWORD)"\xEB\x02\x90\x90", 4);
	while (i != size)
		WriteASM(address + i++, (DWORD)"\x90", 1);
	morph_count++;
}

void		morphing(DWORD eip, unsigned int id)
{
	BYTE	obf[2];

	obf[0] = rand() % 255;
	obf[1] = rand() % 255;
	WriteASM(eip + 0x2, (DWORD)obf, 2);
}

void		metamorphe()
{
	int		i;

	while (1)
	{
		i = 0;
		while (morph_list[i] != 0)
		{
			morphing(morph_list[i], i);
			i++;
		}
		Sleep(100);
	}
}

void	init_metamorphe()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)metamorphe, 0, 0, 0);
}