#include "hook.h"

// This Function is for Restoring the old Bytes, so we can Unhook
void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	// We are Creating a Variable to store the old Protectiona and the overwrite the Read/Write Permission at the given Destination and copy over the bytes over to our src Variable and restore the Protection back
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

void Nop(BYTE* dst, unsigned int size)
{
	// We are Creating a Variable to store the old Protectiona and the overwrite the Read/Write Permission at the given Destination and copy over the bytes over to our src Variable and restore the Protection back
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

// This Function Hooks the EndScene Function and will replace it with our Function
bool Hook(char* src, char* dst, int len)
{
	// Check if len is higher than 5, otherwise return, because a jump instruction in x86 has to be atleast 5 bytes
	if (len < 5)
		return false;

	// Create a Variable to hold our Old Procetions and then overwrite the Protection at the given Destination with Read/Write
	DWORD oldProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	// replace all the bytes with 0x90 (NOP), for safety purposes
	memset(src, 0x90, len);

	// get our relativeAddr, so the destination - source and 5 because of the jump instruction
	uintptr_t relativeAddr = (uintptr_t)(dst - src - 5);

	// place in our relative Address 
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddr;

	// restore the old Protections
	VirtualProtect(src, len, oldProtect, &oldProtect);

	// return true indicating that the Hook was Sucessfully done
	return true;
}

// This Function creates the Gateway Function where will jump, after our Function is finished
char* TrampHook(char* src, char* dst, unsigned int len)
{
	// Check if len is higher than 5, otherwise return, because a jump instruction in x86 has to be atleast 5 bytes
	if (len < 5)
		return 0;

	// Create a new Memory Cave where we hold a pointer to it with (gateway), we will then copy the bytes over to the gateway
	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	
	// get our jumpAddy, so we know how much we have to jump back, 0xE9 is the jump instruction in x86 Assembly
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;

	// then we call our hook function 
	if (Hook(src, dst, len))
	{
		return gateway;
	}
	else
		return nullptr;
}