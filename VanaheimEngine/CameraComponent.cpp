#include "VanaheimPCH.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "Window.h"
#include "Scene.h"

CameraComponent::CameraComponent()
				: Component()
				, m_IsMainCamera(false)
				// Dirty Flag pattern
				// Reference: https://gameprogrammingpatterns.com/dirty-flag.html
				, m_UpdateView(false)
				, m_UpdateProjection(false)
				, m_UpdateViewProjection(false)
				, m_UpdateViewInverse(false)
				, m_UpdateViewProjectionInverse(false)
				// Camera
				, m_Near(0.1f)
				, m_Far(5000.f)
				, m_FOV(DirectX::XM_PIDIV4)	// 45 degrees
				, m_View()
				, m_Projection()
				, m_ViewProjection()
				, m_ViewInverse()
				, m_ViewProjectionInverse()
{
	DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ViewProjection, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ViewInverse, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ViewProjectionInverse, DirectX::XMMatrixIdentity());
}

void CameraComponent::Initialize(Scene* /*pParentScene*/)
{}
void CameraComponent::PostInitialize(Scene* /*pParentScene*/)
{}
void CameraComponent::Update(const float /*elapsedSec*/)
{}
void CameraComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}

const DirectX::XMFLOAT4X4& CameraComponent::GetView()
{
	// To be able to do XMVECTOR + XMVECTOR
	// Reference: https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	//if (m_UpdateView)
	{
		DirectX::XMMATRIX view{};
		{
			TransformComponent* pTransformComponent{ m_pParentObject->GetComponent<TransformComponent>() };

			// ********************************
			// Calculate the VIEW matrix
			// ********************************
			const DirectX::XMFLOAT3 worldPosition{ pTransformComponent->GetWorldPosition() };
			const DirectX::XMFLOAT3 forward{ pTransformComponent->GetForward() };
			const DirectX::XMFLOAT3 up{ pTransformComponent->GetUp() };

			/* XMMatrixLookAtLH - Parameters */
			DirectX::XMVECTOR EyePosition{ DirectX::XMLoadFloat3(&worldPosition) };
			DirectX::XMVECTOR FocusPosition{ DirectX::XMLoadFloat3(&worldPosition) + DirectX::XMLoadFloat3(&forward) };
			DirectX::XMVECTOR UpDirection{ DirectX::XMLoadFloat3(&up) };

			// Explanation for all parameters in link below
			// Reference: https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixlookatlh
			view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);
		}
		XMStoreFloat4x4(&m_View, view);

		m_UpdateView = false;
	}

	return m_View;
}
const DirectX::XMFLOAT4X4& CameraComponent::GetProjection()
{
	//if (m_UpdateProjection)
	{
		DirectX::XMMATRIX projection{};
		{
			// ********************************
			// Calculate the PROJECTION matrix
			// ********************************
			/* XMMatrixLookAtLH - Parameters */
			float FovAngleY{ m_FOV };
			float AspectRatio{ Locator::GetWindowService()->GetAscpectRatio() };
			float NearZ{ m_Near };
			float FarZ{ m_Far };

			// Explanation for all parameters in link below
			// Reference: https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixperspectivefovlh
			projection = DirectX::XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
		}
		XMStoreFloat4x4(&m_Projection, projection);

		m_UpdateProjection = false;
	}

	return m_Projection;
}
const DirectX::XMFLOAT4X4& CameraComponent::GetViewProjection()
{
	// To be able to do XMMATRIX * XMMATRIX
	// Reference: https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	if (m_UpdateViewProjection)
	{
		DirectX::XMMATRIX viewProjection{};
		{
			const DirectX::XMFLOAT4X4 view{ GetView() };
			const DirectX::XMFLOAT4X4 projection{ GetProjection() };

			const DirectX::XMMATRIX matrix_view{ DirectX::XMLoadFloat4x4(&view) };
			const DirectX::XMMATRIX matrix_projection{ DirectX::XMLoadFloat4x4(&projection) };
			viewProjection = matrix_view * matrix_projection;
		}
		XMStoreFloat4x4(&m_ViewProjection, viewProjection);

		m_UpdateViewProjection = false;
	}

	return m_ViewProjection;
}
const DirectX::XMFLOAT4X4& CameraComponent::GetViewInverse()
{
	if (m_UpdateViewInverse)
	{
		DirectX::XMMATRIX viewInverse{};
		{
			// ********************************
			// Calculate the VIEW_INVERSE matrix
			// ********************************
			const DirectX::XMFLOAT4X4 view{ GetView() };
			const DirectX::XMMATRIX matrix_view{ DirectX::XMLoadFloat4x4(&view) };

			/* XMMatrixInverse - Parameters */
			DirectX::XMVECTOR* pDeterminant{ nullptr };
			DirectX::FXMMATRIX M{ matrix_view };

			// Explanation for all parameters in link below
			// Reference: https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixinverse
			viewInverse = DirectX::XMMatrixInverse(pDeterminant, M);
		}
		XMStoreFloat4x4(&m_ViewInverse, viewInverse);

		m_UpdateViewInverse = false;
	}

	return m_ViewInverse;
}
const DirectX::XMFLOAT4X4& CameraComponent::GetViewProjectionInverse()
{
	if (m_UpdateViewProjectionInverse)
	{
		DirectX::XMMATRIX viewProjectionInverse{};
		{	
			// ********************************
			// Calculate the VIEW_PROJECTION_INVERSE matrix
			// ********************************
			const DirectX::XMFLOAT4X4 view{ GetView() };
			const DirectX::XMFLOAT4X4 projection{ GetProjection() };

			const DirectX::XMMATRIX matrix_view{ DirectX::XMLoadFloat4x4(&view) };
			const DirectX::XMMATRIX matrix_projection{ DirectX::XMLoadFloat4x4(&projection) };
			const DirectX::XMMATRIX matrix_viewProjection{ matrix_view * matrix_projection };

			/* XMMatrixInverse - Parameters */
			DirectX::XMVECTOR* pDeterminant{ nullptr };
			DirectX::FXMMATRIX M{ matrix_viewProjection };

			// Explanation for all parameters in link below
			// Reference: https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixinverse
			viewProjectionInverse = DirectX::XMMatrixInverse(pDeterminant, M);
		}
		XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInverse);

		m_UpdateViewProjectionInverse = false;
	}

	return m_ViewProjectionInverse;
}

void CameraComponent::SetIsMainCamera(const bool isMainCamera)
{
	m_IsMainCamera = isMainCamera;

	if (isMainCamera)
	{
		Locator::ProvideSceneCameraService(this);
		if (m_pParentObject && m_pParentObject->GetParentScene())
		{
			GameObject* pOldCameraObject{ m_pParentObject->GetParentScene()->GetSceneCamera() };
			m_pParentObject->GetParentScene()->SetSceneCamera(m_pParentObject);
			pOldCameraObject->GetComponent<CameraComponent>()->SetIsMainCamera(false);
		}
	}
	else
	{
		if (m_pParentObject == m_pParentObject->GetParentScene()->GetSceneCamera())
		{
			m_IsMainCamera = true;
		}
	}
}


//void CameraComponent::Serialize(YAML::Emitter& out)
//{
//	out << YAML::Key << "CameraComponent";
//	out << YAML::BeginMap;
//	out << YAML::Key << "IsMainCamera" << YAML::Value << m_IsMainCamera;
//
//	out << YAML::Key << "Near" << YAML::Value << m_Near;
//	out << YAML::Key << "Far" << YAML::Value << m_Far;
//	out << YAML::Key << "FOV" << YAML::Value << m_FOV;
//
//	out << YAML::Key << "View" << YAML::Value << m_View;
//	out << YAML::Key << "Projection" << YAML::Value << m_Projection;
//	out << YAML::Key << "ViewProjection" << YAML::Value << m_ViewProjection;
//	out << YAML::Key << "ViewInverse" << YAML::Value << m_ViewInverse;
//	out << YAML::Key << "ViewProjectionInverse" << YAML::Value << m_ViewProjectionInverse;
//	out << YAML::EndMap;
//}