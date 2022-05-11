// ********************************
// GPU Instancing used
// Reference: https://www.braynzarsoft.net/viewtutorial/q16390-33-instancing-with-indexed-primitives
// ********************************
#include "VanaheimPCH.h"
#include "Mesh.h"

#include "Graphics.h"
#include "OBJParser.h"
#include "Scene.h"

Mesh::Mesh()
	 : Mesh("")
{}
Mesh::Mesh(const std::string& path)
	 : Mesh_Base(path)
{}

void Mesh::PostInitialize(Material* pMaterial)
{
	if (m_IsInitialized)
		return;

	DebugLogger* pLogger = Locator::GetDebugLoggerService();
	ID3D11Device* pDevice{ Locator::GetGraphicsService()->GetDevice() };

	HRESULT hr{};
	hr = CreateVertex_InputLayout(pDevice, pMaterial);
	pLogger->LogHRESULT(hr, "Mesh::CreateVertex_InputLayout", __FILE__, std::to_string(__LINE__));

	hr = CreateInstanceBuffer(pDevice);
	pLogger->LogHRESULT(hr, "Line::CreateInstanceBuffer", __FILE__, std::to_string(__LINE__));

	// ********************************
	// Set that the mesh is initialized
	// ********************************
	m_IsInitialized = true;
}

HRESULT Mesh::CreateInstanceBuffer(ID3D11Device* pDevice)
{
	if (m_AmountInstances < 0)
		return S_OK;

	// ********************************
	// Create instance buffer: Data
	// ********************************
	Scene* pScene{ Locator::GetSceneManagerService()->GetActiveGameScene() };
	std::vector<GameObject*> pGameObjects{ pScene->GetObjects() };
	for (GameObject* pGameObject : pGameObjects)
	{
		ModelComponent* pModelComponent{ pGameObject->GetComponent<ModelComponent>() };
		if (!pModelComponent)
			continue;

		Mesh* pMesh{ pModelComponent->GetMesh() };
		if (pMesh != this)
			continue;

		TransformComponent* pTransformComponent{ pGameObject->GetComponent<TransformComponent>() };
		m_InstancePositionData.push_back(pTransformComponent->GetWorldPosition());
	}

	// If there is only 1 instance of that mesh than clear out the data vector so it won't render as an instance
	if (m_InstancePositionData.size() <= 1)
	{
		m_InstancePositionData.clear();
		return S_OK;
	}

	// ********************************
	// Create instance buffer: DirectX
	// ********************************
	D3D11_BUFFER_DESC instBuffDesc;
	ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));
	instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	instBuffDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * m_AmountInstances;
	instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instBuffDesc.CPUAccessFlags = 0;
	instBuffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA instData;
	ZeroMemory(&instData, sizeof(instData));
	instData.pSysMem = m_InstancePositionData.data();

	/* CreateBuffer - Parameters */
	const D3D11_BUFFER_DESC* pInstanceBufferDesc{ &instBuffDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData_InstanceBuffer{ &instData };
	ID3D11Buffer** ppInstanceBuffer{ &m_pInstanceBuffer };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createbuffer
	return pDevice->CreateBuffer(pInstanceBufferDesc, pInitialData_InstanceBuffer, ppInstanceBuffer);
}