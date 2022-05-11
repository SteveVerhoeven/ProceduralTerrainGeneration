#pragma once
#include "Material.h"

class GameObject;
class Material_Basic final : public Material
{
	public:
		Material_Basic();
		virtual ~Material_Basic();

		Material_Basic(const Material_Basic&) = delete;
		Material_Basic& operator=(const Material_Basic&) = delete;
		Material_Basic(Material_Basic&&) = delete;
		Material_Basic& operator=(Material_Basic&&) = delete;

		void Initialize(ID3D11Device* pDevice) override;
		void Update(GameObject* pParentGameObject) override;

	protected:

	private:

};