#pragma once
#include "Scene.h"
#include <GeneratorSettings.h>

class Scene_ProcGen final : public Scene
{
	public:
		Scene_ProcGen();
		virtual ~Scene_ProcGen() override;

		Scene_ProcGen(const Scene_ProcGen& other) = delete;
		Scene_ProcGen(Scene_ProcGen&& other) = delete;
		Scene_ProcGen& operator=(const Scene_ProcGen& other) = delete;
		Scene_ProcGen& operator=(Scene_ProcGen&& other) = delete;

		virtual void Initialize() override;
		virtual void PostInitialize() override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;
		virtual void LateUpdate() override;
		virtual void Render() const override;

	private:
		NoiseGenSettings m_NoiseGenSettings;
		ProcGenSettings m_ProcGenSettings;

		void CreateInputs();
		void CreateMainCamera();
		void CreateLandscape();
};