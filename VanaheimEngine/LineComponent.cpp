#include "VanaheimPCH.h"
#include "LineComponent.h"

#include "Line.h"
#include "Material.h"
#include "Graphics.h"

LineComponent::LineComponent(Line* pLine)
				: Component()
				, m_pLine(pLine)
				, m_pMaterial(nullptr)
				, m_FilePath("")
{}
LineComponent::~LineComponent()
{
	//DELETE_POINTER(m_pLine);
}

void LineComponent::Initialize(Scene* /*pParentScene*/)
{
	//m_pLine = dynamic_cast<Line*>(Locator::GetResourceManagerService()->Load3DMesh(m_pLine, m_pParentObject));
	m_pParentObject->GetComponent<RenderComponent>()->EnableRenderComponent();
}
void LineComponent::PostInitialize(Scene* /*pParentScene*/)
{
	m_pLine->PostInitialize(m_pMaterial);
}
void LineComponent::Update(const float /*elapsedSec*/)
{
	m_pMaterial->Update(m_pParentObject);
}
void LineComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}
void LineComponent::Render()
{
	RenderComponent* pRenderComponent{ m_pParentObject->GetComponent<RenderComponent>() };
	pRenderComponent->RenderLine(Locator::GetGraphicsService()->GetDeviceContext(), m_pLine, m_pMaterial);
}

void LineComponent::AddMaterial(Material* pMaterial)
{
	m_pMaterial = Locator::GetResourceManagerService()->LoadMaterial(pMaterial);
	m_pMaterial->Initialize(Locator::GetGraphicsService()->GetDevice());
}