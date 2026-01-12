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
	mBody = FindComponent<CColliderSphere2D>("MonsterBody");
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	Anim->SetUpdateComponent(Mesh);
	auto Movement = mMovement.lock();
	mAttackAnimName = "OgreAttack";
	mIdleAnimName = "OgreRun";
	mHP = 4;


	if (Anim)
	{
		Anim->AddAnimation(mIdleAnimName);
		Anim->AddAnimation(mAttackAnimName);
		Anim->ChangeAnimation(mIdleAnimName);

		Anim->AddNotify<COgre>(mAttackAnimName,
			mAttackAnimName, 7, this, &COgre::AttackNotify);
		Anim->SetFinishNotify<COgre>(mAttackAnimName,
			this, &COgre::AttackFinish);
		Anim->SetLoop(mIdleAnimName, true);
		Anim->SetLoop(mAttackAnimName, true);
	}
	if (Movement)
	{
		Movement->SetSpeed(30.f);
	}
}
void COgre::AttackNotify()
{
	auto	Line2D = mLine2D.lock();
	if (Line2D)
	{
		Line2D->SetWorldRotation(GetWorldRot());
		Line2D->SetWorldPos(GetWorldPos() + GetAxis(EAxis::Y) * 30.f);
		Line2D->SetEnable(true);
		Line2D->SetDebugDraw(true);
	}
}

void COgre::AttackFinish()
{
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