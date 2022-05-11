#pragma once
#include <Scene.h>

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
		/** Function that creates the inputs for the scene camera */
		void CreateInputs();	// TODO: move to the base class

		///** Function that creates a landscape from a 2D plane */
		//void CreateLandscape_2DPlane();
		///** Function that creates a landscape with voxels on the CPU */
		//void CreateLandscape_VoxelsCPU();
		///** Function that creates a landscape with voxels on the GPU */
		//void CreateLandscape_VoxelsGPU();
		///** Function that creates a landscape from a 2D plane with the Octree data structure */
		//void CreateLandscape_2DPlaneOctree(const bool visualizeDataStructure);
		///** Function that creates a landscape from a 2D plane with the KDTree data structure */
		//void CreateLandscape_2DPlaneKDtree();
};