#include "pch.h"
#include "GeneratorManager.h"

#include "NoiseGenerator.h"
#include "TerrainGenerator.h"

#include "GeneratorData.h"

GeneratorManager::GeneratorManager()
	: m_pGenerators(std::vector<GeneratorData*>())
{
	NoiseGenerator* pNoiseGen{ new NoiseGenerator() };
	TerrainGenerator* pTerrainGen{ new TerrainGenerator(pNoiseGen) };

	GeneratorData* pGData0{ new GeneratorData() };
	pGData0->name = "NoiseGenerator";
	pGData0->pGenerator = pNoiseGen;
	m_pGenerators.push_back(pGData0);
	
	GeneratorData* pGData1{ new GeneratorData() };
	pGData1->name = "TerrainGenerator";
	pGData1->pGenerator = pTerrainGen;
	m_pGenerators.push_back(pGData1);
}
GeneratorManager::~GeneratorManager()
{
	for (GeneratorData* pGData : m_pGenerators)
		DELETE_POINTER(pGData);

	m_pGenerators.clear();
}