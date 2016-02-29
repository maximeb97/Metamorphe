#include <windows.h>

void WriteASM(DWORD dwAddy, DWORD dwASM, int len)
{
	unsigned long Protection;

	VirtualProtect((void*)dwAddy, len, PAGE_EXECUTE_READWRITE, &Protection);
	memcpy((void*)dwAddy, (const void*)dwASM, len);
	VirtualProtect((void*)dwAddy, len, Protection, NULL);
}

void *DetourFunction(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp;
	DWORD dwBack;

	jmp = (BYTE*)malloc(len + 5);
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (int i = 5; i<len; i++)  src[i] = 0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp - len);
}

void *DetourCallFunction(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp;
	DWORD dwBack;

	jmp = (BYTE*)malloc(len + 5);
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE8;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE8;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (int i = 5; i<len; i++)  src[i] = 0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp - len);
}