#pragma once
#include "VanaheimPCH.h"

enum class TerrainType
{
	SAND,
	WATER_DEEP,
	WATER_SHALLOW,
	ROCK,
	SNOW,
	GRASS,
	GRASS_DENSE
};

struct Terrain
{
	Terrain()
		: Terrain(TerrainType::WATER_SHALLOW, 0.4f, DirectX::Colors::Blue)
	{}
	Terrain(const TerrainType& terrainType, const float terrainHeight, const DirectX::XMVECTORF32& terrainColorVector)
	{
		type = terrainType;
		height = terrainHeight;
		DirectX::XMStoreFloat3(&color, terrainColorVector);
	}

	TerrainType type{};
	float height{};
	DirectX::XMFLOAT3 color{};
};