#pragma once
#include <Scene.h>
#include <GeneratorSettings.h>

class Scene_Game final : public Scene
{
	public:
		Scene_Game();
		virtual ~Scene_Game() override;

		Scene_Game(const Scene_Game& other) = delete;
		Scene_Game(Scene_Game&& other) = delete;
		Scene_Game& operator=(const Scene_Game& other) = delete;
		Scene_Game& operator=(Scene_Game&& other) = delete;

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
		void CreateLandscape_2DPlane();
		void CreateLandscape_VoxelsCPU();
		void CreateLandscape_VoxelsGPU();
		void CreateLandscape_2DPlaneOctree(const bool visualizeDataStructure);
		void CreateLandscape_2DPlaneKDtree();
};