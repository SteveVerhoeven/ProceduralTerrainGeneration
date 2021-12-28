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
		: NoiseGenSettings(true, 1, 5, 32.24f, 2.3f, 0.448f, { 400, 400 })
	{}
	NoiseGenSettings(const bool addToInspector,
					 const size_t seed,
					 const size_t octaves,
					 const float scale,
					 const float lacunarity,
					 const float persistence,
					 const DirectX::XMFLOAT2& size)
					 : addToInspector(addToInspector)
					 , seed(seed)
					 , octaves((octaves < 0) ? 0 : octaves)
					 , scale((scale < 0) ? 0 : scale)
					 , lacunarity((lacunarity < 1) ? 1 : lacunarity)
					 , persistence((persistence < 0) ? 0 : persistence)
					 , mapSize(size)
	{}

	bool addToInspector;
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
		: ProcGenSettings(400, 400, NoiseGenSettings(true, 1, 5, 32.24f, 2.3f, 0.448f, { 400, 400 }))
	{}
	ProcGenSettings(const int xRes,
					const int zRes,
					const NoiseGenSettings& noiseSetting)
					: xRes(xRes)
					, zRes(zRes)
					, noiseGenSettings(noiseSetting)
	{}

	int xRes;
	int	zRes;
	NoiseGenSettings noiseGenSettings;
};