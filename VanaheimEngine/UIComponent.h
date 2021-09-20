#pragma once
#include "Component.h"

class UIComponent final : public Component
{
	public:
		UIComponent();
		virtual ~UIComponent() = default;

		UIComponent(const UIComponent&) = delete;
		UIComponent(UIComponent&&) noexcept = delete;
		UIComponent& operator=(const UIComponent&) = delete;
		UIComponent& operator=(UIComponent&&) noexcept = delete;

		void Render();

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;
		
	private:

};