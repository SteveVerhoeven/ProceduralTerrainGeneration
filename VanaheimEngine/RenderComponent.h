#pragma once
#include "Component.h"

class Mesh;
class Line;
class Scene;
class Material;
class Mesh_Base;
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
		void RenderLine(ID3D11DeviceContext* pDeviceContext, Line* pLine, Material* pEffect);

		void EnableRenderComponent() { m_CanRenderComponent = true; }
		void SetCanRenderComponent(const bool canRenderComponent) { m_CanRenderComponent = canRenderComponent; }
		bool GetCanRenderComponent() const { return m_CanRenderComponent; }
		
		// Serialization
		//void Serialize(YAML::Emitter& out) override;

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
		void SetVertexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh* pMeshBase);
		void SetVertexBuffers(ID3D11DeviceContext* pDeviceContext, Line* pMeshBase);
		void SetIndexBuffers(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase);
		void SetInputLayout(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase);
		void SetPrimTopology(ID3D11DeviceContext* pDeviceContext, const D3D11_PRIMITIVE_TOPOLOGY& topology);
		void Render(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh* pMeshBase);
		void Render(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Line* pMeshBase);

		// Instancing functions
		void SetVertexBuffersNormal(ID3D11DeviceContext* pDeviceContext, Mesh_Base* pMeshBase);
		void SetVertexBuffersInstanced(ID3D11DeviceContext* pDeviceContext, Mesh* pMeshBase);
		void SetVertexBuffersInstanced(ID3D11DeviceContext* pDeviceContext, Line* pMeshBase);

		void RenderNormal(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh_Base* pMeshBase);
		void RenderInstanced(ID3D11DeviceContext* pDeviceContext, Material* pEffect, Mesh_Base* pMeshBase);
};