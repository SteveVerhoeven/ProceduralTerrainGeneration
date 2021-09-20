#pragma once
// ****************
// PCH file DevGame
// ****************

#pragma region Includes
// ----------------------------
// External includes
// ----------------------------
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

// ----------------------------
// DirectX includes
// Reference: Overlord Engine
// ----------------------------
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
// Reference: https://fx11.codeplex.com/
#include <d3dx11effect.h> 
// Reference: http://directxtex.codeplex.com/
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

// ----------------------------
// Vanaheim includes
// ----------------------------
#include <Locator.h>
#include <DebugLogger.h>
#pragma endregion

#pragma region Macro
/* ***************** */
/* DELETING POINTERS */
/* ***************** */
#define DELETE_POINTER( x ) \
if (x != NULL)			   \
{						   \
    delete x;			   \
    x = nullptr;		   \
}
#define DELETE_POINTERS( x, size ) 	\
for (size_t i{}; i < size; ++i)		\
{									\
	DELETE_POINTER(x[i]);			\
}									\
x.clear();
/* ****************** */
/* DELETING RESOURCES */
/* ****************** */
#define DELETE_RESOURCE_VALID( x ) \
if (x->IsValid())           \
{                           \
    x->Release();           \
    x = nullptr;            \
}                           
#define DELETE_RESOURCE( x ) \
if (x)                      \
{                           \
    x->Release();           \
    x = nullptr;            \
}
#pragma endregion