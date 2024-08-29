// Input Output Stuff
#include <iostream>

// Windows API Stuff
#include <Windows.h>

// Directx9 Interface Stuff
#include "dx.h"

// Hooking Functions
#include "hook.h"

// Hacking Helper Stuff
#include "hack.h"

// Drawing Functions
#include "drawing.h"

// data we need
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

// Get the different Modules you need
//uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"");
//uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"");
//uintptr_t serverBase = (uintptr_t)GetModuleHandle(L"");

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
	if (!pDevice)
		pDevice = o_pDevice;

	// drawing hook message 
	DrawText("Succesfully Hooked!", windowWidth / 2, windowHeight - 50, D3DCOLOR_ARGB(255, 255, 255, 255));

	// call the original function
	oEndScene(pDevice);
}

// Our HackThread where it initializes Everything and starts the Hook
DWORD WINAPI HackThread(HMODULE hModule)
{
	// Allocate a Console (for debugging)
	//AllocConsole();
	//FILE* f;
	//freopen_s(&f, "CONOUT$", "w", stdout);

	//std::cout << "Hello World!" << std::endl;

	// This starts the Hook and gets the Device from the vTable
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	// Waiting for Uninject, and gets the viewMatrix
	while (!GetAsyncKeyState(VK_INSERT))
	{
		// change the module if the viewMatrix reads it differently
		//memcpy(&viewMatrix, (BYTE*)(moduleBase + mViewMatrix), sizeof(viewMatrix));
		Sleep(1);
	}

	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	Sleep(1000);

	// Cleanup
	//fclose(f);
	//FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

// Entry DllMain
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	}
	return TRUE;
}