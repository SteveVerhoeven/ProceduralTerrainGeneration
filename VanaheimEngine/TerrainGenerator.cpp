#include "pch.h"
#include "TerrainGenerator.h"

#include "GeneratorManager.h"
#include "NoiseGenerator.h"

#include "Mesh.h"

#include "ResourceManager.h"

#include "UIManager.h"
#include "GeneratorUI.h"

TerrainGenerator::TerrainGenerator(NoiseGenerator* pGen)
				 : Generator("TerrainGenerator")
				 , m_Settings()
				 , m_pNoiseGenerator(pGen)
				 , m_TerrainRegions()
{
	Initialize();
}
void TerrainGenerator::Initialize()
{
	m_TerrainRegions.push_back({ TerrainType::WATER_DEEP   , 0.2f, DirectX::Colors::DarkBlue });
	m_TerrainRegions.push_back({ TerrainType::WATER_SHALLOW, 0.4f, DirectX::Colors::Blue });
	m_TerrainRegions.push_back({ TerrainType::SAND		   , 0.45f, DirectX::Colors::LightGoldenrodYellow });
	m_TerrainRegions.push_back({ TerrainType::GRASS		   , 0.50f, DirectX::Colors::LightGreen });
	m_TerrainRegions.push_back({ TerrainType::GRASS_DENSE  , 0.6f, DirectX::Colors::DarkGreen });
	m_TerrainRegions.push_back({ TerrainType::ROCK		   , 0.85f, DirectX::Colors::Brown });
	m_TerrainRegions.push_back({ TerrainType::SNOW		   , 1.f , DirectX::Colors::White });

	CreateUIData();
}

Mesh* TerrainGenerator::CreateTerrain()
{
	const std::vector<std::vector<float>> noiseMap = m_pNoiseGenerator->GenerateNoiseMap({0,0,0});
	GenerateColorMap(noiseMap);

	//CreateVertices();
	CreateVoxels();
	//CreateIndices();

	Mesh* pMesh{ new Mesh() };
	pMesh->Initialize(m_Vertices, m_Indices);

	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");

	return pMesh;
}
void TerrainGenerator::GenerateColorMap(const std::vector<std::vector<float>>& noiseMap)
{
	const DirectX::XMFLOAT2 mapSize{ m_Settings.noiseGenSettings.mapSize };
	std::vector<std::vector<DirectX::XMFLOAT3>> colorMap{};
	for (size_t y{}; y < mapSize.y; ++y)
	{
		std::vector<DirectX::XMFLOAT3> row{};
		for (size_t x{}; x < mapSize.x; ++x)
		{
			const float currentHeight{ noiseMap[y][x] };

			const size_t nbrOfRegions{ m_TerrainRegions.size() };
			for (size_t r{}; r < nbrOfRegions; ++r)
			{
				if (currentHeight <= m_TerrainRegions[r].height)
				{
					row.push_back(m_TerrainRegions[r].color);
					break;
				}
			}
		}
		colorMap.push_back(row);
	}

	GenerateImage(colorMap, "./Resources/Textures/Landscape/colorMap.bmp", DirectX::XMFLOAT2{ (float)m_Settings.xRes, (float)m_Settings.zRes });
}

void TerrainGenerator::onNotify(ObserverEvent event)
{
	if (event == ObserverEvent::REBUILD_LANDSCAPE)
		GenerateColorMap(m_pNoiseGenerator->GetNoiseMap());
}

void TerrainGenerator::EditSettings(const ProcGenSettings& settings)
{
	m_Settings.xRes = settings.xRes;
	m_Settings.zRes = settings.zRes;

	m_Settings.noiseGenSettings = settings.noiseGenSettings;

	m_pNoiseGenerator->EditSettings(m_Settings.noiseGenSettings);
}

void TerrainGenerator::CreateVertices()
{
	const int width{ m_Settings.xRes };
	const int height{ m_Settings.zRes };
	for (int z{}; z < height; ++z)
	{
		for (int x{}; x < width; ++x)
		{
			Vertex_Input vertex{};
			vertex.Position = DirectX::XMFLOAT3((float)x, 0.f, -(float)z);
			vertex.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));
			m_Vertices.push_back(vertex);
		}
	}
}
void TerrainGenerator::CreateIndices()
{
	uint32_t vertexIndex{};
	const int width{ m_Settings.xRes };
	const int height{ m_Settings.zRes };
	for (int z{}; z < height; ++z)
	{
		for (int x{}; x < width; ++x)
		{
			/*if (x < width - 1 && z < height - 1)
			{*/
				// i		 i + 1
				// +---------+
				// |         |
				// |		 |
				// |		 |
				// +---------+
				// i + w	 i + w + 1

				m_Indices.push_back(vertexIndex);
				m_Indices.push_back(vertexIndex + width + 1);
				m_Indices.push_back(vertexIndex + width);

				m_Indices.push_back(vertexIndex + width + 1);
				m_Indices.push_back(vertexIndex);
				m_Indices.push_back(vertexIndex + 1);
			/*}
			vertexIndex++;*/
		}
	}
}
void TerrainGenerator::CreateVoxels()
{
	const int width{ m_Settings.xRes };
	const int height{ m_Settings.zRes };
	for (int z{}; z < height; ++z)
	{
		for (int x{}; x < width; ++x)
		{
			// Side Front
			CreateFrontFace(x, z, width, height);
			// Side Left
			CreateLeftFace(x, z, width, height);
			// Side Right
			CreateRightFace(x, z, width, height);
			// Side Bottom
			CreateBottomFace(x, z, width, height);
			// Side Top
			CreateTopFace(x, z, width, height);
			// Side Back
			CreateBackFace(x, z, width, height);			
		}
	}
}

void TerrainGenerator::CreateVoxelIndices()
{
	// i		 i + 1
	// +---------+
	// |         |
	// |		 |
	// |		 |
	// +---------+
	// i + w	 i + w + 1

	m_Indices.push_back(m_Vertices.size() - 4);
	m_Indices.push_back(m_Vertices.size() - 1);
	m_Indices.push_back(m_Vertices.size() - 3);

	m_Indices.push_back(m_Vertices.size() - 1);
	m_Indices.push_back(m_Vertices.size() - 4);
	m_Indices.push_back(m_Vertices.size() - 2);
}

void TerrainGenerator::CreateFrontFace(const int x, const int z, const int width, const int height)
{
	const float size{ 0.5f };
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x - size, pos.y + size, pos.z - size );
	vLB.Position = DirectX::XMFLOAT3(pos.x - size, pos.y - size, pos.z - size );
	vRT.Position = DirectX::XMFLOAT3(pos.x + size, pos.y + size, pos.z - size );
	vRB.Position = DirectX::XMFLOAT3(pos.x + size, pos.y - size, pos.z - size );
	
	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));
	
	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}
void TerrainGenerator::CreateLeftFace(const int x, const int z, const int width, const int height)
{
	const float size{ 0.5f };
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x - size, pos.y + size, pos.z + size);
	vLB.Position = DirectX::XMFLOAT3(pos.x - size, pos.y - size, pos.z + size);
	vRT.Position = DirectX::XMFLOAT3(pos.x - size, pos.y + size, pos.z - size);
	vRB.Position = DirectX::XMFLOAT3(pos.x - size, pos.y - size, pos.z - size);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));

	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}
void TerrainGenerator::CreateRightFace(const int x, const int z, const int width, const int height)
{
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f);
	vLB.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f);
	vRT.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f);
	vRB.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));

	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}
void TerrainGenerator::CreateBottomFace(const int x, const int z, const int width, const int height)
{
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f);
	vLB.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f);
	vRT.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f);
	vRB.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));

	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}
void TerrainGenerator::CreateTopFace(const int x, const int z, const int width, const int height)
{
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f);
	vLB.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f);
	vRT.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f);
	vRB.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));

	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}
void TerrainGenerator::CreateBackFace(const int x, const int z, const int width, const int height)
{
	DirectX::XMFLOAT3 pos{ DirectX::XMFLOAT3((float)x, 0.f, -(float)z) };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f);
	vLB.Position = DirectX::XMFLOAT3(pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f);
	vRT.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f);
	vRB.Position = DirectX::XMFLOAT3(pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(height)));

	m_Vertices.push_back(vLT);
	m_Vertices.push_back(vLB);
	m_Vertices.push_back(vRT);
	m_Vertices.push_back(vRB);

	CreateVoxelIndices();
}




void TerrainGenerator::CreateUIData()
{
	//// Inspector
	//std::vector<ObserverEvent> events_xRes{};
	//events_xRes.push_back(ObserverEvent::DECREASE_XRES);
	//events_xRes.push_back(ObserverEvent::INCREASE_XRES);
	//events_xRes.push_back(ObserverEvent::INCREASE_XRES);

	//std::vector<ObserverEvent> events_zRes{};
	//events_zRes.push_back(ObserverEvent::DECREASE_ZRES);
	//events_zRes.push_back(ObserverEvent::INCREASE_ZRES);

	//const GeneratorVariable iv_xRes(GeneratorType::PROCEDURAL_GENERATION, UIButtonType::SLIDER_INT, "xRes", events_xRes, { 20, 1000 });
	//const GeneratorVariable iv_zRes(GeneratorType::PROCEDURAL_GENERATION, UIButtonType::SLIDER_INT, "zRes", events_zRes, { 20, 1000 });

	//UIManager* pUIManager{ Locator::GetUIManagerService() };
	//GeneratorUI* pGenUI{ pUIManager->GetUI<GeneratorUI>() };
	//pGenUI->StoreVariable(iv_xRes);
	//pGenUI->StoreVariable(iv_zRes);
}