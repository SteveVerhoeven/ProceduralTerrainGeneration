#pragma once
#include "Mesh_Base.h"

class Material;
class Mesh final : public Mesh_Base
{
	public:
		Mesh();
		Mesh(const std::string& path);
		~Mesh() = default;

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) noexcept = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) noexcept = delete;

		virtual void PostInitialize(Material* pMaterial) override;

	private:
		HRESULT CreateInstanceBuffer(ID3D11Device* pDevice) override;
};