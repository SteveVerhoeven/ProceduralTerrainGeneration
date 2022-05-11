#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>
#include "TerrainSettings.h"

class Scene;
class KDTree;
class Octree;
class NoiseGenerator;
class ModelComponent;
struct Vertex_Input;
class TerrainGeneratorComponent final : public Component, public Observer
{
	public:
		TerrainGeneratorComponent();
		virtual ~TerrainGeneratorComponent() = default;

		TerrainGeneratorComponent(const TerrainGeneratorComponent&) = delete;
		TerrainGeneratorComponent(TerrainGeneratorComponent&&) noexcept = delete;
		TerrainGeneratorComponent& operator=(const TerrainGeneratorComponent&) = delete;
		TerrainGeneratorComponent& operator=(TerrainGeneratorComponent&&) noexcept = delete;

		// Terrain
		void GenerateTerrain();

		void SetSeed(const size_t seed) { m_Seed = seed; }
		void SetOctaves(const size_t octaves) { m_Octaves = octaves; }
		void SetLacunarity(const float lacunarity) { m_Lacunarity = lacunarity; }
		void SetScale(const float scale) { m_Scale = scale; }
		void SetPersistence(const float persistence) { m_Persistence = persistence; }
		void SetMapsize(const DirectX::XMINT2& mapSize) { m_MapSize = mapSize; }

		size_t GetSeed() const { return m_Seed; }
		size_t GetOctaves() const { return m_Octaves; }
		float GetLacunarity() const { return m_Lacunarity; }
		float GetScale() const { return m_Scale; }
		float GetPersistence() const { return m_Persistence; }
		DirectX::XMINT2 GetMapsize() const { return m_MapSize; }

	protected:
		virtual void Initialize(Scene * pParentScene) override;
		virtual void PostInitialize(Scene * pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

		virtual void onNotify(ObserverEvent event) override;

	private:
		// *************
		// Variables
		// *************
		ModelComponent* m_pModelComponent;
		size_t m_Seed;
		size_t m_Octaves;						// The number of iterations that will be layered together
		float m_Lacunarity;						// The speed that the frequency grows per octave;		
		float m_Scale;
		float m_Persistence;					// The speed that the amplitude changes per octave e;	[0, 1]
		DirectX::XMINT2 m_MapSize;
		//std::vector<std::vector<float>> m_NoiseMap;

		NoiseGenerator* m_pNoiseGenerator;
		std::vector<Terrain> m_TerrainRegions;
		std::vector<Vertex_Input> m_Vertices;
		std::vector<uint32_t> m_Indices;

		// General
		void CreateVertices();
		void CreateIndices();
		void CreateTerrainRegions();
		
		// Generating
		void GenerateNoiseMap(std::vector<std::vector<float>>& noiseMap);
		void GenerateColorMap(const std::vector<std::vector<float>>& noiseMap);

		// Helpers
		void GenerateOffsets(std::vector<DirectX::XMFLOAT3>& offsets, const DirectX::XMFLOAT3& personalOffset);		
};