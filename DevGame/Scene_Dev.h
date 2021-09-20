#pragma once
#include "Scene.h"
#include <GeneratorSettings.h>

class Scene_Dev final : public Scene
{
	public:
		Scene_Dev();
		virtual ~Scene_Dev();

		Scene_Dev(const Scene_Dev& other) = delete;
		Scene_Dev(Scene_Dev&& other) = delete;
		Scene_Dev& operator=(const Scene_Dev& other) = delete;
		Scene_Dev& operator=(Scene_Dev&& other) = delete;

		virtual void Initialize() override;
		virtual void PostInitialize() override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
				
	private:
		void CreateInputs();
		void CreateMainCamera();
};