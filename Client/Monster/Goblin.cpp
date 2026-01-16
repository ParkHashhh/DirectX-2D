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
	mBody = FindComponent<CColliderBox2D>("MonsterBody");
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	auto Movement = mMovement.lock();
	mHP = 2;
	mType = MonsterType::Goblin;

	mIdleAnimName = "GoblinRun";
	mAttackAnimName = "GoblinAttack";
	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		Anim->ChangeAnimation(mIdleAnimName);
		Anim->AddNotify<CGoblin>(mAttackAnimName,
			mAttackAnimName, 1, this, &CGoblin::AttackNotify);
		Anim->SetFinishNotify<CGoblin>(mAttackAnimName,
			this, &CGoblin::AttackFinish);
		Anim->SetLoop(mIdleAnimName, true);
	}
	if (Movement)
	{
		Movement->SetSpeed(20.f);
	}
}
void CGoblin::AttackNotify()
{
	auto Target = mTargetObject.lock();
	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	FVector3 MyPos = GetWorldPos();
	FVector3 DirToTarget = TargetPos - MyPos;

	DirToTarget.Normalize();
	float Angle = MyPos.GetViewTargetAngle2D(TargetPos, EAxis::Y);
	TargetDir.Normalize();
	auto	Line2D = mLine2D.lock();
	if (Line2D)
	{
		Line2D->SetInheritRot(false);
		Line2D->SetLineDistance(0);
		Line2D->SetCollisionProfile("MonsterAttack");
		Line2D->SetWorldRotationZ(Angle);
		Line2D->SetEnable(true);
		Line2D->SetDebugDraw(true);
	}
}
void CGoblin::OnHit()
{
	auto	Line2D = mLine2D.lock();
	Line2D->SetEnable(false);
}

void CGoblin::AttackFinish()
{
	auto	Line2D = mLine2D.lock();
	if (Line2D)
	{
		Line2D->SetEnable(false);
		Line2D->SetDebugDraw(false);
		Line2D->ClearCollisionList();
	}
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation(mIdleAnimName);
	}
	mParring = false;
	mIsAttack = false;

}
CGoblin* CGoblin::Clone()
{
	return new CGoblin(*this);
}