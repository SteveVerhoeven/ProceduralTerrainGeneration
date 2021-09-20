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

		Mesh* CreateTerrain();
		void GenerateColorMap(const std::vector<std::vector<float>>& noiseMap);

		void EditSettings(const ProcGenSettings& settings);
		ProcGenSettings GetProcGenSettings() const { return m_Settings; }

		template<typename T>
		T GetValueByName(const std::string& name) const;

	protected:
		virtual void onNotify(ObserverEvent event) override;

	private:
		ProcGenSettings m_Settings;

		NoiseGenerator* m_pNoiseGenerator;
		std::vector<Terrain> m_TerrainRegions;
		std::vector<Vertex_Input> m_Vertices;
		std::vector<uint32_t> m_Indices;

		void CreateVertices();
		void CreateIndices();

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
