#pragma once
namespace YAML
{
	class Emitter;
	namespace detail
	{
		struct iterator_value;
	}
}

class Mesh;
class Scene;
class Texture;
class Material;
class GameObject;
class NameComponent;
class TransformComponent;
class CameraComponent;
class RenderComponent;
class ModelComponent;
class SceneSerializer final
{
	public:
		SceneSerializer() = default;
		~SceneSerializer() = default;

		SceneSerializer(const SceneSerializer&) = delete;
		SceneSerializer(SceneSerializer&&) noexcept = delete;
		SceneSerializer& operator=(const SceneSerializer&) = delete;
		SceneSerializer& operator=(SceneSerializer&&) noexcept = delete;

		void Serialize(const std::string& filePath, const Scene* pScene);
		bool Deserialize(const std::string& filePath, Scene* pScene);

	protected:
	private:
		// --------------------
		// Serializing
		// --------------------
		void SerializeGameObject(YAML::Emitter& out, GameObject* pGameObject);
		void SerializeNameComponent(YAML::Emitter& out, NameComponent* pNameComponent);
		void SerializeTransformComponent(YAML::Emitter& out, TransformComponent* pTransformComponent);
		void SerializeCameraComponent(YAML::Emitter& out, CameraComponent* pCameraComponent);
		void SerializeRenderComponent(YAML::Emitter& out, RenderComponent* pRenderComponent);
		void SerializeModelComponent(YAML::Emitter& out, ModelComponent* pModelComponent);

		// Helpers for SerializeModelComponent
		void SerializeMesh(YAML::Emitter& out, ModelComponent* pModelComponent);
		void SerializeMaterial(YAML::Emitter& out, ModelComponent* pModelComponent);
		void SerializeTexture(YAML::Emitter& out, ModelComponent* pModelComponent);
		void SerializeID3D11Buffer_Vertices(YAML::Emitter& out, const std::string& bufferName, uint32_t typeAmount, Mesh* pMesh);
		void SerializeID3D11Buffer_Indicies(YAML::Emitter& out, const std::string& bufferName, uint32_t typeAmount, Mesh* pMesh);
		
		// --------------------
		// Deserializing
		// --------------------
		void DeserializeNameComponent(const YAML::detail::iterator_value& yamlGO, GameObject* pGO);
		void DeserializeTransformComponent(const YAML::detail::iterator_value& yamlGO, GameObject* pGO);
		void DeserializeCameraComponent(const YAML::detail::iterator_value& yamlGO, GameObject* pGO);
		void DeserializeRenderComponent(const YAML::detail::iterator_value& yamlGO, GameObject* pGO);
		void DeserializeModelComponent(const YAML::detail::iterator_value& yamlGO, GameObject* pGO);

		Material* CreateMaterial(const std::string& name, const std::vector<Texture*> pTextures);

		// --------------------
		// Templates
		// --------------------
		template <class T>
		const std::vector<T> ReadBufferData(ID3D11Buffer* pBuffer, size_t bufferSize, size_t vectorSize);
		template <class T>
		T* CastMaterial(Material* pMaterial);
};

template<class T>
inline const std::vector<T> SceneSerializer::ReadBufferData(ID3D11Buffer* pBuffer, size_t bufferSize, size_t vectorSize)
{
	HRESULT hr{};	

	// Create base vector
	std::vector<T> dataVector{};
	dataVector.resize(vectorSize);

	// Get Device, DeviceContext
	ID3D11Device* pDevice{ Locator::GetGraphicsService()->GetDevice() };
	ID3D11DeviceContext* pDeviceContext{ Locator::GetGraphicsService()->GetDeviceContext() };

	// Call ID3D11Buffer::GetDesc to get size and other parameters of the buffer you wanna read.
	D3D11_BUFFER_DESC desc;
	pBuffer->GetDesc(&desc);

	// Adjust the description of the buffer, set usage to D3D11_USAGE_STAGING, bind flags to zero, and CPU access flags to D3D11_CPU_ACCESS_READ.
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	// Create another temporary buffer using the modified description, by calling ID3D11Device::CreateBuffer. Don’t specify initial data, pass nullptr there.
	ID3D11Buffer* pTempBuffer{ nullptr };
	hr = pDevice->CreateBuffer(&desc, nullptr, &pTempBuffer);
	LOG_HRESULT(hr, "SceneSerializer::SerializeID3D11Buffer", __FILE__, std::to_string(__LINE__));

	// Copy the source buffer into the newly created temporary one, by calling ID3D11DeviceContext::CopyResource
	pDeviceContext->CopyResource(pTempBuffer, pBuffer);

	// Now you can use ID3D11DeviceContext::Map/Unmap to read vertex data from the temporary buffer you created on step 4.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pDeviceContext->Map(pTempBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	memcpy(dataVector.data(), mappedResource.pData, bufferSize);
	pDeviceContext->Unmap(pTempBuffer, 0);

	// Release the local created buffer to prevent resource leaks
	DELETE_RESOURCE(pTempBuffer);

	return dataVector;
}

template<class T>
inline T* SceneSerializer::CastMaterial(Material* pMaterial)
{
	T* pMaterialCasted{ dynamic_cast<T*>(pMaterial) };
	if (pMaterialCasted == nullptr)
	{
		const std::string name{ typeid(T).name() };
		const std::string message{ "Material could not get casted to " + name };
		LOG(ErrorLevel::LOG_ERROR, message);
		return nullptr;
	}

	return pMaterialCasted;
}
