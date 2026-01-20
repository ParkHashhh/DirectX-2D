#include "Ogre.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"

COgre::COgre()
{
	SetClassType<COgre>();

}

COgre::COgre(const COgre& ref)
	: CMonster(ref)
{
}

COgre::COgre(COgre&& ref) noexcept
	: CMonster(std::move(ref))
{
}

COgre::~COgre()
{
}

bool COgre::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}



void COgre::SetMonsterData()
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
	SetSpeed(50.f);

	mAttackAnimName = "OgreAttack";
	mIdleAnimName = "OgreRun";
	mSturnAnimName = "OgreSturn";
	mHP = 4;
	mType = MonsterType::Ogre;

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);

		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		Anim->AddAnimation(mSturnAnimName);
		Anim->ChangeAnimation(mIdleAnimName);

		Anim->AddNotify<COgre>(mAttackAnimName,
			mAttackAnimName, 1, this, &COgre::AttackNotify);
		Anim->SetFinishNotify<COgre>(mAttackAnimName,
			this, &COgre::AttackFinish);
		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, true);
	}
	if (Movement)
	{
		Movement->SetSpeed(50.f);
	}
}
void COgre::AttackNotify()
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

void COgre::OnHit()
{

	mParring = false;
	auto Movement = mMovement.lock();
	Movement->SetSpeed(GetDefaultSpeed());
	mSturnTime += 1.f;
	if (Movement)
		Movement->SetSpeed(GetDefaultSpeed());

	mIsAttack = false;

	auto Anim = mAnimation2DComponent.lock();
	if (Anim) Anim->ChangeAnimation(mIdleAnimName);
}

void COgre::AttackFinish()
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
COgre* COgre::Clone()
{
	return new COgre(*this);
}