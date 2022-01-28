#pragma once
#include "Material.h"

class Material_GPUInstance final : public Material
{
	public:
		Material_GPUInstance();
		virtual ~Material_GPUInstance();

		Material_GPUInstance(const Material_GPUInstance&) = delete;
		Material_GPUInstance& operator=(const Material_GPUInstance&) = delete;
		Material_GPUInstance(Material_GPUInstance&&) = delete;
		Material_GPUInstance& operator=(Material_GPUInstance&&) = delete;

		void Initialize(ID3D11Device* pDevice) override;
		void Update(GameObject* pParentGameObject) override;

	protected:

	private:
};