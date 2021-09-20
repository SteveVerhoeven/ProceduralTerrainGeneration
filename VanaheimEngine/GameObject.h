#pragma once
#include <string>
#include <vector>

class Scene;
class Component;
class TransformComponent;
class ModelComponent;
class RenderComponent;
class GameObject final
{
	public:
		GameObject(const DirectX::XMFLOAT3& position = { 0, 0, 0 }, 
				   const DirectX::XMFLOAT4& rotation = { 0, 0, 0, 0 }, 
				   const DirectX::XMFLOAT3& scale = { 1, 1, 1 });
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Initialize();
		void PostInitialize();
		void Update(const float elapsedSec);
		void FixedUpdate(const float timeEachUpdate);
		void Render() const;

		void SetName(const std::string& name) { m_Name = name; }
		std::string GetName() const { return m_Name; }

		void SetParentScene(Scene* const pParentScene) { m_pParentScene = pParentScene; }
		Scene* GetParentScene() const { return m_pParentScene; }

		void AddComponent(Component* pComponent);
		template <class T>
		T* GetComponent() const;
		template <class T>
		std::vector<T*> GetComponents() const;
		template <class T>
		bool HasComponent() const;

		TransformComponent* Check_AddComponent_TransformComponent(const DirectX::XMFLOAT3& position,
																  const DirectX::XMFLOAT4& rotation,
																  const DirectX::XMFLOAT3& scale);
		ModelComponent* Check_AddComponent_ModelComponent(const std::string& path);
		RenderComponent* Check_AddComponent_RenderComponent();

	protected:

	private:
		std::string m_Name;
		Scene* m_pParentScene;
		std::vector<Component*> m_pComponents;
};

template<class T>
inline T* GameObject::GetComponent() const
{
	for (Component* pComp : m_pComponents)
	{
		T* pTComponent = dynamic_cast<T*>(pComp);
		if (pTComponent)
			return pTComponent;
	}
	return nullptr;
}
template<class T>
inline std::vector<T*> GameObject::GetComponents() const
{
	std::vector<T*> pTComponents{};
	for (Component* pComp : m_pComponents)
	{
		T* pTComponent = dynamic_cast<T*>(pComp);
		if (pTComponent)
			pTComponents->push_back(pTComponent);
	}
	return pTComponents;
}
template<class T>
inline bool GameObject::HasComponent() const
{
	T* pComponent{ GetComponent<T>() };
	if (pComponent)
		return true;
	else
		return false;
}