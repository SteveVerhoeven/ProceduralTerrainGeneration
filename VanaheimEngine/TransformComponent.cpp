#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const DirectX::XMFLOAT3& position, 
									   const DirectX::XMFLOAT4& rotation, 
									   const DirectX::XMFLOAT3& scale)
				   : Component()
				   , m_UpdateWorldMatrix(false)
				   , m_Pitch(0)
				   , m_Yaw(0)
				   , m_Roll(0)
				   , m_Position(position)
				   , m_WorldPosition({0, 0, 0})
				   , m_Scale(scale)
				   , m_WorldScale({ 1, 1, 1 })
				   , m_Rotation(rotation)
				   , m_WorldRotation({ 0, 0, 0, 1 })
				   , m_Forward({ 0, 0, 1 })
				   , m_Right({ 1, 0, 0 })
				   , m_Up({ 0, 1, 0 })
				   , m_WorldUp({ 0, 1, 0 })
				   , m_World()
{
	DirectX::XMStoreFloat4x4(&m_World, DirectX::XMMatrixIdentity());
}

void TransformComponent::Initialize(Scene* /*pParentScene*/)
{}
void TransformComponent::PostInitialize(Scene* /*pParentScene*/)
{}
void TransformComponent::Update(const float /*elapsedSec*/)
{}
void TransformComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}

void TransformComponent::Translate(const DirectX::XMFLOAT3& position, const bool localChange)
{
	Translate(position.x, position.y, position.z, localChange);
}
void TransformComponent::Translate(const DirectX::XMVECTOR& position, const bool localChange)
{
	if (localChange)
		XMStoreFloat3(&m_Position, position);
	else
		XMStoreFloat3(&m_WorldPosition, position);

	m_UpdateWorldMatrix = true;
}
void TransformComponent::Translate(const float x, const float y, const float z, const bool localChange)
{
	if (localChange)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}
	else
	{
		m_WorldPosition.x = x;
		m_WorldPosition.y = y;
		m_WorldPosition.z = z;
	}

	m_UpdateWorldMatrix = true;
}
void TransformComponent::Rotate(const DirectX::XMFLOAT3& rotation, const bool isEuler)
{
	Rotate(rotation.x, rotation.y, rotation.z, isEuler);
}
void TransformComponent::Rotate(const float x, const float y, const float z, const bool isEuler)
{
	DirectX::XMVECTOR rotationVector{};
	if (isEuler)	// Converting to radians
	{
		rotationVector = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(x),
																   DirectX::XMConvertToRadians(y),
																   DirectX::XMConvertToRadians(z));
		XMStoreFloat4(&m_WorldRotation, rotationVector);

		m_Pitch = x;
		m_Yaw   = y;
		m_Roll  = z;
	}
	else
	{
		rotationVector = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
		XMStoreFloat4(&m_WorldRotation, rotationVector);

		// Store the values -- TODO: Fix quaternion to euler to store them
		Locator::GetDebugLoggerService()->Log(ErrorLevel::LOG_WARNING, "TransformComponent: Rotate - Fix quaternion to euler to store them");
		m_Pitch += x;
		m_Yaw += y;
		m_Roll += z;
	}

	UpdateVectors(rotationVector);

	m_UpdateWorldMatrix = true;
}
void TransformComponent::Scale(const DirectX::XMFLOAT3& scale, const bool localChange)
{
	Scale(scale.x, scale.y, scale.z, localChange);
}
void TransformComponent::Scale(const float x, const float y, const float z, const bool localChange)
{
	if (localChange)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}
	else
	{
		m_WorldScale.x = x;
		m_WorldScale.y = y;
		m_WorldScale.z = z;
	}

	m_UpdateWorldMatrix = true;
}

void TransformComponent::UpdateVectors(const DirectX::XMVECTOR& rotationVector)
{
	const DirectX::XMVECTOR default_Forward{ DirectX::XMVectorSet(0, 0, 1, 0) };
	const DirectX::XMVECTOR default_Right{ DirectX::XMVectorSet(1, 0, 0, 0) };

	const DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(rotationVector);
	const DirectX::XMVECTOR forward = XMVector3TransformCoord(default_Forward, rotMat);
	const DirectX::XMVECTOR right = XMVector3TransformCoord(default_Right, rotMat);
	const DirectX::XMVECTOR up = DirectX::XMVector3Cross(forward, right);

	XMStoreFloat3(&m_Forward, forward);
	XMStoreFloat3(&m_Right, right);
	XMStoreFloat3(&m_Up, up);
}

DirectX::XMFLOAT4 TransformComponent::GetRotationEuler() const
{
	DirectX::XMFLOAT4 eR{};

	eR.x = m_Pitch;
	eR.y = m_Yaw;
	eR.z = m_Roll;

	return eR;
}
DirectX::XMFLOAT4 TransformComponent::GetWorldRotationEuler() const
{
	DirectX::XMFLOAT4 eR{};

	eR.x = m_Pitch;
	eR.y = m_Yaw;
	eR.z = m_Roll;

	return eR;
}
DirectX::XMFLOAT4X4 TransformComponent::GetWorld()
{
	if (m_UpdateWorldMatrix)
	{
		const DirectX::XMVECTOR rotation{ DirectX::XMLoadFloat4(&m_WorldRotation) };
		const DirectX::XMMATRIX world = DirectX::XMMatrixScaling(m_WorldScale.x, m_WorldScale.y, m_WorldScale.z) *
									    DirectX::XMMatrixRotationQuaternion(rotation) *
									    DirectX::XMMatrixTranslation(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);
		DirectX::XMStoreFloat4x4(&m_World, world);
	}

	return m_World;
}