#include "VanaheimPCH.h"
#include "ModelComponent.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Graphics.h"

ModelComponent::ModelComponent()
			   : ModelComponent(nullptr)
{}
ModelComponent::ModelComponent(const std::string& path)
			   : Component()
			   , m_pMesh(nullptr)
			   , m_pMaterial(nullptr)
			   , m_FilePath(path)
			   , m_pTextures(std::vector<Texture*>())
{}
ModelComponent::ModelComponent(Mesh* pMesh)
			   : Component()
			   , m_pMesh(pMesh)
			   , m_pMaterial(nullptr)
			   , m_FilePath("")
{}

void ModelComponent::Initialize(Scene* /*pParentScene*/)
{	
	if (!m_pMesh)
		m_pMesh = Locator::GetResourceManagerService()->Load3DMesh(m_pParentObject->GetComponent<NameComponent>()->GetName(), m_FilePath);

	m_pParentObject->GetComponent<RenderComponent>()->EnableRenderComponent();
}
void ModelComponent::PostInitialize(Scene* /*pParentScene*/)
{ 
	if (!m_pMesh)
		return;

	m_pMesh->PostInitialize(m_pMaterial); 
}
void ModelComponent::Update(const float /*elapsedSec*/)
{ 
	if (!m_pMaterial)
		return; 
	
	m_pMaterial->Update(m_pParentObject); 
}
void ModelComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}
void ModelComponent::Render()
{
	if (!m_pMesh || !m_pMaterial)
		return;

	RenderComponent* pRenderComponent{ m_pParentObject->GetComponent<RenderComponent>() };
	pRenderComponent->Render3DMesh(Locator::GetGraphicsService()->GetDeviceContext(), m_pMesh, m_pMaterial);
}

void ModelComponent::AddMaterial(Material* pMaterial)
{
	m_pMaterial = Locator::GetResourceManagerService()->LoadMaterial(pMaterial);
	m_pMaterial->Initialize(Locator::GetGraphicsService()->GetDevice());
}

//void ModelComponent::Serialize(YAML::Emitter& out)
//{
//	out << YAML::Key << "ModelComponent";
//	out << YAML::BeginMap;
//
//	out << YAML::Key << "Mesh" << YAML::Value;
//	out << YAML::BeginMap;
//	out << YAML::Key << "IsInitialized" << YAML::Value << m_pMesh->GetIsInitialized();
//	out << YAML::Key << "IsRendered" << YAML::Value << m_pMesh->GetIsRendered();
//
//	out << YAML::Key << "FilePath" << YAML::Value << m_pMesh->GetFilePath();
//
//	out << YAML::Key << "AmountIndices" << YAML::Value << m_pMesh->GetAmountIndices();
//	out << YAML::Key << "AmountVertices" << YAML::Value << m_pMesh->GetAmountVertices();
//	out << YAML::Key << "AmountInstances" << YAML::Value << m_pMesh->GetAmountInstances();
//
//	SerializeID3D11Buffer(out, "IBuffer", m_pMesh->GetIBuffer());
//	SerializeID3D11Buffer(out, "VBuffer", m_pMesh->GetVBuffer());
//	SerializeID3D11Buffer(out, "InstanceBuffer", m_pMesh->GetInstanceBuffer());
//
//	out << YAML::Key << "ID3D11InputLayout" << YAML::Value;
//	out << YAML::BeginMap;
//	out << YAML::EndMap;
//
//	out << YAML::EndMap;
//
//	out << YAML::Key << "Material" << YAML::Value;
//	out << YAML::BeginMap;
//	out << YAML::EndMap;
//
//	out << YAML::Key << "FilePath" << YAML::Value << m_FilePath;
//
//	out << YAML::Key << "Textures" << YAML::Value;
//	out << YAML::BeginMap;
//	out << YAML::EndMap;
//
//	out << YAML::EndMap;
//}
//
//void ModelComponent::SerializeID3D11Buffer(YAML::Emitter& out, const std::string& bufferName, ID3D11Buffer* pBuffer)
//{
//	// Reference: https://www.gamedev.net/forums/topic/608576-writing-a-d3dx11buffer-to-a-file/
//	out << YAML::Key << bufferName << YAML::Value;
//	out << YAML::BeginMap;
//
//	// Create a staging buffer with the same size
//	// 
//	// Copy to the staging buffer
//	// Map the staging buffer to get a pointer to the buffer data
//	// Write that data to a file using whatever file I / O classes / functions you're comfortable with
//
//	out << YAML::EndMap;
//}