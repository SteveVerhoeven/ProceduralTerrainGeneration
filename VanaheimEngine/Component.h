#pragma once

namespace YAML
{
	class Emitter;
}

class Scene;
class GameObject;
class Component
{
	public:
		Component();
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		virtual void Initialize(Scene* pParentScene) = 0;
		virtual void PostInitialize(Scene* pParentScene) = 0;
		virtual void Update(const float elapsedSec) = 0;
		virtual void FixedUpdate(const float timeEachUpdate) = 0;
			
		// Parent GameObject
		void SetParentObject(GameObject* pParentGameObject) { m_pParentObject = pParentGameObject; }
		GameObject* GetParentObject() const { return m_pParentObject; }

		// Serialization
		//virtual void Serialize(YAML::Emitter& out) = 0;
		//virtual void Deserialize(YAML::detail::iterator_value yamlGO) = 0;
			
	protected:
		GameObject* m_pParentObject;
		
		template <class T>
		T* RequiresComponent();

	private:

};

template<class T>
inline T* Component::RequiresComponent()
{
	T* pComponent = new T();
	m_pParentObject->AddComponent(pComponent);

	return pComponent;
}