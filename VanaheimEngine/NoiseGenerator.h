#pragma once
#include "Generator.h"

class NoiseGenerator final : public Generator
{
	public:
		NoiseGenerator();
		virtual ~NoiseGenerator() = default;

		NoiseGenerator(const NoiseGenerator&) = delete;
		NoiseGenerator(NoiseGenerator&&) noexcept = delete;
		NoiseGenerator& operator=(const NoiseGenerator&) = delete;
		NoiseGenerator& operator=(NoiseGenerator&&) noexcept = delete;

		void Initialize();
		void PostInitialize();

		// General
		const std::vector<std::vector<float>>& GetNoiseMap() const { return m_NoiseMap; }

		// Generating
		const std::vector<std::vector<float>>& GenerateNoiseMap(const DirectX::XMFLOAT3 & personalOffset);

		NoiseGenSettings* GetNoiseGenSettings() { return &m_Settings; }

	protected:
		virtual void onNotify(ObserverEvent event) override;

	private:
		// *************
		// Variables
		// *************
		std::vector<std::vector<float>> m_NoiseMap;
		// Perlin Noise
		std::vector<int> m_Permutation;
		std::vector<int> m_HashTable;

		// Fractal Noise
		NoiseGenSettings m_Settings;

		// *************
		// Functions
		// *************
		// General
		bool Validate(DirectX::XMFLOAT2 & mapSize, float& scale);
		void CreateUIData();
		void CalculateNoises(const std::vector<DirectX::XMFLOAT3>&offsets);

		// Fractal Noise
		void GenerateOffsets(std::vector<DirectX::XMFLOAT3>&offsets, const DirectX::XMFLOAT3 & personalOffset);
		void RecalculateFractalNoises(const std::vector<DirectX::XMFLOAT3>&offsets);
		float GetFractalNoise(const float xCoord, const float yCoord, const float zCoord, const float noiseScale, const std::vector<DirectX::XMFLOAT3>&offsets);

		// Perlin Noise
		float GetPerlinNoise(const float xCoord, const float yCoord, const float zCoord);
		float Fade(const float t);
		int Increment(const int num);
		float Gradient(const bool useOriginalPerlinFunction, const int hash, const float x, const float y, const float z);
		float Lerp(const float a, const float b, const float x);

		// Billow noise
		float GetBillowNoise(const float fractalNoise);

		// Ridged noise
		float GetRidgedNoise(const float billowNoise);


};