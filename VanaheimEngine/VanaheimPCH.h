#pragma once
// ************************
// PCH file Vanaheim Engine
// ************************

#pragma region Macro
/* ***************** */
/* DELETING POINTERS */
/* ***************** */
#define DELETE_POINTER( x ) \
if (x != NULL)			    \
{						    \
    delete x;			    \
    x = nullptr;			    \
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
if (x->IsValid())				   \
{								   \
    DELETE_RESOURCE( x );		   \
} 
#define DELETE_RESOURCE( x ) \
if (x)                       \
{                            \
    x->Release();            \
    x = nullptr;                \
}
/* ******* */
/* LOGGING */
/* ******* */
/** Function to log a message to the visual studio console (IF created) and to the engine console */
// @param: errorLevel - The level of message that is sent to the consoles (ErrorLevel::...)
// @param: message    - The message to sent to the consoles
#define LOG( errorLevel, message )	\
Locator::GetDebugLoggerService()->Log(errorLevel, message);
#define LOG_HRESULT( hr, function, file, line )	\
Locator::GetDebugLoggerService()->LogHRESULT(hr, function, file, line);
#pragma endregion

#pragma region Includes
// ----------------------------
// External includes
// ----------------------------
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
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
#include "Locator.h"

#include "DebugLogger.h"

#include "ConsoleUI.h"
#include "InspectorUI.h"
#include "HierarchyUI.h"
#include "ViewportUI.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "GeneratorManager.h"
#include "NoiseGenerator.h"
#include "ImageGenerator.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "LineComponent.h"
#include "NameComponent.h"
#include "TerrainGeneratorComponent.h"
#pragma endregion

#pragma region General structs
struct Vertex_Input
{
	DirectX::XMFLOAT3 Position{};
	DirectX::XMFLOAT3 Color{};
	DirectX::XMFLOAT2 UV{};
	DirectX::XMFLOAT3 InstancePos{};
	DirectX::XMFLOAT4 InstanceLengthV1{};
	DirectX::XMFLOAT4 InstanceLengthV2{};

	bool operator==(const Vertex_Input& v)
	{
		if (v.Position.x == Position.x &&
			v.Position.y == Position.y &&
			v.Position.z == Position.z)
			return true;
		return false;
	}
};
struct SerializerData
{
	bool isChanged = false;
	bool isSerialized = false;
};
#pragma endregion