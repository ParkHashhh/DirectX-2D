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
#include "BallockEffect.h"
#include "BressEffect.h"
#include "../Player/Player.h"
#include "Timer.h"
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
	mBressBody = CreateComponent<CColliderBox2D>("BressBody");
	return true;
}
void CBallock::SetMonsterData()
{
	auto World = mWorld.lock();
	mMeshComponent = FindComponent<CMeshComponent>("MonsterMesh");
	mBody = FindComponent<CColliderBox2D>("MonsterBody");
	mAnimation2DComponent = FindComponent<CAnimation2DComponent>("MonsterAnimation2D");
	mMovement = FindComponent<CObjectMovementComponent>("MonsterMovement");
	mStateComponent = FindComponent<CStateComponent>("MonsterState");
	mLine2D = FindComponent<CColliderLine2D>("MonsterLine2D");
	mBressBody = FindComponent<CColliderBox2D>("BressBody");

	auto BressBody = mBressBody.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	auto Movement = mMovement.lock();
	auto Body = mBody.lock();
	//Slash,Throw
	mAttackAnimName = "BallockSlash";
	mIdleAnimName = "BallockMove";
	mThrowAnimName = "BallockThrow";
	mSturnAnimName = "BallockSturn";
	SetName("Ballock");
	mHP = mHPMax;
	mDetectRange = 400.f;
	mType = MonsterType::Ballock;
	SetSpeed(120.f);
	if (BressBody)
	{
		Body->SetCollisionProfile("MonsterAttack");
		BressBody->SetEnable(true);
		BressBody->SetBoxSize(150.f, 300.f);
		Body->SetInheritScale(false);
		Body->SetDebugDraw(true);

	}
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
		Anim->AddAnimation(mSturnAnimName);
		Anim->AddAnimation("BallockSkill");
		Anim->AddAnimation("BallockDead");
		Anim->AddAnimation("BallockBress");
		Anim->SetPlayTime("BallockBress", 5.f);
		Anim->ChangeAnimation(mIdleAnimName);
		Anim->AddNotify<CBallock>("BallockBress",
			"BallockBress", 5, this, &CBallock::BressNotify);
		Anim->AddNotify<CBallock>(mAttackAnimName,
			mAttackAnimName, 6, this, &CBallock::AttackNotify);
		Anim->AddNotify<CBallock>("BallockSkill",
			"BallockSkill", 17, this, &CBallock::SkillNotify);
		Anim->SetPlayRate("BallockSkill", 0.5f);
		Anim->AddNotify<CBallock>(mThrowAnimName,
			mThrowAnimName, 6, this, &CBallock::ThrowNotify);
		Anim->SetFinishNotify<CBallock>(mAttackAnimName,
			this, &CBallock::AttackFinish);

		Anim->SetFinishNotify<CBallock>(mThrowAnimName,
			this, &CBallock::AttackFinish);

		Anim->SetFinishNotify<CBallock>("BallockSkill",
			this, &CBallock::AttackFinish);
		Anim->SetFinishNotify<CBallock>("BallockBress",
			this, &CBallock::AttackFinish);
		Anim->SetFinishNotify<CBallock>("BallockDead",
			this, &CBallock::DeadFinish);
		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, false);
		Anim->SetLoop(mThrowAnimName, false);
		Anim->SetLoop(mSturnAnimName, false);
		Anim->SetLoop("BallockSkill", false);
	}
	
	if (Movement)
	{
		Movement->SetSpeed(60.f);
	}
}

void CBallock::AttackNotify()
{
	auto Target = mTargetObject.lock();
	auto	Line2D = mLine2D.lock();

	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	FVector3 DirToTarget = TargetPos - GetWorldPos();

	DirToTarget.Normalize();
	float Angle = GetWorldPos().GetViewTargetAngle2D(TargetPos, EAxis::Y);
	TargetDir.Normalize();
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

void CBallock::SkillNotify()
{
	auto World = mWorld.lock();
	auto Effect = World->CreateGameObject<CBallockEffect>("BallockEffect").lock();
	auto Target = mTargetObject.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Player = mWorld.lock()->FindObject<CPlayer>("Player").lock();
	if (Target->GetWorldPos().x < GetWorldPos().x)
	{
		Effect->SetWorldPos(GetWorldPos().x - 100, GetWorldPos().y);
		Anim->SetSymmetry(mIdleAnimName, false);

	}
	else
	{
		Effect->SetWorldPos(GetWorldPos().x + 100, GetWorldPos().y);
		Anim->SetSymmetry(mIdleAnimName, true);
	}
	Player->SetDebuff(true);

}

void CBallock::BressNotify()
{
	auto World = mWorld.lock();
	auto Target = mTargetObject.lock();
	auto BressBody = mBressBody.lock();
	auto Movement = mMovement.lock();

	if (!Target || !BressBody || mBressEffect)
		return;
	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	TargetDir.Normalize();
	float Angle = GetWorldPos().GetViewTargetAngle2D(Target->GetWorldPos(), EAxis::Y);
	mBressEffect = World->CreateGameObject<CBressEffect>("BressEffect").lock();

	if (mBressEffect)
	{
		float Angle = GetWorldPos().GetViewTargetAngle2D(TargetPos, EAxis::Y);
		mBressEffect->SetWorldPos(GetWorldPos() + TargetDir * 300.f);
		mBressEffect->SetWorldRotationZ(Angle);

	}
	if (BressBody)
	{
		BressBody->SetWorldRotationZ(Angle);
		BressBody->SetWorldPos(GetWorldPos() + TargetDir * 100.f);
	}
	Movement->SetSpeed(0.f);

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
	mParring = false;
	auto Movement = mMovement.lock();
	Movement->SetSpeed(GetDefaultSpeed());
	mSturnTime += 1.f;
	if (Movement)
		Movement->SetSpeed(GetDefaultSpeed());

	mIsAttack = false;

	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
		Anim->ChangeAnimation(mIdleAnimName);
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
	auto BressBody = mBressBody.lock();
	if (BressBody)
	{
		BressBody->SetEnable(false);
		BressBody->ClearCollisionList();
	}
	if (mBressEffect)
	{
		mBressEffect->Destroy();
		mBressEffect = nullptr;
	}
	auto Mesh = mMeshComponent.lock();
	Mesh->SetRelativeScale(200.f, 200.f);
	Mesh->SetPivot(0, 0);

	mIsAttack = false;
	mIsBress = false;
}
CBallock* CBallock::Clone()
{
	return new CBallock(*this);
}
void CBallock::DeadFinish()
{
	Destroy();
}
void CBallock::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	mSkillTime -= DeltaTime;
	auto World = mWorld.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Movement = mMovement.lock();
	auto Target = mTargetObject.lock();
	auto Mesh = mMeshComponent.lock();
	auto BressBody = mBressBody.lock();

	if (mIsBress)
	{
		if (mBressEffect && Target && BressBody)
		{
			FVector3 TargetPos = Target->GetWorldPos();
			FVector3 TargetDir = TargetPos - GetWorldPos();
			TargetDir.Normalize();
			float Angle = GetWorldPos().GetViewTargetAngle2D(TargetPos, EAxis::Y);
			mBressEffect->SetWorldRotationZ(Angle);
			mBressEffect->SetWorldPos(GetWorldPos() + TargetDir * 200.f);
			BressBody->SetWorldRotationZ(Angle);
			BressBody->SetWorldPos(GetWorldPos() + TargetDir * 200.f);
		}

		return;
	}
	if (!mIsAttack || !mParring)
	{
		if (mHP % mBressPercent == 0 && mLastBressHP != mHP)
		{
			mIsBress = true;
			mIsAttack = true;
			mBressTime = 5.f;
			mLastBressHP = mHP;
			if (Target->GetWorldPos().x < GetWorldPos().x)
				Anim->SetSymmetry("BallockBress", false);
			else
				Anim->SetSymmetry("BallockBress", true);
			Anim->ChangeAnimation("BallockBress");
			auto Mesh = mMeshComponent.lock();
			Mesh->SetRelativeScale(500.f, 450.f);
			Mesh->SetPivot(0.05, 0);
			return;
		}
		else if (mSkillTime <= 0)
		{
			mSkillTime = 15.f;
			if (Target->GetWorldPos().x < GetWorldPos().x)
				Anim->SetSymmetry("BallockSkill", false);
			else
				Anim->SetSymmetry("BallockSkill", true);
			Anim->ChangeAnimation("BallockSkill");
			auto Mesh = mMeshComponent.lock();
			Mesh->SetRelativeScale(500.f, 450.f);
			Mesh->SetPivot(0.05, 0);

			mIsAttack = true;
			Movement->SetSpeed(0.f);
			return;
		}
	}

}


