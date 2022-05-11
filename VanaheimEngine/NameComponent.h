#pragma once
#include "Component.h"
#include <string>

class Scene;
class NameComponent final : public Component
{
	public:
		NameComponent(const std::string& name);
		virtual ~NameComponent() = default;

		NameComponent(const NameComponent&) = delete;
		NameComponent(NameComponent&&) noexcept = delete;
		NameComponent& operator=(const NameComponent&) = delete;
		NameComponent& operator=(NameComponent&&) noexcept = delete;
				
		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }

		// Serialization
		//void Serialize(YAML::Emitter& out) override;

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		std::string m_Name;
};