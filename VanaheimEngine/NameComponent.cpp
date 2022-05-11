#include "VanaheimPCH.h"
#include "NameComponent.h"

NameComponent::NameComponent(const std::string& name)
			  : m_Name(name)
{}

void NameComponent::Initialize(Scene* /*pParentScene*/)
{}
void NameComponent::PostInitialize(Scene* /*pParentScene*/)
{}
void NameComponent::Update(const float /*elapsedSec*/)
{}
void NameComponent::FixedUpdate(const float /*timeEachUpdate*/)
{}

//void NameComponent::Serialize(YAML::Emitter& out)
//{
//	out << YAML::Key << "NameComponent";
//	out << YAML::BeginMap;
//	out << YAML::Key << "Name" << YAML::Value << m_Name;
//	out << YAML::EndMap;
//}