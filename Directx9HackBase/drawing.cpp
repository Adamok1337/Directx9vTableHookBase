#include "drawing.h"

// Ínclude the pointers the direct Font and Line Interface
// Our Pointers to the Line and Font Interface of Directx9
ID3DXLine* LineL;
ID3DXFont* FontF;

void DrawText(const char* text, float x, float y, D3DCOLOR color)
{
	RECT rect;
	if (!FontF)
	{
		D3DXCreateFont(pDevice, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &FontF);
	}

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}