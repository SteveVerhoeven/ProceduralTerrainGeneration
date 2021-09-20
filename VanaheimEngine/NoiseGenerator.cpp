#include "pch.h"
#include "NoiseGenerator.h"

#include "MathHelper.h"
#include "GeneratorManager.h"

#include "UIManager.h"
#include "GeneratorUI.h"

NoiseGenerator::NoiseGenerator()
			   : Generator("NoiseGenerator")
			   , m_NoiseMap(std::vector<std::vector<float>>())
			   , m_HashTable(std::vector<int>())
			   , m_Permutation(std::vector<int>())
			   , m_Settings()
{
	Initialize();
}
void NoiseGenerator::Initialize()
{
	// Hash lookup table as defined by Ken Perlin.  This is a randomly
	// arranged array of all numbers from 0-255 inclusive.
	m_HashTable = { 151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	m_Permutation.resize(m_HashTable.size() * 2);
	const int doubleHashtableSize{ 512 };
	for (int i = 0; i < doubleHashtableSize; ++i)
		m_Permutation[i] = m_HashTable[i % 256];

	CreateUIData();
}

// Generating
std::vector<std::vector<float>> NoiseGenerator::GenerateNoiseMap(const DirectX::XMFLOAT3& personalOffset)
{
	// Check if the scale is lower then or equal to zero to prevent division by 0
	if (!Validate(m_Settings.mapSize, m_Settings.scale))
	{
		m_NoiseMap.clear();
		return m_NoiseMap;
	}

	// Create a collection of offsets to offset the samples ;)
	std::vector<DirectX::XMFLOAT3> offsets{};
	GenerateOffsets(offsets, personalOffset);

	// recalculate the noise
	if (m_NoiseMap.empty())
		CalculateFractalNoises(offsets);
	else
		RecalculateFractalNoises(offsets);

	// Normalize the values back to [0,1]
	m_NoiseMap = Normalize2DVector(m_NoiseMap);

	// Generate Image
	GenerateImage(m_NoiseMap, "./Resources/Textures/Landscape/noiseMap.bmp", m_Settings.mapSize);

	return m_NoiseMap;
}

// Settings
void NoiseGenerator::EditSettings(NoiseGenSettings& settings)
{
	m_Settings.seed = settings.seed;
	m_Settings.octaves = settings.octaves;
	m_Settings.lacunarity = settings.lacunarity;
	m_Settings.persistence = settings.persistence;
	m_Settings.mapSize = settings.mapSize;
	m_Settings.scale = settings.scale;
}

void NoiseGenerator::onNotify(ObserverEvent event)
{
	if (event == ObserverEvent::INCREASE_SEED)
		m_Settings.seed++;
	else if (event == ObserverEvent::DECREASE_SEED)
		m_Settings.seed--;
	else if (event == ObserverEvent::INCREASE_OCTAVES)
		m_Settings.octaves++;
	else if (event == ObserverEvent::DECREASE_OCTAVES)
		m_Settings.octaves--;
	else if (event == ObserverEvent::INCREASE_SCALE)
		m_Settings.scale += 20.f;
	else if (event == ObserverEvent::DECREASE_SCALE)
		m_Settings.scale -= 1.f;
	else if (event == ObserverEvent::INCREASE_LACUNARITY)
		m_Settings.lacunarity += 0.1f;
	else if (event == ObserverEvent::DECREASE_LACUNARITY)
		m_Settings.lacunarity -= 0.1f;
	else if (event == ObserverEvent::INCREASE_PERSISTENCE)
		m_Settings.persistence += 0.1f;
	else if (event == ObserverEvent::DECREASE_PERSISTENCE)
		m_Settings.persistence -= 0.1f;

	if (event == ObserverEvent::REBUILD_LANDSCAPE)
	{
		GenerateNoiseMap({ 0,0,0 });
		//Locator::GetGeneratorManagerService()->GetGenerator<TerrainGenerator>()->GenerateColorMap(m_NoiseMap);
	}
}

// General
bool NoiseGenerator::Validate(DirectX::XMFLOAT2& mapSize, float& scale)
{
	const float minScale{ 0.0001f };
	if (scale <= 0.f)
		scale = minScale;

	if (mapSize.x < 0 || mapSize.y < 0)
		return false;

	if (0 < mapSize.x && mapSize.x < 1.f)
		mapSize.x = 1.f;
	if (0 < mapSize.y && mapSize.y < 1.f)
		mapSize.y = 1.f;

	return true;
}
void NoiseGenerator::CreateUIData()
{
	// Inspector
	std::vector<ObserverEvent> events_scale{};
	events_scale.push_back(ObserverEvent::DECREASE_SCALE);
	events_scale.push_back(ObserverEvent::INCREASE_SCALE);

	std::vector<ObserverEvent> events_seed{};
	events_seed.push_back(ObserverEvent::DECREASE_SEED);
	events_seed.push_back(ObserverEvent::INCREASE_SEED);

	std::vector<ObserverEvent> events_octaves{};
	events_octaves.push_back(ObserverEvent::DECREASE_OCTAVES);
	events_octaves.push_back(ObserverEvent::INCREASE_OCTAVES);

	std::vector<ObserverEvent> events_lacunarity{};
	events_lacunarity.push_back(ObserverEvent::DECREASE_LACUNARITY);
	events_lacunarity.push_back(ObserverEvent::INCREASE_LACUNARITY);

	std::vector<ObserverEvent> events_persistence{};
	events_persistence.push_back(ObserverEvent::DECREASE_PERSISTENCE);
	events_persistence.push_back(ObserverEvent::INCREASE_PERSISTENCE);

	const GeneratorVariable iv_scale(GeneratorType::NOISE, UIButtonType::SLIDER_FLOAT, "Scale", events_scale, { 0, 100 });
	const GeneratorVariable iv_seed(GeneratorType::NOISE, UIButtonType::SLIDER_INT, "Seed", events_seed, { 0, 10 });
	const GeneratorVariable iv_octaves(GeneratorType::NOISE, UIButtonType::SLIDER_INT, "Octaves", events_octaves, { 1, 8 });
	const GeneratorVariable iv_lacunarity(GeneratorType::NOISE, UIButtonType::SLIDER_FLOAT, "Lacunarity", events_lacunarity, { 1, 10 });
	const GeneratorVariable iv_persistence(GeneratorType::NOISE, UIButtonType::SLIDER_FLOAT, "Persistence", events_persistence, { 0, 1 });

	UIManager* pUIManager{ Locator::GetUIManagerService() };
	GeneratorUI* pGenUI{ pUIManager->GetUI<GeneratorUI>() };
	pGenUI->StoreVariable(iv_scale);
	pGenUI->StoreVariable(iv_seed);
	pGenUI->StoreVariable(iv_octaves);
	pGenUI->StoreVariable(iv_lacunarity);
	pGenUI->StoreVariable(iv_persistence);
}

// Fractal Noise
void NoiseGenerator::GenerateOffsets(std::vector<DirectX::XMFLOAT3>& offsets, const DirectX::XMFLOAT3& personalOffset)
{
	srand(m_Settings.seed);
	for (size_t i{}; i < m_Settings.octaves; ++i)
	{
		DirectX::XMFLOAT3 offset{};
		offset.x = (float)RandomInt(-100, 100) + personalOffset.x;
		offset.y = (float)RandomInt(-100, 100) + personalOffset.y;
		offset.z = (float)RandomInt(-100, 100) + personalOffset.z;
		offsets.push_back(offset);
	}
}
void NoiseGenerator::CalculateFractalNoises(const std::vector<DirectX::XMFLOAT3>& offsets)
{
	for (size_t y{}; y < m_Settings.mapSize.y; ++y)
	{
		std::vector<float> vector{};
		for (size_t x{}; x < m_Settings.mapSize.x; ++x)
		{
			const float fractalNoise{ GetFractalNoise((float)x, 1, (float)y, m_Settings.scale, offsets) };
			vector.push_back(fractalNoise);
		}
		m_NoiseMap.push_back(vector);
	}
}
void NoiseGenerator::RecalculateFractalNoises(const std::vector<DirectX::XMFLOAT3>& offsets)
{
	for (size_t y{}; y < m_Settings.mapSize.y; ++y)
	{
		for (size_t x{}; x < m_Settings.mapSize.x; ++x)
		{
			m_NoiseMap[y][x] = GetFractalNoise((float)x, 1, (float)y, m_Settings.scale, offsets);
		}
	}
}
float NoiseGenerator::GetFractalNoise(const float xCoord,
									  const float yCoord,
									  const float zCoord,
									  const float noiseScale,
									  const std::vector<DirectX::XMFLOAT3>& /*offsets*/)
{
	float fractalNoise{};

	float frequency{ 1 };
	float amplitude{ 1 };

	// Calculate the half of the map so that the noiseScale zooms in on the center
	for (size_t i{}; i < m_Settings.octaves; ++i)
	{
		float sampleX{ xCoord / noiseScale * frequency };
		float sampleY{ yCoord / noiseScale * frequency };
		float sampleZ{ zCoord / noiseScale * frequency };

		// Range is [0,1]
		float perlinNoise{ GetPerlinNoise(sampleX, sampleY, sampleZ) };
		// Range is [-1,1]
		perlinNoise *= 2 - 1;

		fractalNoise += perlinNoise * amplitude;

		frequency *= m_Settings.lacunarity;
		amplitude *= m_Settings.persistence;
	}

	return fractalNoise;
}

// Perlin Noise
float NoiseGenerator::GetPerlinNoise(const float xCoord, const float yCoord, const float zCoord)
{
	// Reference: https://adrianb.io/2014/08/09/perlinnoise.html

	int xi = (int)xCoord & 255;       // Calculate the "unit cube" that the point asked will be located in
	int yi = (int)yCoord & 255;       // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
	int zi = (int)zCoord & 255;       // plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
	float xf = xCoord - (int)xCoord;
	float yf = yCoord - (int)yCoord;
	float zf = zCoord - (int)zCoord;

	float u = Fade(xf);
	float v = Fade(yf);
	float w = Fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = m_Permutation[m_Permutation[m_Permutation[xi] + yi] + zi];
	aba = m_Permutation[m_Permutation[m_Permutation[xi] + Increment(yi)] + zi];
	aab = m_Permutation[m_Permutation[m_Permutation[xi] + yi] + Increment(zi)];
	abb = m_Permutation[m_Permutation[m_Permutation[xi] + Increment(yi)] + Increment(zi)];
	baa = m_Permutation[m_Permutation[m_Permutation[Increment(xi)] + yi] + zi];
	bba = m_Permutation[m_Permutation[m_Permutation[Increment(xi)] + Increment(yi)] + zi];
	bab = m_Permutation[m_Permutation[m_Permutation[Increment(xi)] + yi] + Increment(zi)];
	bbb = m_Permutation[m_Permutation[m_Permutation[Increment(xi)] + Increment(yi)] + Increment(zi)];

	float x1, x2, y1, y2;
	x1 = Lerp(Gradient(true, aaa, xf, yf, zf),          // The gradient function calculates the dot product between a pseudorandom
		Gradient(true, baa, xf - 1, yf, zf),      // gradient vector and the vector from the input coordinate to the 8
		u);										// surrounding points in its unit cube.
	x2 = Lerp(Gradient(true, aba, xf, yf - 1, zf),      // This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
		Gradient(true, bba, xf - 1, yf - 1, zf),  // values we made earlier.
		u);
	y1 = Lerp(x1, x2, v);

	x1 = Lerp(Gradient(true, aab, xf, yf, zf - 1),
		Gradient(true, bab, xf - 1, yf, zf - 1),
		u);
	x2 = Lerp(Gradient(true, abb, xf, yf - 1, zf - 1),
		Gradient(true, bbb, xf - 1, yf - 1, zf - 1),
		u);
	y2 = Lerp(x1, x2, v);

	// Returns a value between 0 and 1.
	return (Lerp(y1, y2, w) + 1) / 2;					// For convenience we bind the result to 0 - 1 (theoretical min/max before is [-1, 1])
}
float NoiseGenerator::Fade(const float t)
{
	// Fade function as defined by Ken Perlin.  This eases coordinate values
	// so that they will ease towards integral values.  
	// This ends up smoothing the final output.
	// Function -> 6t^5 - 15t^4 + 10t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}
int NoiseGenerator::Increment(const int num)
{
	return num + 1;
}
float NoiseGenerator::Gradient(const bool useOriginalPerlinFunction, 
							   const int hash, 
							   const float x, 
							   const float y, 
							   const float z)
{
	float gradient{};
	if (useOriginalPerlinFunction)
	{
		int h = hash & 15;                                    // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
		float u = h < 8 /* 0b1000 */ ? x : y;                 // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

		float v;                                              // In Ken Perlin's original implementation this was another conditional operator (?:).  I
															  // expanded it for readability.

		if (h < 4 /* 0b0100 */)                               // If the first and second significant bits are 0 set v = y
			v = y;
		else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/) // If the first and second significant bits are 1 set v = x
			v = x;
		else                                                  // If the first and second significant bits are not equal (0/1, 1/0) set v = z
			v = z;

		gradient = ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
	}
	else
	{
		// Reference: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
		switch (hash & 0xF)
		{
			case 0x0: gradient = x + y;
			case 0x1: gradient = -x + y;
			case 0x2: gradient = x - y;
			case 0x3: gradient = -x - y;
			case 0x4: gradient = x + z;
			case 0x5: gradient = -x + z;
			case 0x6: gradient = x - z;
			case 0x7: gradient = -x - z;
			case 0x8: gradient = y + z;
			case 0x9: gradient = -y + z;
			case 0xA: gradient = y - z;
			case 0xB: gradient = -y - z;
			case 0xC: gradient = y + x;
			case 0xD: gradient = -y + z;
			case 0xE: gradient = y - x;
			case 0xF: gradient = -y - z;
			default: gradient = 0.f; // never happens
		}
	}

	return gradient;
}
float NoiseGenerator::Lerp(const float a, const float b, const float x)
{
	return a + x * (b - a);
}