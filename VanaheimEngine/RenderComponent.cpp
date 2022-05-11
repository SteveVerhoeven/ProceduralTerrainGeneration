#include "VanaheimPCH.h"
#include "RenderComponent.h"

#include "Mesh.h"
#include "Line.h"
#include "Timer.h"
#include "Graphics.h"
#include "Material.h"
#include "Mesh_Base.h"
#include "MathHelper.h"

RenderComponent::RenderComponent()
				: Component()
				, m_CanRenderComponent(false)
{}

void RenderComponent::Initialize(Scene* /*pParentScene*/)
{}
void RenderComponent::PostInitialize(Scene* /*pParentScene*/)
{}
void RenderComponent::Update(const float /*elapsedSec*/)
{}
void RenderComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}

void RenderComponent::Render3DMesh(ID3D11DeviceContext* pDeviceContext, Mesh* p3DMesh, Material* pEffect)
{
	if (!m_CanRenderComponent)
		return;

	SetBlendingState(pDeviceContext);
	SetRasterizerState(pDeviceContext);
	SetFilterMethod(pEffect);
	SetVertexBuffers(pDeviceContext, p3DMesh);
	SetIndexBuffers(pDeviceContext, p3DMesh);
	SetInputLayout(pDeviceContext, p3DMesh);
	SetPrimTopology(pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Render(pDeviceContext, pEffect, p3DMesh);
}
void RenderComponent::RenderLine(ID3D11DeviceContext* pDeviceContext, Line* pLine, Material* pEffect)
{
	if (!pLine->GetRenderLine())
		return;

	SetBlendingState(pDeviceContext);
	SetRasterizerState(pDeviceContext);
	SetFilterMethod(pEffect);
	SetVertexBuffers(pDeviceContext, pLine);
	SetIndexBuffers(pDeviceContext, pLine);
	SetInputLayout(pDeviceContext, pLine);
	SetPrimTopology(pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	Render(pDeviceContext, pEffect, pLine);
}

void RenderComponent::SetBlendingState(ID3D11DeviceContext* pDeviceContext)
{
	UNREFERENCED_PARAMETER(pDeviceContext);
	//if (m_pEffect_Lambert)
	//{
	//	/* CreateBlendState - Parameters */
	//	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//	UINT sampleMask = 0xffffffff;
	//	// Explanation for all parameters in link below
	//	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-omsetblendstate
	//	if (!m_Transparancy_ON)
	//		pDeviceContext->OMSetBlendState(m_pEffect_Lambert->GetBlendState_OFF(), blendFactor, sampleMask);
	//	else
	//		pDeviceContext->OMSetBlendState(m_pEffect_Lambert->GetBlendState_ON(), blendFactor, sampleMask);
	//}
}
void RenderComponent::SetVertexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh* pMesh)
{
	if (pMesh->GetAmountInstances() > 0)
		SetVertexBuffersInstanced(pDeviceContext, pMesh);
	else
		SetVertexBuffersNormal(pDeviceContext, pMesh);
}
void RenderComponent::SetVertexBuffers(ID3D11DeviceContext* pDeviceContext, Line* pLine)
{
	if (pLine->GetAmountInstances() > 0)
		SetVertexBuffersInstanced(pDeviceContext, pLine);
	else
		SetVertexBuffersNormal(pDeviceContext, pLine);
}

void RenderComponent::SetIndexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase)
{
	ID3D11Buffer* iBuffer{ pMeshBase->GetIBuffer() };
	// ********************************
	// Set index buffer
	// ********************************
	/* IASetIndexBuffer - Parameters */
	ID3D11Buffer* pIndexBuffer{ iBuffer };
	DXGI_FORMAT format{ DXGI_FORMAT_R32_UINT };
	UINT offset{ 0 };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, format, offset);
}
void RenderComponent::SetInputLayout(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase)
{
	// ********************************
// Set the input layout
// ********************************
/* IASetInputLayout - Parameters */
	ID3D11InputLayout* pInputLayout{ pMeshBase->GetInputLayout() };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetinputlayout
	pDeviceContext->IASetInputLayout(pInputLayout);
}
void RenderComponent::SetRasterizerState(ID3D11DeviceContext* pDeviceContext)
{
	UNREFERENCED_PARAMETER(pDeviceContext);

	//if (m_pEffect_LambertPhong)
	//{
	//	/* RSSetState - Parameters */
	//	// Explanation for all parameters in link below
	//	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-rssetstate
	//	if (m_Cullmode == Cullmode::D3D11_CULL_BACK)
	//		pDeviceContext->RSSetState(m_pEffect_LambertPhong->GetRasterizerState_BACK());
	//	else if (m_Cullmode == Cullmode::D3D11_CULL_FRONT)
	//		pDeviceContext->RSSetState(m_pEffect_LambertPhong->GetRasterizerState_FRONT());
	//	else if (m_Cullmode == Cullmode::D3D11_CULL_NONE)
	//		pDeviceContext->RSSetState(m_pEffect_LambertPhong->GetRasterizerState_NONE());
	//}
}
void RenderComponent::SetFilterMethod(Material* pEffect)
{
	ID3DX11EffectSamplerVariable* samplerVariable{ pEffect->GetSamplerVariable() };
	Material::SamplingType samplingState{ pEffect->GetSamplingType() };

	if (samplingState == Material::SamplingType::POINT)
		samplerVariable->SetSampler(0, pEffect->GetSamplerPoint());
	else if (samplingState == Material::SamplingType::LINEAR)
		samplerVariable->SetSampler(0, pEffect->GetSamplerLinear());
	else if (samplingState == Material::SamplingType::ANISOTROPIC)
		samplerVariable->SetSampler(0, pEffect->GetSamplerAnisotropic());
}
void RenderComponent::SetPrimTopology(ID3D11DeviceContext* pDeviceContext, const D3D11_PRIMITIVE_TOPOLOGY& topo)
{
	// ********************************
	// Set primitive topology
	// ********************************
	/* IASetPrimitiveTopology - Parameters */
	const D3D11_PRIMITIVE_TOPOLOGY topology{ topo };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	pDeviceContext->IASetPrimitiveTopology(topology);
}
void RenderComponent::Render(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh* pMesh)
{
	if (pMesh->GetAmountInstances() > 0)
		RenderInstanced(pDeviceContext, pEffect, pMesh);
	else
		RenderNormal(pDeviceContext, pEffect, pMesh);
}
void RenderComponent::Render(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Line* pLine)
{
	if (pLine->GetAmountInstances() > 0)
		RenderInstanced(pDeviceContext, pEffect, pLine);
	else
		RenderNormal(pDeviceContext, pEffect, pLine);
}

void RenderComponent::SetVertexBuffersNormal(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase)
{
	ID3D11Buffer* vBuffer{ pMeshBase->GetVBuffer() };
	// ********************************
	// Set vertex buffer
	// ********************************
	UINT stride{ sizeof(Vertex_Input) };
	UINT offset1{ 0 };
	/* IASetVertexBuffers - Parameters */
	UINT startSlot{ 0 };
	UINT numBuffers{ 1 };
	ID3D11Buffer* const* ppVertexBuffers{ &vBuffer };
	const UINT* pStrides{ &stride };
	const UINT* pOffsets{ &offset1 };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void RenderComponent::SetVertexBuffersInstanced(ID3D11DeviceContext* pDeviceContext, Mesh* pMesh)
{
	ID3D11Buffer* vBuffer{ pMesh->GetVBuffer() };
	ID3D11Buffer* instBuffer{ pMesh->GetInstanceBuffer() };
	ID3D11Buffer* vertInstBuffers[2] = { vBuffer, instBuffer };
	// ********************************
	// Set vertex buffer
	// ********************************
	UINT strides[2] = { sizeof(Vertex_Input), sizeof(DirectX::XMFLOAT4) };
	UINT offsets[2] = { 0, 0 };
	/* IASetVertexBuffers - Parameters */
	UINT startSlot{ 0 };
	UINT numBuffers{ 2 };
	ID3D11Buffer* const* ppVertexBuffers{ vertInstBuffers };
	const UINT* pStrides{ strides };
	const UINT* pOffsets{ offsets };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets);
}
void RenderComponent::SetVertexBuffersInstanced(ID3D11DeviceContext* pDeviceContext, Line* pLine)
{
	ID3D11Buffer* vBuffer{ pLine->GetVBuffer() };
	ID3D11Buffer* instBuffer{ pLine->GetInstanceBuffer() };
	ID3D11Buffer* instBufferLength1{ pLine->GetInstanceBufferLengthV1() };
	ID3D11Buffer* instBufferLength2{ pLine->GetInstanceBufferLengthV2() };
	ID3D11Buffer* vertInstBuffers[4] = { vBuffer, instBuffer, instBufferLength1, instBufferLength2 };
	// ********************************
	// Set vertex buffer
	// ********************************
	UINT strides[4] = { sizeof(Vertex_Input), sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT4), sizeof(DirectX::XMFLOAT4) };
	UINT offsets[4] = { 0, 0, 0, 0 };
	/* IASetVertexBuffers - Parameters */
	UINT startSlot{ 0 };
	UINT numBuffers{ 4 };
	ID3D11Buffer* const* ppVertexBuffers{ vertInstBuffers };
	const UINT* pStrides{ strides };
	const UINT* pOffsets{ offsets };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void RenderComponent::RenderNormal(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh_Base* pMeshBase)
{
	ID3DX11EffectTechnique* pTechnique{ pEffect->GetTechnique() };

	D3DX11_TECHNIQUE_DESC techDesc{};
	pTechnique->GetDesc(&techDesc);
	for (UINT p{}; p < techDesc.Passes; ++p)
	{
		// COMMENT HERE
		pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);

		// ****************************************
		// Set up the DrawIndexed function
		// ****************************************
		/* DrawIndexed - Parameters */
		UINT IndexCount{ pMeshBase->GetAmountIndices() };
		UINT StartIndexLocation{ 0 };
		INT  BaseVertexLocation{ 0 };

		// Explanation for all parameters in link below
		// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexed
		pDeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
	}
}

void RenderComponent::RenderInstanced(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh_Base* pMeshBase)
{
	if (pMeshBase->GetIsRendered())
		return;

	ID3DX11EffectTechnique* pTechnique{ pEffect->GetTechnique() };

	D3DX11_TECHNIQUE_DESC techDesc{};
	pTechnique->GetDesc(&techDesc);
	for (UINT p{}; p < techDesc.Passes; ++p)
	{
		// COMMENT HERE
		pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);

		// ****************************************
		// Set up the DrawIndexedInstanced function
		// ****************************************
		/* DrawIndexedInstanced - Parameters */
		UINT IndexCountPerInstance{ pMeshBase->GetAmountIndices() };
		UINT InstanceCount{ pMeshBase->GetAmountInstances() };
		UINT StartIndexLocation{ 0 };
		INT  BaseVertexLocation{ 0 };
		UINT StartInstanceLocation{ 0 };

		// Explanation for all parameters in link below
		// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexedinstanced
		pDeviceContext->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	}

	pMeshBase->SetIsRendered(true);
}

//void RenderComponent::Serialize(YAML::Emitter& out)
//{
//	out << YAML::Key << "RenderComponent";
//	out << YAML::BeginMap;
//	out << YAML::Key << "RenderComponent" << YAML::Value << m_CanRenderComponent;
//	out << YAML::EndMap;
//}