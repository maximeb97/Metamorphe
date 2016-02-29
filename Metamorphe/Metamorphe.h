#pragma once
#include <Windows.h>
#include "SimpleDetour.h"

extern DWORD		*morph_list;
extern unsigned int	morph_count;

void	spawn_metamorphe(DWORD address, DWORD hook, int size);
void	init_metamorphe();