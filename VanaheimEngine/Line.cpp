#include "VanaheimPCH.h"
#include "Line.h"

#include "Scene.h"
#include "Graphics.h"

Line::Line(const DirectX::XMFLOAT3& /*position_Start*/, const DirectX::XMFLOAT3& /*position_End*/, const LineType& LineType, const float length, const float height)
	 : Mesh_Base()
	 , m_RenderLine(true)
	 , m_LineType(LineType)
	 , m_pInstanceBuffer_Length_V1(nullptr)
	 , m_pInstanceBuffer_Length_V2(nullptr)
	 , m_InstanceLengthDataV1(std::vector<DirectX::XMFLOAT4>())
	 , m_InstanceLengthDataV2(std::vector<DirectX::XMFLOAT4>())
{
	Vertex_Input vStart{};
	vStart.Position = { 0.f, 0.f, 0.f };
	vStart.Color = { 1.f, 1.f, 1.f };
	vStart.InstanceLengthV1 = { 0.f, 0.f, 0.f, length };
	Vertex_Input vEnd{};
	vEnd.Position = { 1.f, 0.f, 0.f };
	vEnd.Color = { 1.f, 1.f, 1.f };
	vEnd.InstanceLengthV2 = { (LineType == LineType::HORIZONTAL_X) ? 1.f : 0.f,
							  (LineType == LineType::VERTICAL)     ? 1.f : 0.f,
							  (LineType == LineType::HORIZONTAL_Z) ? 1.f : 0.f, (LineType == LineType::VERTICAL) ? height : length };

	std::vector<Vertex_Input> vertices;
	vertices.push_back(vStart);
	vertices.push_back(vEnd);
	std::vector<uint32_t> indices;
	indices.push_back(0);
	indices.push_back(1);

	m_InstanceLengthDataV1.push_back(vStart.InstanceLengthV1);
	m_InstanceLengthDataV2.push_back(vEnd.InstanceLengthV2);

	Initialize(vertices, indices);
}
Line::Line(const LineCorner& sc1, const LineCorner& sc2, const LineType& LineType, const float sizeXZ, const float sizeY)
	   : Line(GetCornerPosition(sc1, sizeXZ, sizeY, LineType), GetCornerPosition(sc2, sizeXZ, sizeY, LineType), LineType, sizeXZ, sizeY)
{}

void Line::PostInitialize(Material* pMaterial)
{
	Mesh_Base::PostInitialize(pMaterial);

	if (m_IsInitialized)
		return;

	DebugLogger* pLogger = Locator::GetDebugLoggerService();
	ID3D11Device* pDevice{ Locator::GetGraphicsService()->GetDevice() };

	HRESULT hr{};
	hr = CreateInstanceBuffer(pDevice);
	pLogger->LogHRESULT(hr, "Line::CreateInstanceBuffer", __FILE__, std::to_string(__LINE__));
	
	hr = CreateInstanceBufferLengthV1(pDevice);
	pLogger->LogHRESULT(hr, "Line::CreateInstanceBufferLengthV1", __FILE__, std::to_string(__LINE__));

	hr = CreateInstanceBufferLengthV2(pDevice);
	pLogger->LogHRESULT(hr, "Line::CreateInstanceBufferLengthV2", __FILE__, std::to_string(__LINE__));

	//BInspectorVariable* InspectorVarRenderUI{ new BInspectorVariable() };
	//if (m_LineType == LineType::HORIZONTAL_X)
	//	InspectorVarRenderUI->name = "Render x-axis Lines (Right)";
	//else if (m_LineType == LineType::HORIZONTAL_Z)
	//	InspectorVarRenderUI->name = "Render z-axis Lines (Front)";
	//else 
	//	InspectorVarRenderUI->name = "Render y-axis Lines (Up)";
	//InspectorVarRenderUI->value = &m_RenderLine;
	//InspectorVarRenderUI->varRange = { 0, 0 };
	//UIManager* pUIManager{ Locator::GetUIManagerService() };
	//InspectorUI* pVanaheimUI{ pUIManager->GetUI<InspectorUI>() };
	//if (pVanaheimUI)
	//	pVanaheimUI->StoreVariable(InspectorVarRenderUI);

	// ********************************
	// Set that the mesh is initialized
	// ********************************
	m_IsInitialized = true;
}

void Line::StoreLengthsFromInstances(const std::vector<DirectX::XMFLOAT4>& lenghts)
{
	m_InstanceLengthDataV2.push_back(lenghts[1]);
}
void Line::StorePositionsFromInstances(const DirectX::XMFLOAT3& positions/*const std::vector<DirectX::XMFLOAT3>& positions*/)
{
	m_InstancePositionData.push_back(positions);
}

HRESULT Line::CreateInstanceBuffer(ID3D11Device* pDevice)
{
	// If there is only 1 instance of that mesh than clear out the data vector so it won't render as an instance
	if (m_InstancePositionData.size() <= 1)
	{
		//m_InstancePositionData.clear();
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
	//instData.pSysMem = m_InstancePositionData.data();
	instData.pSysMem = m_InstancePositionData.data();

	/* CreateBuffer - Parameters */
	const D3D11_BUFFER_DESC* pInstanceBufferDesc{ &instBuffDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData_InstanceBuffer{ &instData };
	ID3D11Buffer** ppInstanceBuffer{ &m_pInstanceBuffer };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createbuffer
	return pDevice->CreateBuffer(pInstanceBufferDesc, pInitialData_InstanceBuffer, ppInstanceBuffer);
}
HRESULT Line::CreateInstanceBufferLengthV1(ID3D11Device* pDevice)
{
	// If there is only 1 instance of that mesh than clear out the data vector so it won't render as an instance
	if (m_InstanceLengthDataV1.size() <= 1)
	{
		m_InstanceLengthDataV1.clear();
		return S_OK;
	}

	// ********************************
	// Create instance buffer: DirectX
	// ********************************
	D3D11_BUFFER_DESC instBuffDesc;
	ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));
	instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	instBuffDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * (2 * m_AmountInstances);
	instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instBuffDesc.CPUAccessFlags = 0;
	instBuffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA instData;
	ZeroMemory(&instData, sizeof(instData));
	instData.pSysMem = m_InstanceLengthDataV1.data();

	/* CreateBuffer - Parameters */
	const D3D11_BUFFER_DESC* pInstanceBufferDesc{ &instBuffDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData_InstanceBuffer{ &instData };
	ID3D11Buffer** ppInstanceBuffer{ &m_pInstanceBuffer_Length_V1 };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createbuffer
	return pDevice->CreateBuffer(pInstanceBufferDesc, pInitialData_InstanceBuffer, ppInstanceBuffer);
}
HRESULT Line::CreateInstanceBufferLengthV2(ID3D11Device* pDevice)
{
	// ********************************
	// Create instance buffer: DirectX
	// ********************************
	D3D11_BUFFER_DESC instBuffDesc;
	ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));
	instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	instBuffDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * (2 * m_AmountInstances);
	instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instBuffDesc.CPUAccessFlags = 0;
	instBuffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA instData;
	ZeroMemory(&instData, sizeof(instData));
	instData.pSysMem = m_InstanceLengthDataV2.data();

	/* CreateBuffer - Parameters */
	const D3D11_BUFFER_DESC* pInstanceBufferDesc{ &instBuffDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData_InstanceBuffer{ &instData };
	ID3D11Buffer** ppInstanceBuffer{ &m_pInstanceBuffer_Length_V2 };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createbuffer
	return pDevice->CreateBuffer(pInstanceBufferDesc, pInitialData_InstanceBuffer, ppInstanceBuffer);
}

DirectX::XMFLOAT3 Line::GetCornerPosition(const LineCorner& LineCorner, const float sizeXZ, const float sizeY, const LineType& LineType) const
{
	const float sizeHalfXZ{ sizeXZ / 2.f };
	const float sizeHalfY{ sizeY / 2.f };
	switch (LineCorner)
	{
		case LineCorner::TOP_LEFT_FRONT:
			return DirectX::XMFLOAT3{ 0.f,  (LineType == LineType::VERTICAL) ? sizeY : 0.f, 0.f };
			break;
		case LineCorner::TOP_RIGHT_FRONT:
			return DirectX::XMFLOAT3{ sizeXZ,  (LineType == LineType::VERTICAL) ? sizeY : 0.f, 0.f };

			break;
		case LineCorner::BOTTOM_LEFT_FRONT:
			return DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
			break;
		case LineCorner::BOTTOM_RIGHT_FRONT:
			return DirectX::XMFLOAT3{ sizeXZ, 0.f, 0.f };
			break;

		case LineCorner::TOP_LEFT_BACK:
			return DirectX::XMFLOAT3{ 0.f, (LineType == LineType::VERTICAL) ? sizeY : 0.f, sizeXZ };
			break;
		case LineCorner::TOP_RIGHT_BACK:
			return DirectX::XMFLOAT3{ sizeXZ, (LineType == LineType::VERTICAL) ? sizeY : 0.f, sizeXZ };
			break;
		case LineCorner::BOTTOM_LEFT_BACK:
			return DirectX::XMFLOAT3{ 0.f, 0.f, sizeXZ };
			break;
		case LineCorner::BOTTOM_RIGHT_BACK:
			return DirectX::XMFLOAT3{ sizeXZ, 0.f, sizeXZ };
			break;

		default:
			return DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
			break;
	}
}
