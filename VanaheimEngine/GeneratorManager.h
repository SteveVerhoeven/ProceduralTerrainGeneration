#pragma once

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
		std::vector<Generator*> m_pGenerators;
};

template<class T>
inline T* GeneratorManager::GetGenerator() const
{
	for (Generator* pGenerator : m_pGenerators)
	{
		T* pTGen = dynamic_cast<T*>(pGenerator);
		if (pTGen)
			return pTGen;
	}

	return nullptr;
}
