#include "Ballock.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "FireBall.h"

CBallock::CBallock()
{
	SetClassType<CBallock>();

}

CBallock::CBallock(const CBallock& ref)
	: CMonster(ref)
{
}

CBallock::CBallock(CBallock&& ref) noexcept
	: CMonster(std::move(ref))
{
}

CBallock::~CBallock()
{
}
bool CBallock::Init()
{
	if (!CMonster::Init())
		return false;
	return true;
}
void CBallock::SetMonsterData()
{
	mMeshComponent = FindComponent<CMeshComponent>("MonsterMesh");
	mBody = FindComponent<CColliderBox2D>("MonsterBody");

	mAnimation2DComponent = FindComponent<CAnimation2DComponent>("MonsterAnimation2D");
	mMovement = FindComponent<CObjectMovementComponent>("MonsterMovement");
	mStateComponent = FindComponent<CStateComponent>("MonsterState");
	mLine2D = FindComponent<CColliderLine2D>("MonsterLine2D");
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	auto Movement = mMovement.lock();
	auto Body = mBody.lock();
	//Slash,Throw
	mAttackAnimName = "BallockSlash";
	mIdleAnimName = "BallockMove";
	mThrowAnimName = "BallockThrow";
	SetName("Ballock");
	mHP = 500;
	mDetectRange = 400.f;
	mType = MonsterType::Ballock;
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetRelativeScale(200.f, 200.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetWorldPos(0, -400);
	}
	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetBoxSize(200.f,200.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);

	}
	
	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		Anim->AddAnimation(mThrowAnimName);

		Anim->ChangeAnimation(mIdleAnimName);
		Anim->AddNotify<CBallock>(mAttackAnimName,
			mAttackAnimName, 6, this, &CBallock::AttackNotify);
		Anim->AddNotify<CBallock>(mThrowAnimName,
			mThrowAnimName, 6, this, &CBallock::ThrowNotify);

		Anim->SetFinishNotify<CBallock>(mAttackAnimName,
			this, &CBallock::AttackFinish);

		Anim->SetFinishNotify<CBallock>(mThrowAnimName,
			this, &CBallock::AttackFinish);

		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, false);
		Anim->SetLoop(mThrowAnimName, false);
	}
	
	if (Movement)
	{
		Movement->SetSpeed(60.f);
	}
}

void CBallock::AttackNotify()
{
	
}

void CBallock::ThrowNotify()
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

void CBallock::OnHit()
{

}


void CBallock::AttackFinish()
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
CBallock* CBallock::Clone()
{
	return new CBallock(*this);
}