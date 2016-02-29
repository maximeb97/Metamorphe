#pragma once
#include <Windows.h>

void WriteASM(DWORD dwAddy, DWORD dwASM, int len);
void *DetourFunction(BYTE *src, const BYTE *dst, const int len);
void *DetourCallFunction(BYTE *src, const BYTE *dst, const int len);