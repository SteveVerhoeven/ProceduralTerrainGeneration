#include "VanaheimPCH.h"
#include "GameObject.h"

#include "VanaheimEngine.h"
#include "Component.h"

GameObject::GameObject(const DirectX::XMFLOAT3& position, 
					   const DirectX::XMFLOAT4& rotation, 
					   const DirectX::XMFLOAT3& scale,
					   const std::string& name)
		   : m_ToBeRemoved(false)
		   , m_pParentScene(nullptr)
		   , m_pComponents(std::vector<Component*>())
{
	Check_AddComponent_TransformComponent(position, rotation, scale);
	Check_AddComponent_RenderComponent();
	Check_AddComponent_NameComponent(name);
}
GameObject::~GameObject()
{
	DELETE_POINTERS(m_pComponents, m_pComponents.size());
}

void GameObject::Initialize()
{
	for (Component* pComp : m_pComponents)
		pComp->Initialize(m_pParentScene);
}
void GameObject::PostInitialize()
{
	for (Component* pComp : m_pComponents)
		pComp->PostInitialize(m_pParentScene);
}
void GameObject::Update(const float elapsedSec)
{
	for (Component* pComp : m_pComponents)
		pComp->Update(elapsedSec);
}
void GameObject::FixedUpdate(const float timeEachUpdate)
{
	for (Component* pComp : m_pComponents)
		pComp->FixedUpdate(timeEachUpdate);
}
void GameObject::Render() const
{
	for (Component* pComp : m_pComponents)
	{
		ModelComponent* pModelComponent = dynamic_cast<ModelComponent*>(pComp);
		if (pModelComponent)
		{
			pModelComponent->Render();
			return;
		}

		LineComponent* pLineComponent = dynamic_cast<LineComponent*>(pComp);
		if (pLineComponent)
		{
			pLineComponent->Render();
			return;
		}
	}
}

void GameObject::AddComponent(Component* pComponent)
{
	pComponent->SetParentObject(this);
	m_pComponents.push_back(pComponent);
}

TransformComponent* GameObject::Check_AddComponent_TransformComponent(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& scale)
{
	if (!HasComponent<TransformComponent>())
	{
		TransformComponent* pTransformComponent{ new TransformComponent(position, rotation, scale) };
		AddComponent(pTransformComponent);

		return pTransformComponent;
	}

	return GetComponent<TransformComponent>();
}
ModelComponent* GameObject::Check_AddComponent_ModelComponent(const std::string& path)
{
	if (!HasComponent<ModelComponent>())
	{
		ModelComponent* pModelComponent{ new ModelComponent(path) };
		AddComponent(pModelComponent);

		return pModelComponent;
	}

	return GetComponent<ModelComponent>();
}
RenderComponent* GameObject::Check_AddComponent_RenderComponent()
{
	if (!HasComponent<RenderComponent>())
	{
		RenderComponent* pRenderComponent{ new RenderComponent() };
		AddComponent(pRenderComponent);

		return pRenderComponent;
	}

	return GetComponent<RenderComponent>();
}
NameComponent* GameObject::Check_AddComponent_NameComponent(const std::string& name)
{
	if (!HasComponent<NameComponent>())
	{
		NameComponent* pNameComponent{ new NameComponent(name) };
		AddComponent(pNameComponent);

		return pNameComponent;
	}

	return GetComponent<NameComponent>();
}
