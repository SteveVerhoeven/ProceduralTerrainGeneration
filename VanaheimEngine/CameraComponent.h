#pragma once
#include "Component.h"

class Scene;
class CameraComponent final : public Component
{
	public:
		CameraComponent();
		virtual ~CameraComponent() = default;

		CameraComponent(const CameraComponent&) = delete;
		CameraComponent(CameraComponent&&) noexcept = delete;
		CameraComponent& operator=(const CameraComponent&) = delete;
		CameraComponent& operator=(CameraComponent&&) noexcept = delete;

		const DirectX::XMFLOAT4X4& GetView();
		const DirectX::XMFLOAT4X4& GetProjection();
		const DirectX::XMFLOAT4X4& GetViewProjection();
		const DirectX::XMFLOAT4X4& GetViewInverse();
		const DirectX::XMFLOAT4X4& GetViewProjectionInverse();

		float GetNear() const { return m_Near; }
		float GetFar() const { return m_Far; }
		float GetFOV_Radians() const { return m_FOV; }
		float GetFOV_Degrees() const { return DirectX::XMConvertToDegrees(m_FOV); }

		void SetNear(const float nearValue) { m_Near = nearValue; }
		void SetFar(const float farValue) { m_Far = farValue; }
		void SetFOV(const float fovValue) { m_FOV = fovValue; }

		bool GetIsMainCamera() const { return m_IsMainCamera; }
		void SetIsMainCamera(const bool isMainCamera);

		void SetView(const DirectX::XMFLOAT4X4& view) { m_View = view; };
		void SetProjection(const DirectX::XMFLOAT4X4& projection) { m_Projection = projection; };
		void SetViewProjection(const DirectX::XMFLOAT4X4& vp) { m_ViewProjection = vp; };
		void SetViewInverse(const DirectX::XMFLOAT4X4& vi) { m_ViewInverse = vi; };
		void SetViewProjectionInverse(const DirectX::XMFLOAT4X4& vpi) { m_ViewProjectionInverse = vpi; };

		// Serialization
		//void Serialize(YAML::Emitter& out) override;

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		bool m_IsMainCamera;
		bool m_UpdateView,
			 m_UpdateProjection,
			 m_UpdateViewProjection,
			 m_UpdateViewInverse,
			 m_UpdateViewProjectionInverse;
		float m_Near,
			  m_Far,
			  m_FOV;
		DirectX::XMFLOAT4X4 m_View,
							m_Projection,
							m_ViewProjection,
							m_ViewInverse,
							m_ViewProjectionInverse;
};