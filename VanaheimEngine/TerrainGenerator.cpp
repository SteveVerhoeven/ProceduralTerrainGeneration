#include "VanaheimPCH.h"
#include "TerrainGenerator.h"
//
//#include "ResourceManager.h"
//#include "GeneratorManager.h"
//#include "NoiseGenerator.h"
//
//#include "Mesh.h"
//#include "Timer.h"
//
//#include "Octree.h"
//#include "KDTree.h"
//#include "Voxel.h"
//
//// Timings
//#define Total
//#define Average
//
//TerrainGenerator::TerrainGenerator(NoiseGenerator* pGen)
//				 : Generator("TerrainGenerator")
//				 , m_Settings()
//				 , m_pNoiseGenerator(pGen)
//				 , m_TerrainRegions()
//				 , m_pKDTree()
//				 , m_pOctree()
//{
//	Initialize();
//}
//TerrainGenerator::~TerrainGenerator()
//{
//	DELETE_POINTER(m_pOctree);
//	DELETE_POINTER(m_pKDTree);
//}
//void TerrainGenerator::Initialize()
//{
//	CreateTerrainRegions();
//}
//
//Mesh* TerrainGenerator::CreateNormalTerrain()
//{
//	//const std::vector<std::vector<float>>& noiseMap = m_pNoiseGenerator->GenerateNoiseMap({ 0,0,0 });
//	//GenerateColorMap(noiseMap);
//
//	CreateVertices();
//	CreateIndices();
//
//	Mesh* pMesh{ new Mesh() };
//	pMesh->Initialize(m_Vertices, m_Indices);
//	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");
//
//	return pMesh;
//}
//Mesh* TerrainGenerator::CreateVoxelTerrain_CPU()
//{
//	//const std::vector<std::vector<float>>& noiseMap = m_pNoiseGenerator->GenerateNoiseMap({0,0,0});
//	//GenerateColorMap(noiseMap);
//
//	CreateVoxels();
//
//	Mesh* pMesh{ new Mesh() };
//	pMesh->Initialize(m_Vertices, m_Indices);
//	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");
//
//	return pMesh;
//}
//Mesh* TerrainGenerator::CreateVoxelTerrain_GPU()
//{
//	//const std::vector<std::vector<float>>& noiseMap = m_pNoiseGenerator->GenerateNoiseMap({ 0,0,0 });
//	//GenerateColorMap(noiseMap);
//
//	CreateVertices();
//	CreateIndices();
//
//	Mesh* pMesh{ new Mesh() };
//	pMesh->Initialize(m_Vertices, m_Indices);
//	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");
//
//	return pMesh;
//}
//Mesh* TerrainGenerator::CreateNormalTerrain_OcTree(Scene* /*pScene*/, const bool /*visualizeDataStructure*/)
//{
//	//const std::vector<std::vector<float>>& noiseMap = m_pNoiseGenerator->GenerateNoiseMap({ 0,0,0 });
//	//GenerateColorMap(noiseMap);
//
//	CreateVertices();
//	CreateIndices();
//
//	//CreateOctree(noiseMap, pScene, visualizeDataStructure);
//
//	Mesh* pMesh{ new Mesh() };
//	pMesh->Initialize(m_Vertices, m_Indices);
//	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");
//
//	return pMesh;
//}
//Mesh* TerrainGenerator::CreateNormalTerrain_KDTree(Scene* /*pScene*/)
//{
//	//const std::vector<std::vector<float>>& noiseMap = m_pNoiseGenerator->GenerateNoiseMap({ 0,0,0 });
//	//GenerateColorMap(noiseMap);
//
//	CreateVertices();
//	CreateIndices();
//
//	//CreateKDTree(noiseMap, pScene);
//
//	Mesh* pMesh{ new Mesh() };
//	pMesh->Initialize(m_Vertices, m_Indices);
//	Locator::GetResourceManagerService()->Store3DMesh(pMesh, "Landscape_1");
//
//	return pMesh;
//}
//
//void TerrainGenerator::CreateVertices()
//{
//	//const int width{ m_Settings.xRes };
//	//const int depth{ m_Settings.zRes };
//
//	for (int z{}; z < depth; ++z)
//	{
//		for (int x{}; x < width; ++x)
//		{
//			Vertex_Input vertex{};
//			vertex.Position = DirectX::XMFLOAT3((float)x, 0.f, -(float)z);
//			vertex.UV = DirectX::XMFLOAT2((float(x) / float(width)), (float(z) / float(depth)));
//			m_Vertices.push_back(vertex);
//		}
//	}
//
//	//FindArrayTimings();
//}
//void TerrainGenerator::CreateIndices()
//{
//	uint32_t vertexIndex{};
//	const int width{ m_Settings.xRes };
//	const int height{ m_Settings.zRes };
//	for (int z{}; z < height; ++z)
//	{
//		for (int x{}; x < width; ++x)
//		{
//			if (x < width - 1 && z < height - 1)
//			{
//				// i		 i + 1
//				// +---------+
//				// |         |
//				// |		 |
//				// |		 |
//				// +---------+
//				// i + w	 i + w + 1
//
//				m_Indices.push_back(vertexIndex);
//				m_Indices.push_back(vertexIndex + width + 1);
//				m_Indices.push_back(vertexIndex + width);
//
//				m_Indices.push_back(vertexIndex + width + 1);
//				m_Indices.push_back(vertexIndex);
//				m_Indices.push_back(vertexIndex + 1);
//			}
//			vertexIndex++;
//		}
//	}
//}
//void TerrainGenerator::CreateTerrainRegions()
//{
//	m_TerrainRegions.push_back({ TerrainType::WATER_DEEP   , 0.2f, DirectX::Colors::DarkBlue });
//	m_TerrainRegions.push_back({ TerrainType::WATER_SHALLOW, 0.4f, DirectX::Colors::Blue });
//	m_TerrainRegions.push_back({ TerrainType::SAND		   , 0.45f, DirectX::Colors::LightGoldenrodYellow });
//	m_TerrainRegions.push_back({ TerrainType::GRASS		   , 0.50f, DirectX::Colors::LightGreen });
//	m_TerrainRegions.push_back({ TerrainType::GRASS_DENSE  , 0.6f, DirectX::Colors::DarkGreen });
//	m_TerrainRegions.push_back({ TerrainType::ROCK		   , 0.85f, DirectX::Colors::Brown });
//	m_TerrainRegions.push_back({ TerrainType::SNOW		   , 1.f , DirectX::Colors::White });
//}
//
//void TerrainGenerator::CreateOctree(const std::vector<std::vector<float>>& noiseMap, Scene* pScene, const bool visualizeDataStructure)
//{
//	m_pOctree = new Octree(0, 50, (float)-m_Settings.zRes, (float)m_Settings.xRes, 0, 0);
//	const size_t size{ noiseMap.size() };
//	for (size_t z{ 0 }; z < size; ++z)
//		for (size_t x{ 0 }; x < size; ++x)
//		{
//			const float noiseHeight = powf(noiseMap[z][x], 3.9f) * 25.f;
//			m_pOctree->Insert({ float(x), noiseHeight, -float(z) });
//		}
//	
//	if (visualizeDataStructure)
//		m_pOctree->Visualize(pScene);
//
//	//FindOctantTimings(noiseMap);
//}
//void TerrainGenerator::CreateKDTree(const std::vector<std::vector<float>>& noiseMap, Scene* /*pScene*/)
//{
//	const size_t width{ (size_t)m_Settings.xRes };
//	const size_t widthHalf{ width / 2 };
//	const size_t depth{ (size_t)m_Settings.zRes };
//	const size_t depthHalf{ depth / 2 };
//
//	// Start at middle
//	m_pKDTree = new KDTree();
//	KDTreeNode* pRootNode{};
//	{
//		const float noiseHeight = powf(noiseMap[depthHalf][widthHalf], 3.9f) * 25.f;
//		const float pointValues[] = { float(widthHalf), noiseHeight, -float(depthHalf) };
//		m_pKDTree->SetRootNode(m_pKDTree->Insert(pointValues, 0));
//		pRootNode = m_pKDTree->GetRootNode();
//	}
//
//	/** Fill the tree without the middle */
//	for (size_t z{}; z < depth; ++z)
//	{
//		for (size_t x{}; x < width; ++x)
//		{
//			if (z == depthHalf && x == widthHalf)
//				continue;
//
//			const float noiseHeight = powf(noiseMap[z][x], 3.9f) * 25.f;
//			const float pointValues[] = { float(x), noiseHeight, -float(z) };
//			m_pKDTree->SetRootNode(m_pKDTree->Insert(pointValues, 0));
//		}
//	}
//	m_pKDTree->SetRootNode(pRootNode);
//
//	//FindKDNodeTimings(noiseMap);
//}
//void TerrainGenerator::CreateVoxels()
//{
//	const float width{ float(m_Settings.xRes) };
//	const float depth{ float(m_Settings.yRes) };
//	const float height{ float(m_Settings.zRes) };
//
//	/** Create Surface Voxels */
//	for (float z{}; z < height; ++z)
//	{
//		for (float x{}; x < width; ++x)
//		{
//			// Top layer
//			Voxel voxel1{ m_Vertices, m_Indices, { x, 0.f, -z }, { width, height } };
//			// Lower layer
//			Voxel voxel2{ m_Vertices, m_Indices, { x, -1.f, -z }, { width, height }, -depth };
//		}
//	}
//}
//
//void TerrainGenerator::FindArrayTimings()
//{
//	LOG(ErrorLevel::LOG_INFO, "----------------------------------------------!");
//	LOG(ErrorLevel::LOG_INFO, "Array timings!");
//
//	Timer* pTimer{ Locator::GetTimerService() };
//	long long timeMS{};
//	#ifdef Average
//	int pointsFound{};
//	#endif
//
//	const size_t randomNumbers{ size_t((m_Settings.xRes * m_Settings.zRes) / 2) };
//	#ifdef Total
//	pTimer->StartCodeTimer();
//	#endif
//	for (size_t i{}; i < randomNumbers; ++i)
//	{
//		const float randomXPos{ float(RandomInt(0, (m_Settings.xRes - 1))) };
//		const float randomYPos{ 0.f };
//		const float randomZPos{ -float(RandomInt(0, (m_Settings.zRes - 1))) };
//
//		#ifdef Average
//		pTimer->StartCodeTimer();
//		#endif
//		const size_t size{ m_Vertices.size() };
//		for (size_t j{}; j < size; ++j)
//		{
//			const Vertex_Input& vIn{ m_Vertices[j] };
//			if (vIn.Position.x == randomXPos)
//				if (vIn.Position.y == randomYPos)
//					if (vIn.Position.z == randomZPos)
//					{
//						#ifdef average
//						timeMS += pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//						++pointsFound;
//						#endif
//						break;
//						
//					}
//		}
//		
//	}
//	#ifdef Total
//	timeMS = pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//	LOG(ErrorLevel::LOG_INFO, "total time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS)) + "ms.");
//	#endif
//	#ifdef Average
//	LOG(ErrorLevel::LOG_INFO, "Average time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS) / randomNumbers) + "ms.");
//	LOG(ErrorLevel::LOG_INFO, "# points found of " + std::to_string(randomNumbers) + ": " + std::to_string(pointsFound) + ".");
//	#endif
//}
//void TerrainGenerator::FindOctantTimings(const std::vector<std::vector<float>>& noiseMap)
//{
//	LOG(ErrorLevel::LOG_INFO, "----------------------------------------------!");
//	LOG(ErrorLevel::LOG_INFO, "Octant timings!");
//	
//	Timer* pTimer{ Locator::GetTimerService() };
//	long long timeMS{};
//	#ifdef Average
//	int pointsFound{};
//	#endif
//
//	const size_t randomNumbers{ size_t((m_Settings.xRes * m_Settings.zRes) / 2) };
//	#ifdef Total
//	pTimer->StartCodeTimer();
//	#endif
//	for (size_t i{}; i < randomNumbers; ++i)
//	{
//		const int randomXPos{ RandomInt(0, (m_Settings.xRes - 1)) };
//		const int randomZPos{ RandomInt(0, -(m_Settings.zRes - 1)) };
//
//		const float height{ noiseMap[randomZPos][randomXPos] };
//		const float randomYPos = powf(height, 3.9f) * 25.f;
//
//		#ifdef Average
//		pTimer->StartCodeTimer();
//		
//		if (m_pOctree->Find({ float(randomXPos), randomYPos, -float(randomZPos) }))
//			++pointsFound;
//
//		timeMS += pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//		#endif
//
//		#ifndef Average
//		m_pOctree->Find({ float(randomXPos), randomYPos, -float(randomZPos) });
//		#endif // !average
//	}
//	#ifdef Total
//	timeMS = pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//	LOG(ErrorLevel::LOG_INFO, "total time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS)) + "ms.");
//	#endif
//	#ifdef Average
//	LOG(ErrorLevel::LOG_INFO, "Average time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS) / randomNumbers) + "ms.");
//	LOG(ErrorLevel::LOG_INFO, "# points found of " + std::to_string(randomNumbers) + ": " + std::to_string(pointsFound) + ".");
//	#endif
//}
//void TerrainGenerator::FindKDNodeTimings(const std::vector<std::vector<float>>& noiseMap)
//{
//	LOG(ErrorLevel::LOG_INFO, "----------------------------------------------!");
//	LOG(ErrorLevel::LOG_INFO, "KDtree timings!");
//
//	Timer* pTimer{ Locator::GetTimerService() };
//	long long timeMS{};
//	#ifdef Average
//	int pointsFound{};
//	#endif
//
//	const size_t randomNumbers{ size_t((m_Settings.xRes * m_Settings.zRes) / 2) };
//	
//	#ifdef Total
//	pTimer->StartCodeTimer();
//	#endif
//	for (size_t i{}; i < randomNumbers; ++i)
//	{
//		const int randomXPos{ RandomInt(0, m_Settings.xRes - 1) };
//		const int randomZPos{ RandomInt(0, m_Settings.zRes - 1) };
//
//		const float height{ noiseMap[randomZPos][randomXPos] };
//		const float randomYPos = powf(height, 3.9f) * 25.f;
//
//		float arr[DIMENSIONS]{ float(randomXPos), randomYPos, -float(randomZPos) };
//		m_pKDTree->FindNode(arr);
//
//		#ifdef Average
//		pTimer->StartCodeTimer();
//
//		if (m_pKDTree->FindNode(arr))
//			++pointsFound;
//
//		timeMS += pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//		#endif
//		#ifndef Average
//		m_pKDTree->FindNode(arr);
//		#endif // !average
//	}
//	#ifdef Total
//	timeMS = pTimer->EndCodeTimer(TimeSizeType::NANOSECONDS);
//	LOG(ErrorLevel::LOG_INFO, "total time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS)) + "ms.");
//	#endif
//	#ifdef Average
//	LOG(ErrorLevel::LOG_INFO, "Average time to find all " + std::to_string(randomNumbers) + " points: " + std::to_string(ConvertNanoSecondsToMilliSeconds(timeMS) / randomNumbers) + "ms.");
//	LOG(ErrorLevel::LOG_INFO, "# points found of " + std::to_string(randomNumbers) + ": " + std::to_string(pointsFound) + ".");
//	#endif
//}
//
//void TerrainGenerator::GenerateColorMap(const std::vector<std::vector<float>>& noiseMap)
//{
//	const DirectX::XMFLOAT2 mapSize{ m_Settings.noiseGenSettings.mapSize };
//	std::vector<std::vector<DirectX::XMFLOAT3>> colorMap{};
//	for (size_t y{}; y < mapSize.y; ++y)
//	{
//		std::vector<DirectX::XMFLOAT3> row{};
//		for (size_t x{}; x < mapSize.x; ++x)
//		{
//			const float currentHeight{ noiseMap[y][x] };
//
//			const size_t nbrOfRegions{ m_TerrainRegions.size() };
//			for (size_t r{}; r < nbrOfRegions; ++r)
//			{
//				if (currentHeight <= m_TerrainRegions[r].height)
//				{
//					row.push_back(m_TerrainRegions[r].color);
//					break;
//				}
//			}
//		}
//		colorMap.push_back(row);
//	}
//
//}