#pragma once
//#include "Generator.h"
//#include "TerrainSettings.h"
//
//class KDTree;
//class Octree;
//class NoiseGenerator;
//class TerrainGenerator final : public Generator
//{
//	public:
//		TerrainGenerator(NoiseGenerator* pGen);
//		~TerrainGenerator();
//
//		TerrainGenerator(const TerrainGenerator&) = delete;
//		TerrainGenerator(TerrainGenerator&&) noexcept = delete;
//		TerrainGenerator& operator=(const TerrainGenerator&) = delete;
//		TerrainGenerator& operator=(TerrainGenerator&&) noexcept = delete;
//
//		void Initialize();
//
//		// Terrain
//		/** 2D plane terrain */
//		Mesh* CreateNormalTerrain();
//		/** Voxel terrain, all on CPU */
//		Mesh* CreateVoxelTerrain_CPU();
//		/** Voxel terrain, center points on CPU, cubes on GPU */
//		Mesh* CreateVoxelTerrain_GPU();
//		/** Normal 2D plane terrain but Octree created over it - Visually rendered */
//		Mesh* CreateNormalTerrain_OcTree(Scene* pScene, const bool visualizeDataStructure);
//		/** Normal 2D plane terrain but KDtree created over it - Not visually rendered */
//		Mesh* CreateNormalTerrain_KDTree(Scene* pScene);
//
//		// Generating
//		void GenerateColorMap(const std::vector<std::vector<float>>& noiseMap);
//
//	protected:
//
//	private:
//		// *************
//		// Variables
//		// *************
//		ProcGenSettings m_Settings;
//		NoiseGenerator* m_pNoiseGenerator;
//		std::vector<Terrain> m_TerrainRegions;
//		std::vector<Vertex_Input> m_Vertices;
//		std::vector<uint32_t> m_Indices;
//
//		// Data structures
//		Octree* m_pOctree;
//		KDTree* m_pKDTree;
//
//		// *************
//		// Functions
//		// *************
//		// General
//		void CreateVertices();
//		void CreateIndices();
//		void CreateTerrainRegions();
//
//		// DataStructures
//		void CreateOctree(const std::vector<std::vector<float>>& noiseMap, Scene* pScene, const bool visualizeDataStructure);
//		void CreateKDTree(const std::vector<std::vector<float>>& noiseMap, Scene* pScene);
//		
//		void FindArrayTimings();
//		void FindOctantTimings(const std::vector<std::vector<float>>& noiseMap);
//		void FindKDNodeTimings(const std::vector<std::vector<float>>& noiseMap);
//
//		// Voxels
//		void CreateVoxels();
//};