#pragma once
enum class FallOffType : int
{
	None = 0,
	Circular = 1,
	RoundedSquare = 2
};

struct NoiseGenSettings
{
	NoiseGenSettings()
		: NoiseGenSettings(21, 4, 32.24f, 2.3f, 1.f, { 400, 400 })
	{}
	NoiseGenSettings(const size_t seed,
		const size_t octaves,
		const float scale,
		const float lacunarity,
		const float persistence,
		const DirectX::XMFLOAT2& size)
		: seed(seed)
		, octaves((octaves < 0) ? 0 : octaves)
		, scale((scale < 0) ? 0 : scale)
		, lacunarity((lacunarity < 1) ? 1 : lacunarity)
		, persistence((persistence < 0) ? 0 : persistence)
		, mapSize(size)
	{}

	size_t seed;
	size_t octaves;			// The number of iterations that will be layered together
	float lacunarity;		// The speed that the frequency grows per octave;		
	float scale;
	float persistence;		// The speed that the amplitude changes per octave e;	[0, 1]
	DirectX::XMFLOAT2 mapSize;
};

struct ProcGenSettings
{
	ProcGenSettings()
		: ProcGenSettings(0, 0, NoiseGenSettings(0, 0, 0.f, 0.f, 0.f, {}))
	{}
	ProcGenSettings(const int xRes,
		const int zRes,
		const NoiseGenSettings& noiseSetting)
		: xRes(xRes)
		, zRes(zRes)
		, noiseGenSettings(noiseSetting)
	{}

	int xRes,
		zRes;
	NoiseGenSettings noiseGenSettings;
};