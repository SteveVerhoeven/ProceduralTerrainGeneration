#pragma once
#include "Generator.h"
#include <vector>

class NoiseGenerator final : public Generator
{
	public:
		NoiseGenerator();
		virtual ~NoiseGenerator() = default;

		NoiseGenerator(const NoiseGenerator&) = delete;
		NoiseGenerator(NoiseGenerator&&) noexcept = delete;
		NoiseGenerator& operator=(const NoiseGenerator&) = delete;
		NoiseGenerator& operator=(NoiseGenerator&&) noexcept = delete;

		// Noises
		float PerlinNoise(const float xCoord, const float yCoord, const float zCoord);
		float FractalNoise(const DirectX::XMFLOAT3 coords,
						   const float noiseScale,
						   const std::vector<DirectX::XMFLOAT3>& offsets,
						   const float octaves,
						   const float lacunarity,
						   const float persistence);
		float BillowNoise(const float fractalNoise);
		float RidgedNoise(const float billowNoise);
		
	protected:

	private:
		// *************
		// Variables
		// *************
		// Perlin Noise
		std::vector<int> m_Permutation;
		std::vector<int> m_HashTable;

		// *************
		// Functions
		// *************
		// General
		float GetFractalNoise(const DirectX::XMFLOAT3 coords, 
							  const float noiseScale, 
							  const std::vector<DirectX::XMFLOAT3>&offsets, 
							  const float octaves,
							  const float lacunarity,
							  const float persistence);
		float GetPerlinNoise(const float xCoord, const float yCoord, const float zCoord);
		float Fade(const float t);
		int Increment(const int num);
		float Gradient(const bool useOriginalPerlinFunction, const int hash, const float x, const float y, const float z);
		float Lerp(const float a, const float b, const float x);
		float GetBillowNoise(const float fractalNoise);
		float GetRidgedNoise(const float billowNoise);


};