#include "StateComponent.h"
#include "Object/GameObject.h"

CStateComponent::CStateComponent()
{
	SetClassType<CStateComponent>();
}

CStateComponent::CStateComponent(const CStateComponent& ref) :
	CObjectComponent(ref)
{
}

CStateComponent::CStateComponent(CStateComponent&& ref)	noexcept :
	CObjectComponent(std::move(ref))
{
}

CStateComponent::~CStateComponent()
{
}




CStateComponent* CStateComponent::Clone()	const
{
	return new CStateComponent(*this);
}
