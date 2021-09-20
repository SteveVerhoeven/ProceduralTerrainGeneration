#include "pch.h"
#include "Scene.h"

#include "Material.h"
#include "GameObject.h"

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