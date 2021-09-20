#pragma once
#include "GeneratorData.h"

class Generator;
class GeneratorManager final
{
	public:
		GeneratorManager();
		~GeneratorManager();

		GeneratorManager(const GeneratorManager&) = delete;
		GeneratorManager(GeneratorManager&&) noexcept = delete;
		GeneratorManager& operator=(const GeneratorManager&) = delete;
		GeneratorManager& operator=(GeneratorManager&&) noexcept = delete;

		template <class T>
		T* GetGenerator() const;

	private:
		std::vector<GeneratorData*> m_pGenerators;
};

template<class T>
inline T* GeneratorManager::GetGenerator() const
{
	for (GeneratorData* pGData : m_pGenerators)
	{
		T* pTGen = dynamic_cast<T*>(pGData->pGenerator);
		if (pTGen)
			return pTGen;
	}

	return nullptr;
}
