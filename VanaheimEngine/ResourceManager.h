#pragma once
#include <vector>
#include <string>

#include "ResourceData.h"

class Mesh;
class Texture;
class Material;
class Mesh_Base;
class GameObject;
class ResourceManager final
{
	public:
		ResourceManager();
		~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		void Initialize();
		
		Mesh* Load3DMesh(const std::string& name, const std::string& path);
		Mesh_Base* Load3DMesh(Mesh_Base* pMesh, GameObject* pParentGO);
		template <class T>
		T* LoadMaterial(T* material);
		Texture* LoadTexture(const std::string& filePath);

		void Store3DMesh(Mesh* pMesh, const std::string& name);

		void ResetInstancedMeshes();

		std::vector<TextureData*> GetTextures() { return m_pTextures; }

	private:
		std::vector<MeshData*> m_p3DMeshes;
		std::vector<MaterialData*> m_p3DMaterials;
		std::vector<TextureData*> m_pTextures;

		int m_MeshFreeId;
		int m_MaterialFreeId;
		int m_TextureFreeId;

		bool MeshAlreadyParsed(const std::string& newName, int& meshID);
		bool MeshAlreadyLoaded(Mesh_Base* pMesh, int& meshID);
		bool MaterialAlreadyLoaded(const Material* material_in, int& materialID);
		bool TextureAlreadyLoaded(const std::string& filePath, int& textureID);

		int GetFreeMeshID() { return m_MeshFreeId++; }
		int GetFreeMaterialID() { return m_MaterialFreeId++; }
		int GetFreeTextureID() { return m_TextureFreeId++; }
};

template<class T>
inline T* ResourceManager::LoadMaterial(T* material)
{
	int materialID{ false };
	// Check if the Mesh exists already and return the id if it does
	if (MaterialAlreadyLoaded(material, materialID))
	{
		DELETE_POINTER(material);
		return static_cast<T*>(m_p3DMaterials[materialID]->pMaterial);
	}

	// Store the material
	MaterialData* pMaterialData{ new MaterialData() };
	pMaterialData->ID = GetFreeMaterialID();
	pMaterialData->name = material->GetName();
	pMaterialData->pMaterial = material;
	m_p3DMaterials.push_back(pMaterialData);

	// Return the pointer to the new material
	return material;
}