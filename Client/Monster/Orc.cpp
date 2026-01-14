#include "Orc.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "FireBall.h"

COrc::COrc()
{
	SetClassType<COrc>();

}

COrc::COrc(const COrc& ref)
	: CMonster(ref)
{
}

COrc::COrc(COrc&& ref) noexcept
	: CMonster(std::move(ref))
{
}

COrc::~COrc()
{
}
bool COrc::Init()
{
	if (!CMonster::Init())
		return false;
	return true;
}
void COrc::SetMonsterData()
{
	mMeshComponent = FindComponent<CMeshComponent>("MonsterMesh");
	mAnimation2DComponent = FindComponent<CAnimation2DComponent>("MonsterAnimation2D");
	mMovement = FindComponent<CObjectMovementComponent>("MonsterMovement");
	mStateComponent = FindComponent<CStateComponent>("MonsterState");
	mLine2D = FindComponent<CColliderLine2D>("MonsterLine2D");
	mBody = FindComponent<CColliderBox2D>("MonsterBody");
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	Anim->SetUpdateComponent(Mesh);
	auto Movement = mMovement.lock();
	mAttackAnimName = "OrcAttack";
	mIdleAnimName = "OrcRun";
	mHP = 6;
	mDetectRange = 400.f;
	mType = MonsterType::Orc;

	if (Anim)
	{
		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		Anim->ChangeAnimation(mIdleAnimName);
		Anim->AddNotify<COrc>(mAttackAnimName,
			mAttackAnimName, 7, this, &COrc::AttackNotify);
		Anim->SetFinishNotify<COrc>(mAttackAnimName,
			this, &COrc::AttackFinish);
		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, true);
	}
	if (Movement)
	{
		Movement->SetSpeed(40.f);
	}
}

void COrc::AttackNotify()
{
	auto World = mWorld.lock();
	mFireBall = World->CreateGameObject<CFireBall>("FireBall");
	std::shared_ptr<CFireBall>	FireBall = mFireBall.lock();
	auto Target = mTargetObject.lock();
	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	TargetDir.Normalize();
	float Angle = GetWorldPos().GetViewTargetAngle2D(Target->GetWorldPos(), EAxis::Y);
	if (FireBall)
	{
		FireBall->SetWorldPos(GetWorldPos() + TargetDir * 10.f);
		FireBall->SetNearTarget("Player");
		FireBall->SetCollisionName("MonsterAttack");
		FireBall->SetMoveDir(TargetDir);
		FireBall->ComputeCollisionRange();
	}
}

void COrc::OnHit()
{

}


void COrc::AttackFinish()
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
	mIsAttack = false;
}
COrc* COrc::Clone()
{
	return new COrc(*this);
}