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

		// Generating
		std::vector<std::vector<float>> GenerateNoiseMap(const DirectX::XMFLOAT3 & personalOffset);

		// Settings
		void EditSettings(NoiseGenSettings & settings);
		NoiseGenSettings GetNoiseGenSettings() const { return m_Settings; }

		// General
		std::vector<std::vector<float>> GetNoiseMap() const { return m_NoiseMap; }
		template<typename T>
		T GetValueByName(const std::string & name) const;

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

		// Fractal Noise
		void GenerateOffsets(std::vector<DirectX::XMFLOAT3>&offsets, const DirectX::XMFLOAT3 & personalOffset);
		void CalculateFractalNoises(const std::vector<DirectX::XMFLOAT3>&offsets);
		void RecalculateFractalNoises(const std::vector<DirectX::XMFLOAT3>&offsets);
		float GetFractalNoise(const float xCoord, const float yCoord, const float zCoord, const float noiseScale, const std::vector<DirectX::XMFLOAT3>&offsets);

		// Perlin Noise
		float GetPerlinNoise(const float xCoord, const float yCoord, const float zCoord);
		float Fade(const float t);
		int Increment(const int num);
		float Gradient(const bool useOriginalPerlinFunction, const int hash, const float x, const float y, const float z);
		float Lerp(const float a, const float b, const float x);
};

template<typename T>
inline T NoiseGenerator::GetValueByName(const std::string& name) const
{
	if (name == "Seed")
		return (T)m_Settings.seed;
	else if (name == "Octaves")
		return (T)m_Settings.octaves;
	else if (name == "Lacunarity")
		return (T)m_Settings.lacunarity;
	else if (name == "Persistence")
		return (T)m_Settings.persistence;
	else if (name == "Scale")
		return (T)m_Settings.scale;

	return T();
}