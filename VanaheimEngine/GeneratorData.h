#pragma once
#include "Generator.h"

struct GeneratorData
{
	GeneratorData()
	{}
	GeneratorData(const std::string& name, Generator* pGenerator)
		: name(name)
		, pGenerator(pGenerator)
	{}
	~GeneratorData()
	{ DELETE_POINTER(pGenerator); }

	std::string name{};
	Generator* pGenerator{};
};