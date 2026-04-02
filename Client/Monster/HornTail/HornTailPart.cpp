#include "HornTailPart.h"
#include "../../Component/StateComponent.h"

CHornTailPart::CHornTailPart()
{
}

CHornTailPart::CHornTailPart(const CHornTailPart& ref) :
	CMonster(ref)

{
}

CHornTailPart::CHornTailPart(CHornTailPart&& ref) noexcept :
	CMonster(std::move(ref))
{
}

CHornTailPart::~CHornTailPart()
{
}

bool CHornTailPart::Init()
{
	return true;
}
void CHornTailPart::SetHPMax(float HP)
{
	auto State = mStateComponent.lock();
	if (State)
	{
		State->SetHPMax(HP);
	}
}
void CHornTailPart::AddHP(float HP)
{
	auto State = mStateComponent.lock();
	if (State)
	{
		State->AddHP(HP);
	}

}


float CHornTailPart::GetHP()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		return State->GetHP();
	}
	return -1.f;
}
float CHornTailPart::GetHPMax()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		return State->GetHPMax();
	}
	return -1.f;
}