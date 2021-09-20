#pragma once
#include "ResourceData.h"

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

		Mesh* Get3DMesh(const std::string& name);
		
		Mesh* Load3DMesh(const std::string& name, const std::string& path);
		template <class T>
		T* LoadMaterial(T* material);
		Texture* LoadTexture(const std::string& filePath);

		void Store3DMesh(Mesh* pMesh, const std::string& name);

	private:
		std::vector<MeshData*> m_p3DMeshes;
		std::vector<MaterialData*> m_p3DMaterials;
		std::vector<TextureData*> m_pTextures;

		int m_MeshFreeId;
		int m_MaterialFreeId;
		int m_TextureFreeId;

		bool MeshAlreadyParsed(const std::string& newName, int& meshID);
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
		return static_cast<T*>(m_p3DMaterials[materialID]->pMaterial);

	// Store the mesh
	MaterialData* pMaterialData{ new MaterialData() };
	pMaterialData->ID = GetFreeMaterialID();
	pMaterialData->name = material->GetName();
	pMaterialData->pMaterial = material;
	m_p3DMaterials.push_back(pMaterialData);

	// Return the pointer to the new material
	return material;
}