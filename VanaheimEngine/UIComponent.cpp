#include "pch.h"
#include "UIComponent.h"

#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

UIComponent::UIComponent()
{}

void UIComponent::Initialize(Scene* /*pParentScene*/)
{}
void UIComponent::PostInitialize(Scene* /*pParentScene*/)
{
}
void UIComponent::Update(const float /*elapsedSec*/)
{}
void UIComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}
void UIComponent::Render()
{
	//bool showDemo{ true };
	//ImGui::ShowDemoWindow(&showDemo);
}