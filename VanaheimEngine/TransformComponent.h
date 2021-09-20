#pragma once
#include "Component.h"

class TransformComponent final : public Component
{
	public:
		TransformComponent(const DirectX::XMFLOAT3& position = { 0, 0, 0 }, 
						   const DirectX::XMFLOAT4& rotation = { 0, 0, 0, 1 }, 
						   const DirectX::XMFLOAT3& scale = { 1, 1, 1 });
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) noexcept = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		TransformComponent& operator=(TransformComponent&&) noexcept = delete;

		void Translate(const DirectX::XMFLOAT3& position, const bool localChange);
		void Translate(const DirectX::XMVECTOR& position, const bool localChange);
		void Translate(const float x, const float y, const float z, const bool localChange);
		void Rotate(const DirectX::XMFLOAT3& rotation, const bool isEuler);
		void Rotate(const float x, const float y, const float z, const bool isEuler);
		void Scale(const DirectX::XMFLOAT3& scale, const bool localChange);
		void Scale(const float x, const float y, const float z, const bool localChange);

		DirectX::XMFLOAT3 GetPosition() const { return m_Position; }
		DirectX::XMFLOAT3 GetWorldPosition() const { return m_WorldPosition; }
		DirectX::XMFLOAT4 GetRotation() const { return m_Rotation; }
		DirectX::XMFLOAT4 GetWorldRotation() const { return m_WorldRotation; }
		DirectX::XMFLOAT4 GetRotationEuler() const;
		DirectX::XMFLOAT4 GetWorldRotationEuler() const;
		DirectX::XMFLOAT3 GetScale() const { return m_Scale; }
		DirectX::XMFLOAT3 GetWorldScale() const { return m_WorldScale; }

		DirectX::XMFLOAT3 GetForward() const { return m_Forward; }
		DirectX::XMFLOAT3 GetRight() const { return m_Right; }
		DirectX::XMFLOAT3 GetUp() const { return m_Up; }
		DirectX::XMFLOAT3 GetWorldUp() const { return m_WorldUp; }

		DirectX::XMFLOAT4X4 GetWorld();

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		bool m_UpdateWorldMatrix;
		float m_Pitch,
			  m_Yaw,
			  m_Roll;
		DirectX::XMFLOAT3 m_Position,
						  m_WorldPosition,
						  m_Scale,
						  m_WorldScale;
		DirectX::XMFLOAT4 m_Rotation,
						  m_WorldRotation;
		DirectX::XMFLOAT3 m_Forward,
						  m_Right,
						  m_Up,
						  m_WorldUp;
		DirectX::XMFLOAT4X4 m_World;

		void UpdateVectors(const DirectX::XMVECTOR& rotationVector);
};