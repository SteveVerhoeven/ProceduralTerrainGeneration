#pragma once
#include "Component.h"

class Mesh;
class Material;
class RenderComponent final : public Component
{
	public:
		RenderComponent();
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) noexcept = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) noexcept = delete;

		void Render3DMesh(ID3D11DeviceContext* pDeviceContext, Mesh* p3DMesh, Material* pEffect);

		void EnableRenderComponent() { m_CanRenderComponent = true; }
		bool GetCanRenderComponent() const { return m_CanRenderComponent; }

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		bool m_CanRenderComponent;

		// Render 3D Mesh functions
		void SetFilterMethod(Material* pEffect);
		void SetRasterizerState(ID3D11DeviceContext* pDeviceContext);
		void SetBlendingState(ID3D11DeviceContext* pDeviceContext);
		void SetVertexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh* p3DMesh);
		void SetIndexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh* p3DMesh);
		void SetInputLayout(ID3D11DeviceContext* pDeviceContext, Mesh* p3DMesh);
		void SetPrimTopology(ID3D11DeviceContext* pDeviceContext);
		void Render(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh* p3DMesh);
};