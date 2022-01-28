#include "pch.h"
#include "Scene.h"

#include "Material.h"
//#include "Material_Basic.h"
#include "Material_GPUInstance.h"
#include "GameObject.h"

#include "Line.h"

#include "ResourceManager.h"

Scene::Scene()
	  : m_IsActive(false)
	  , m_Name("")
	  , m_pGameObjects(std::vector<GameObject*>())
{}
Scene::~Scene()
{
	for (GameObject* pGameObject : m_pGameObjects)
		DELETE_POINTER(pGameObject);

	m_pGameObjects.clear();
}

void Scene::Initialize()
{
	for (GameObject* pObject : m_pGameObjects)
		pObject->Initialize();
}
void Scene::PostInitialize()
{
	for (GameObject* pObject : m_pGameObjects)
		pObject->PostInitialize();
}
void Scene::Update(const float elapsedSec)
{
	if (!m_IsActive)
		return;

	for (GameObject* pObject : m_pGameObjects)
		pObject->Update(elapsedSec);

	Locator::GetResourceManagerService()->ResetInstancedMeshes();
}
void Scene::FixedUpdate(const float timeEachUpdate)
{
	if (!m_IsActive)
		return;

	for (GameObject* pObject : m_pGameObjects)
		pObject->FixedUpdate(timeEachUpdate);
}
void Scene::LateUpdate()
{}
void Scene::Render() const
{
	if (!m_IsActive)
		return;

	for (GameObject* pObject : m_pGameObjects)
		pObject->Render();
}

void Scene::AddGameObject(GameObject* pObject)
{
	pObject->SetParentScene(this);

	// Find duplicates
	auto comp = pObject->GetComponent<LineComponent>();
	if (comp)
	{
		auto mesh = comp->GetMesh();
		if (mesh)
		{
			if (!Locator::GetResourceManagerService()->Load3DMesh(dynamic_cast<Mesh_Base*>(mesh), pObject))
			{
				DELETE_POINTER(pObject);
				return;
			}
		}		
	}	

	m_pGameObjects.push_back(pObject);
}
GameObject* Scene::GetObjectByName(const std::string& name) const
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		if (pGameObject)
			if (pGameObject->GetName() == name)
				return pGameObject;
	}
	return nullptr;
}
void Scene::CreateCamera(const std::string& name, const DirectX::XMFLOAT3& position)
{
	// Game Object
	GameObject* pMesh{ new GameObject() };

	// Camera
	CameraComponent* pCameraComponent{ new CameraComponent() };

	// Adding to game object
	pMesh->AddComponent(pCameraComponent);

	// Add to scene
	AddGameObject(pMesh);
	pMesh->SetName(name);

	// Edit game object in scene
	pMesh->GetComponent<TransformComponent>()->Translate(position, false);
}
void Scene::Create3DObject(const std::string& name, const DirectX::XMFLOAT3& position, const std::string& meshPath, Material* pMaterial)
{
	// Game Object
	GameObject* pMeshGO{ new GameObject() };				

	// Model
	ModelComponent* pModelComponent{ new ModelComponent(meshPath) };
	pModelComponent->AddMaterial(pMaterial);

	// Adding to game object
	pMeshGO->AddComponent(pModelComponent);
	pMeshGO->SetName(name);

	// Add to scene
	AddGameObject(pMeshGO);

	// Edit game object in scene
	pMeshGO->GetComponent<TransformComponent>()->Translate(position, false);
}
void Scene::Create3DObject(const std::string& name, const DirectX::XMFLOAT3& position, Mesh* pMesh, Material* pMaterial)
{
	// Game Object
	GameObject* pMeshGO{ new GameObject() };

	// Model
	ModelComponent* pModelComponent{ new ModelComponent(pMesh) };
	pModelComponent->AddMaterial(pMaterial);

	// Adding to game object
	pMeshGO->AddComponent(pModelComponent);
	pMeshGO->SetName(name);

	// Add to scene
	AddGameObject(pMeshGO);

	// Edit game object in scene
	pMeshGO->GetComponent<TransformComponent>()->Translate(position, false);
}
void Scene::CreateLineObject(const std::string& name, const DirectX::XMFLOAT3& position, Line* pLine)
{
	// Game Object
	GameObject* pLineGO{ new GameObject() };

	// Model
	LineComponent* pLineComponent{ new LineComponent(pLine) };
	pLineComponent->AddMaterial(new Material_GPUInstance());

	// Adding to game object
	pLineGO->AddComponent(pLineComponent);
	pLineGO->SetName(name);
	
	// Edit game object in scene
	pLineGO->GetComponent<TransformComponent>()->Translate(position, false);

	// Add to scene
	AddGameObject(pLineGO);
}
void Scene::CreateUI()
{
	// Game Object
	GameObject* pUIGO{ new GameObject() };

	// Model
	UIComponent* pUIComponent{ new UIComponent() };

	// Adding to game object
	pUIGO->AddComponent(pUIComponent);
	pUIGO->SetName("UI");

	// Add to scene
	AddGameObject(pUIGO);
}