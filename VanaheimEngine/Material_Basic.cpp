#include "VanaheimPCH.h"
#include "Material_Basic.h"

Material_Basic::Material_Basic()
			   : Material("./Resources/Shaders/PosCol3D_MeshTest.fx", "BASIC")
{}
Material_Basic::~Material_Basic()
{}

void Material_Basic::Initialize(ID3D11Device* pDevice)
{ Material::Initialize(pDevice); }
void Material_Basic::Update(GameObject* pParentGameObject)
{ Material::Update(pParentGameObject); }