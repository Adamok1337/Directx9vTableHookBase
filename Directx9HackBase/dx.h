#pragma once

// Include the d3d9 library
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Hooking the Function 'EndScene' Prototype
typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

// Getting the Windows Process of the Target Game
HWND GetProcessWindow();

// Our Functiont that Creates the Dummy Device Environment
bool GetD3D9Device(void** pTable, size_t size);

// Handle to Window
static HWND window;

// Resolution of the Game -> 800x600 (for example)
extern int windowHeight, windowWidth;

// Our Directx9 Device
extern LPDIRECT3DDEVICE9 pDevice;