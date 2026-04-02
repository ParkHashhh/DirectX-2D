#include "Avenger.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Player.h"
#include "../Monster/Monster.h"
#include "../Interface/StateInterface.h"
#include "ShurikenEffect.h"
#include "Component/ProjectileMovementComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/Animation2DComponent.h"
#include "Asset/Texture/Texture.h"



CAvenger::CAvenger()
{
	SetClassType<CAvenger>();
}

CAvenger::CAvenger(const CAvenger& ref) :
	CGameObject(ref)
{
}

CAvenger::CAvenger(CAvenger&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CAvenger::~CAvenger()
{
}

void CAvenger::SetCollisionName(const std::string& Name)
{
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile(Name);
	}
}

void CAvenger::SetMoveEnable(bool Enable)
{
	mMoveEnable = Enable;

	auto	Movement = mMovement.lock();

	Movement->SetEnable(Enable);
}

void CAvenger::SetDistance(float Distance)
{
	mDistance = Distance;

	auto	Movement = mMovement.lock();

	Movement->SetRange(Distance);
}

void CAvenger::SetSpeed(float Speed)
{
	mSpeed = Speed;

	auto	Movement = mMovement.lock();

	Movement->SetSpeed(Speed);
}

void CAvenger::SetMoveDir(const FVector3& MoveDir)
{
	mMoveDirEnable = true;
	mMoveDir = MoveDir;

	auto	Movement = mMovement.lock();

	Movement->SetMoveDir(mMoveDir);
}


bool CAvenger::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Anim");
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(120.f, 70.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer(0);
		Mesh->SetPivot(-0.17f, 0.05f);
	}
	

	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->AddAnimation("Avenger");
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->SetLoop("Avenger", false);
		Anim->SetSymmetry("Avenger", true);
		Anim->SetPlayRate("Avenger", 2.f);

		Anim->AddNotify<CAvenger>("Avenger",
			"AttackNotify", 7, this, &CAvenger::AnimResume);
		Anim->SetFinishNotify<CAvenger>("Avenger", this, &CAvenger::AnimEnd);
	}
	mMovement = CreateComponent<CProjectileMovementComponent>("Movement");
	auto	Movement = mMovement.lock();

	if (Movement)
	{
		Movement->SetUpdateComponent(mMeshComponent);

		Movement->SetRangeFunction<CAvenger>(this,
			&CAvenger::MoveEndFunction);
		Movement->SetRange(400);
		Movement->SetSpeed(0);
	}
	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();
	if (Body)
	{
		Body->SetCollisionBeginFunction<CAvenger>(this, &CAvenger::CollisionBegin);

		Body->SetCollisionEndFunction<CAvenger>(this, &CAvenger::CollisionEnd);

		Body->SetCollisionProfile("PlayerAttack");
		Body->SetBoxSize(80.f, 80.f);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetInheritScale(false);
		Body->SetInheritRot(false);
	}

	return true;
}
void CAvenger::SetSymmetry(bool Symmetry)
{
	auto Anim = mAnimation2DComponent.lock();
	Anim->SetSymmetry("Avenger", Symmetry);
}

void CAvenger::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		if(mAnimEnd)
			Mesh->AddRelativeRotationZ(7.f);
	}
}

void CAvenger::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CAvenger* CAvenger::Clone()
{
	return new CAvenger(*this);
}

void CAvenger::MoveEndFunction()
{
	Destroy();
}

void CAvenger::CollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	auto Owner = Dest->GetOwner().lock();
	auto Monster = std::dynamic_pointer_cast<CMonster>(Owner);
	
	auto iter = mColliderList.begin();
	auto iterEnd = mColliderList.end();
	for (; iter != iterEnd; iter++)
	{
		if (*iter == Dest)
			return;
	}

	if (Monster)
	{
		Monster->Damage(mDamage);
		mColliderList.push_back(Dest);
	}
}

void CAvenger::CollisionEnd(CCollider* Dest)
{
}

void CAvenger::AnimEnd()
{
	auto	Movement = mMovement.lock();

	if (Movement)
	{
		Movement->SetSpeed(mSpeed);

	}
	auto	Body = mBody.lock();
	if (Body)
	{
		Body->SetDebugDraw(false);
		Body->SetEnable(true);
	}
	mAnimEnd = true;
	
}

void CAvenger::AnimResume()
{
	if (mOwner)
	{
		mOwner->ResumeAvengerAnim();
	}
}
