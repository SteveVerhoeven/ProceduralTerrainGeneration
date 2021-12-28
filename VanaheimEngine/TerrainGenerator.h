#pragma once
#include "Generator.h"
#include "TerrainSettings.h"

class NoiseGenerator;
class TerrainGenerator final : public Generator
{
	public:
		TerrainGenerator(NoiseGenerator* pGen);
		virtual ~TerrainGenerator() = default;

		TerrainGenerator(const TerrainGenerator&) = delete;
		TerrainGenerator(TerrainGenerator&&) noexcept = delete;
		TerrainGenerator& operator=(const TerrainGenerator&) = delete;
		TerrainGenerator& operator=(TerrainGenerator&&) noexcept = delete;

		void Initialize();

		// Terrain
		Mesh* CreateTerrain();

		// Generating
		void GenerateColorMap(const std::vector<std::vector<float>>& noiseMap);

		// Settings
		void EditSettings(const ProcGenSettings& settings);
		ProcGenSettings& GetProcGenSettings() { return m_Settings; }

		// UI
		template<typename T>
		T GetValueByName(const std::string& name) const;

	protected:
		virtual void onNotify(ObserverEvent event) override;

	private:
		// *************
		// Variables
		// *************
		ProcGenSettings m_Settings;

		NoiseGenerator* m_pNoiseGenerator;
		std::vector<Terrain> m_TerrainRegions;
		std::vector<Vertex_Input> m_Vertices;
		std::vector<uint32_t> m_Indices;

		// *************
		// Functions
		// *************
		// General
		void CreateVertices();
		void CreateIndices();

		// UI
		void CreateUIData();
};

template<typename T>
inline T TerrainGenerator::GetValueByName(const std::string& name) const
{
	if (name == "xRes")
		return (T)m_Settings.xRes;
	else if (name == "zRes")
		return (T)m_Settings.zRes;

	return T();
}
