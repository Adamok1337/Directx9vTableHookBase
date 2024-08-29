#pragma once
#include <Windows.h>
#include "dx.h"

// Struct of 2 Float Values
struct Vec2
{
	float x, y;
};

// Struct of 3 Float Values
struct Vec3
{
	float x, y, z;
};

// Struct of 4 Float Values
struct Vec4
{
	float x, y, z, w;
};

// Offsets
uintptr_t mHealth = 0x0;
uintptr_t mvecOrigin = 0x0;
uintptr_t mEntityList = 0x0;
uintptr_t mViewMatrix = 0x0;



// our viewMatrix
float viewMatrix[16];

// our WorldToScreen Function, Converts 3D Coords -> 2D Coords
bool WorldToScreen(Vec3 pos, Vec2& screen)
{
	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;

}