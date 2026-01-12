#include "Goblin.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"


CGoblin::CGoblin()
{
	SetClassType<CGoblin>();
}

CGoblin::CGoblin(const CGoblin& ref)
	: CMonster(ref)
{
	mIdleAnimName = ref.mIdleAnimName;
	mAttackAnimName = ref.mAttackAnimName;
}


CGoblin::CGoblin(CGoblin&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CGoblin::~CGoblin()
{

}

bool CGoblin::Init()
{
	if (!CMonster::Init())
		return false;
	SetMonsterData();
	return true;
}

void CGoblin::SetMonsterData()
{
	mMeshComponent = FindComponent<CMeshComponent>("MonsterMesh");
	mAnimation2DComponent = FindComponent<CAnimation2DComponent>("MonsterAnimation2D");
	mMovement = FindComponent<CObjectMovementComponent>("MonsterMovement");
	mStateComponent = FindComponent<CStateComponent>("MonsterState");
	mLine2D = FindComponent<CColliderLine2D>("MonsterLine2D");
	mBody = FindComponent<CColliderSphere2D>("MonsterBody");
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	Anim->SetUpdateComponent(Mesh);
	auto Movement = mMovement.lock();
	mHP = 2;
	mIdleAnimName = "GoblinRun";
	mAttackAnimName = "GoblinAttack";
	if (Anim)
	{
		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		
		Anim->ChangeAnimation(mIdleAnimName);
		Anim->AddNotify<CGoblin>(mAttackAnimName,
			mAttackAnimName, 7, this, &CGoblin::AttackNotify);
		Anim->SetFinishNotify<CGoblin>(mAttackAnimName,
			this, &CGoblin::AttackFinish);
		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, true);
	}
	if (Movement)
	{
		Movement->SetSpeed(20.f);
	}
}
void CGoblin::AttackNotify()
{
	auto	Line2D = mLine2D.lock();
	if (Line2D)
	{
		Line2D->SetWorldRotation(GetWorldRot());
		Line2D->SetWorldPos(GetWorldPos() + GetAxis(EAxis::Y) * 50.f);
		Line2D->SetEnable(true);
		Line2D->SetDebugDraw(true);
	}
}

void CGoblin::AttackFinish()
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation(mIdleAnimName);
	}
	mIsAttack = false;

}
CGoblin* CGoblin::Clone()
{
	return new CGoblin(*this);
}