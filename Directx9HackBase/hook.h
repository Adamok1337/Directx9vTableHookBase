#pragma once
#include <Windows.h>

// This Function is for Restoring the old Bytes, so we can Unhook
void Patch(BYTE* dst, BYTE* src, unsigned int size);

// This Function is for Nopping Bytes so we can skip instructions
void Nop(BYTE* dst, unsigned int size);

// This Function Hooks the EndScene Function and will replace it with our Function
bool Hook(char* src, char* dst, int len);

// This Function creates the Gateway Function where will jump, after our Function is finished
char* TrampHook(char* src, char* dst, unsigned int len);