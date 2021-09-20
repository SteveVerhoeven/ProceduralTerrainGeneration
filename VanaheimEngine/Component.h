#pragma once

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

	protected:
		GameObject* m_pParentObject;

	private:

};