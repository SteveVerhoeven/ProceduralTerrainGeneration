#pragma once
#include "Component.h"

class CameraComponent final : public Component
{
	public:
		CameraComponent();
		virtual ~CameraComponent() = default;

		CameraComponent(const CameraComponent&) = delete;
		CameraComponent(CameraComponent&&) noexcept = delete;
		CameraComponent& operator=(const CameraComponent&) = delete;
		CameraComponent& operator=(CameraComponent&&) noexcept = delete;

		DirectX::XMFLOAT4X4 GetView();
		DirectX::XMFLOAT4X4 GetProjection();
		DirectX::XMFLOAT4X4 GetViewProjection();
		DirectX::XMFLOAT4X4 GetViewInverse();
		DirectX::XMFLOAT4X4 GetViewProjectionInverse();

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
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