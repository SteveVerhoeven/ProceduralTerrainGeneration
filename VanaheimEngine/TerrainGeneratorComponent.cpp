#include "VanaheimPCH.h"
#include "TerrainGeneratorComponent.h"

#include "Material_ProcGen.h"
#include "Mesh.h"
#include "Timer.h"
#include "Octree.h"
#include "KDTree.h"
#include "Voxel.h"
#include "TerrainSettings.h"

// Timings
#define Total
#define Average

TerrainGeneratorComponent::TerrainGeneratorComponent()
						  : Component()
						  , m_pModelComponent()
						  , m_Seed(1)
						  , m_Octaves(5)
						  , m_Lacunarity(2.3f)
						  , m_Scale(32.24f)
						  , m_Persistence(0.448f)
						  , m_MapSize(100, 100)
{
	m_pNoiseGenerator = Locator::GetGeneratorManagerService()->GetGenerator<NoiseGenerator>();

	// Add Observer
	Locator::GetUIManagerService()->GetUI<InspectorUI>()->AddObserver(this);

	CreateTerrainRegions();
}

void TerrainGeneratorComponent::Initialize(Scene* /*pParentScene*/)
{
	m_pModelComponent = RequiresComponent<ModelComponent>();
}
void TerrainGeneratorComponent::PostInitialize(Scene* /*pParentScene*/)
{}
void TerrainGeneratorComponent::Update(const float /*elapsedSec*/)
{}
void TerrainGeneratorComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}

void TerrainGeneratorComponent::onNotify(ObserverEvent event)
{
	if (event == ObserverEvent::REBUILD_LANDSCAPE)
	{
		ModelComponent* pModelComponent = m_pParentObject->GetComponent<ModelComponent>();
		Mesh* pMesh{ pModelComponent->GetMesh() };
		if (pMesh)
		{
			// Generate up to date noise
			std::vector<std::vector<float>> noiseMap{};
			GenerateNoiseMap(noiseMap);
			GenerateColorMap(noiseMap);

			CreateVertices();
			CreateIndices();

			Material* pMaterial{ m_pModelComponent->GetMaterial() };
			pMesh->Initialize(m_Vertices, m_Indices);
			pMesh->PostInitialize(pMaterial);
		}	
	}
}
void TerrainGeneratorComponent::GenerateTerrain()
{
	std::vector<std::vector<float>> noiseMap{};
	GenerateNoiseMap(noiseMap);
	GenerateColorMap(noiseMap);

	CreateVertices();
	CreateIndices();

	Mesh* pMesh{ new Mesh() };
	pMesh->Initialize(m_Vertices, m_Indices);
	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");

	ResourceManager* pResourceManager{ Locator::GetResourceManagerService() };
	Texture* pNormalTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/noiseMap.bmp") };
	Texture* pColorTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/colorMap.bmp") };

	UIManager* pUIManager{ Locator::GetUIManagerService() };
	InspectorUI* pVanaheimUI{ pUIManager->GetUI<InspectorUI>() };
	if (pVanaheimUI)
	{
		pVanaheimUI->AddObserver(pNormalTexture);
		pVanaheimUI->AddObserver(pColorTexture);
	}

	Material_ProcGen* pMaterial = new Material_ProcGen();
	pMaterial->AddTexture(pNormalTexture);
	pMaterial->AddTexture(pColorTexture);

	ModelComponent* pModelComponent{ m_pParentObject->GetComponent<ModelComponent>() };
	pModelComponent->SetMesh(pMesh);
	pModelComponent->AddMaterial(pMaterial);

	pMesh->PostInitialize(pMaterial);

	m_pParentObject->GetComponent<RenderComponent>()->EnableRenderComponent();
}

void TerrainGeneratorComponent::GenerateNoiseMap(std::vector<std::vector<float>>& noiseMap)
{
	// Create a collection of offsets to offset the samples ;)
	std::vector<DirectX::XMFLOAT3> offsets{};
	GenerateOffsets(offsets, {});

	// Noise
	for (int y{}; y < m_MapSize.y; ++y)
	{
		std::vector<float> vector{};
		for (int x{}; x < m_MapSize.x; ++x)
		{			
			const float fractalNoise{ m_pNoiseGenerator->FractalNoise({(float)x, 1, (float)y}, m_Scale, offsets, (float)m_Octaves, m_Lacunarity, m_Persistence) };
			const float billowNoise{ m_pNoiseGenerator->BillowNoise(fractalNoise) };
			const float ridgedNoise{ m_pNoiseGenerator->RidgedNoise(billowNoise) };
			vector.push_back(ridgedNoise);
		}
		noiseMap.push_back(vector);
	}

	// Normalize the values back to [0,1]
	noiseMap = Normalize2DVector(noiseMap);

	// Texture
	ImageGenerator::GenerateImage(noiseMap, "./Resources/Textures/Landscape/noiseMap.bmp", m_MapSize);
}
void TerrainGeneratorComponent::GenerateColorMap(const std::vector<std::vector<float>>& noiseMap)
{
	std::vector<std::vector<DirectX::XMFLOAT3>> colorMap{};
	for (int y{}; y < m_MapSize.y; ++y)
	{
		std::vector<DirectX::XMFLOAT3> row{};
		for (int x{}; x < m_MapSize.x; ++x)
		{
			const float currentHeight{ noiseMap[(size_t)y][(size_t)x] };

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

	// Texture
	ImageGenerator::GenerateImage(colorMap, "./Resources/Textures/Landscape/colorMap.bmp", m_MapSize);
}

void TerrainGeneratorComponent::GenerateOffsets(std::vector<DirectX::XMFLOAT3>& offsets, const DirectX::XMFLOAT3& personalOffset)
{
	srand(m_Seed);
	for (size_t i{}; i < m_Octaves; ++i)
	{
		DirectX::XMFLOAT3 offset{};
		offset.x = (float)RandomInt(-100, 100) + personalOffset.x;
		offset.y = (float)RandomInt(-100, 100) + personalOffset.y;
		offset.z = (float)RandomInt(-100, 100) + personalOffset.z;
		offsets.push_back(offset);
	}
}

void TerrainGeneratorComponent::CreateVertices()
{
	m_Vertices.clear();

	const int width{ m_MapSize.x };
	const int depth{ m_MapSize.y };

	for (int z{}; z < depth; ++z)
	{
		for (int x{}; x < width; ++x)
		{
			Vertex_Input vertex{};
			vertex.Position = DirectX::XMFLOAT3((float)x, 0.f, -(float)z);
			vertex.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(depth)));
			m_Vertices.push_back(vertex);
		}
	}
}
void TerrainGeneratorComponent::CreateIndices()
{
	m_Indices.clear();

	uint32_t vertexIndex{};
	const int width{ (int)m_MapSize.x };
	const int height{ (int)m_MapSize.y };
	for (int z{}; z < height; ++z)
	{
		for (int x{}; x < width; ++x)
		{
			if (x < width - 1 && z < height - 1)
			{
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
			}
			vertexIndex++;
		}
	}
}
void TerrainGeneratorComponent::CreateTerrainRegions()
{
	m_TerrainRegions.push_back({ TerrainType::WATER_DEEP   , 0.2f, DirectX::Colors::DarkBlue });
	m_TerrainRegions.push_back({ TerrainType::WATER_SHALLOW, 0.4f, DirectX::Colors::Blue });
	m_TerrainRegions.push_back({ TerrainType::SAND		   , 0.45f, DirectX::Colors::LightGoldenrodYellow });
	m_TerrainRegions.push_back({ TerrainType::GRASS		   , 0.50f, DirectX::Colors::LightGreen });
	m_TerrainRegions.push_back({ TerrainType::GRASS_DENSE  , 0.6f, DirectX::Colors::DarkGreen });
	m_TerrainRegions.push_back({ TerrainType::ROCK		   , 0.85f, DirectX::Colors::Brown });
	m_TerrainRegions.push_back({ TerrainType::SNOW		   , 1.f , DirectX::Colors::White });
}