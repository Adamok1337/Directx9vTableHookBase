#include "dx.h"

int windowHeight, windowWidth;

// Goes through each Window and finds the correct one and stores the Handle inside our HWND Window
BOOL CALLBACK enumWind(HWND Handle, LPARAM lp)
{
	// Initialize our Process ID Variable, and then store the Process ID and make a Check if its the right one
	DWORD procId;
	GetWindowThreadProcessId(Handle, &procId);
	if (GetCurrentProcessId() != procId)
	{
		return true;
	}

	// if its correct, store the Handle inside window
	window = Handle;
	return false;
}

// Gets the Process Window and stores the in Game Resolution Values
HWND GetProcessWindow()
{
	// Defined window as NULL
	window = NULL;

	// Call the function that returns the corresponding Handle to our Target Window Game
	EnumWindows(enumWind, NULL);

	// Creating a Struct to store the top, bottom, right and left values
	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	// This is padding for the game overlay topbar, remove if you in fullscreen
	windowWidth -= 29;
	windowWidth -= 5;

	// returning the window Handle
	return window;
}

// Initializes Directx9 and sets up the Dummy Device Environment and then steals the EndScene Function Address of the vTable
bool GetD3D9Device(void** pTable, size_t size)
{
	// Check if pTable is not true
	if (!pTable)
		return false;

	// Initializes Directx9
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	// Creating the Dummy Device Pointer
	IDirect3DDevice9* pDummyDevice = nullptr;

	// Creating the Directx9 Class and giving it Default Properties
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	// This creates the final Dummy Device with its defined Settings
	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	// If the Game is not in fullscreen, lets search for windowed mode
	if (dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	// This copies the entire vTable and stores it in our Array, and then we release the Directx9 Interface and Device
	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}