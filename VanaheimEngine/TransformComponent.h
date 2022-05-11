#pragma once
#include "Component.h"

class Scene;
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

		const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
		const DirectX::XMFLOAT3& GetWorldPosition() const { return m_WorldPosition; }
		const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }
		const DirectX::XMFLOAT4& GetWorldRotation() const { return m_WorldRotation; }
		DirectX::XMFLOAT4		 GetRotationEuler() const;
		DirectX::XMFLOAT4		 GetWorldRotationEuler() const;
		const DirectX::XMFLOAT3& GetScale() const { return m_Scale; }
		const DirectX::XMFLOAT3& GetWorldScale() const { return m_WorldScale; }		 					   
		const DirectX::XMFLOAT3& GetForward() const { return m_Forward; }
		const DirectX::XMFLOAT3& GetRight() const { return m_Right; }
		const DirectX::XMFLOAT3& GetUp() const { return m_Up; }
		const DirectX::XMFLOAT3& GetWorldUp() const { return m_WorldUp; }
		DirectX::XMFLOAT4X4 GetWorld();
		const float GetPitch() const { return m_Pitch; }
		const float GetYaw() const { return m_Yaw; }
		const float GetRoll() const { return m_Roll; }

		void SetPitch(const float pitch) { m_Pitch = pitch; }
		void SetYaw(const float yaw) { m_Yaw = yaw; }
		void SetRoll(const float roll) { m_Roll = roll; }
		void SetLocalPosition(const DirectX::XMFLOAT3& position) { m_Position = position; }
		void SetLocalRotation(const DirectX::XMFLOAT4& rotation) { m_Rotation = rotation; }
		void SetLocalScale(const DirectX::XMFLOAT3& scale) { m_Scale = scale; }
		void SetWorldPosition(const DirectX::XMFLOAT3& position) { m_WorldPosition = position; }
		void SetWorldRotation(const DirectX::XMFLOAT4& rotation) { m_WorldRotation = rotation; }
		void SetWorldScale(const DirectX::XMFLOAT3& scale) { m_WorldScale = scale; }
		void SetForward(const DirectX::XMFLOAT3& forward) { m_Forward = forward; }
		void SetRight(const DirectX::XMFLOAT3& right) { m_Right = right; }
		void SetUp(const DirectX::XMFLOAT3& up) { m_Up = up; }
		void SetWorldUp(const DirectX::XMFLOAT3& worldUp) { m_WorldUp = worldUp; }
		void SetWorld(const DirectX::XMFLOAT4X4& world) { m_World = world; }

		// Serialization
		//void Serialize(YAML::Emitter& out) override;

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