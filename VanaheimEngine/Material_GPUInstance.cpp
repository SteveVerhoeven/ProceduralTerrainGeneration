#include "pch.h"
#include "Material_GPUInstance.h"

#include "GameObject.h"

Material_GPUInstance::Material_GPUInstance()
					 : Material("./Resources/Shaders/PosCol3D_GPU_Instance.fx", "GPUInstance")
{}
Material_GPUInstance::~Material_GPUInstance()
{}

void Material_GPUInstance::Initialize(ID3D11Device* pDevice)
{
	Material::Initialize(pDevice);
}
void Material_GPUInstance::Update(GameObject* pParentGameObject)
{
	using namespace DirectX;

	CameraComponent* pCameraComponent{ Locator::GetCameraService() };
	TransformComponent* pTransformComponent{ pParentGameObject->GetComponent<TransformComponent>() };

	DirectX::XMFLOAT4X4 world{ pTransformComponent->GetWorld() };
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
	DirectX::XMFLOAT4X4 view{ pCameraComponent->GetView() };
	DirectX::XMFLOAT4X4 projection{ pCameraComponent->GetProjection() };

	DirectX::XMMATRIX matrix_world{ DirectX::XMLoadFloat4x4(&world) };
	DirectX::XMMATRIX matrix_view{ DirectX::XMLoadFloat4x4(&view) };
	DirectX::XMMATRIX matrix_projection{ DirectX::XMLoadFloat4x4(&projection) };

	DirectX::XMMATRIX wvp{ matrix_world * matrix_view * matrix_projection };

	HRESULT hr{ m_pMat_WorldViewProjVariable->SetMatrix((float*)&wvp) };
	if (FAILED(hr))
		Locator::GetDebugLoggerService()->LogHRESULT(hr, "Material::Update", __FILE__, std::to_string(__LINE__));
}