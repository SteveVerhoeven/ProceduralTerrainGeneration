#include "VanaheimPCH.h"
#include "GeneratorManager.h"

GeneratorManager::GeneratorManager()
				 : m_pGenerators(std::vector<Generator*>())
{
	NoiseGenerator* pNoiseGen{ new NoiseGenerator() };
	m_pGenerators.push_back(pNoiseGen);
}
GeneratorManager::~GeneratorManager()
{
	for (Generator* pGenerator : m_pGenerators)
		DELETE_POINTER(pGenerator);

	m_pGenerators.clear();
}