#include "Monster.h"
#include "Component/ColliderBox2D.h"
#include "../Component/StateComponent.h"
#include "DamageObject.h"
#include "World/World.h"

CMonster::CMonster()
{
	SetClassType<CMonster>();
}

CMonster::CMonster(const CMonster& ref) :
	CGameObject(ref)
{

}

CMonster::CMonster(CMonster&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CGameObject::Init();
	auto	World = mWorld.lock();
	if (World)
	{
		mTargetObject = World->FindObject<CGameObject>("Player");
	}
	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	auto Body = mBody.lock();
	if (Body)
	{
		Body->ClearCollisionList();
	}

}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}


void CMonster::AttackNotify()
{

}

void CMonster::AttackFinish()
{
}

void CMonster::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	if (Dest->GetCollisionProfile()->Name == "NearAttack")	
		return;
}

void CMonster::Damage(float Damage)
{
	auto State = mStateComponent.lock();
	
	if (State)
	{
		State->AddHP(-Damage);
	}
	
}