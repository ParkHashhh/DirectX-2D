#include "Shuriken.h"
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
#include "Asset/Sound/Sound.h"

CShuriken::CShuriken()
{
	SetClassType<CShuriken>();
}

CShuriken::CShuriken(const CShuriken& ref)	:
	CGameObject(ref)
{
}

CShuriken::CShuriken(CShuriken&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CShuriken::~CShuriken()
{
}

void CShuriken::SetCollisionName(const std::string& Name)
{
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile(Name);
	}
}

void CShuriken::SetMoveEnable(bool Enable)
{
	mMoveEnable = Enable;

	auto	Movement = mMovement.lock();

	Movement->SetEnable(Enable);
}

void CShuriken::SetDistance(float Distance)
{
	mDistance = Distance;

	auto	Movement = mMovement.lock();

	Movement->SetRange(Distance);
}

void CShuriken::SetSpeed(float Speed)
{
	mSpeed = Speed;

	auto	Movement = mMovement.lock();

	Movement->SetSpeed(Speed);
}

void CShuriken::SetMoveDir(const FVector3& MoveDir)
{
	mMoveDirEnable = true;
	mMoveDir = MoveDir;

	auto	Movement = mMovement.lock();

	Movement->SetMoveDir(mMoveDir);
	float Radian = atan2f(mMoveDir.y, mMoveDir.x);

	float Degree = Radian * (180.f / 3.141592f);

	SetWorldRotation(0.f, 0.f, Degree);
}

void CShuriken::SetNearTarget(const std::string& Name, float Horizon)
{

	float SearchRange = 400.f;
	mMoveDir = FVector3::Zero;
	mMoveDir.x = Horizon;

	std::vector <std::weak_ptr<CCollider>> MonsterList;

	auto World = mWorld.lock();

	if (World)
	{
		auto Collision = World->GetCollision().lock();
		if (!Collision)
			return;
		auto List = Collision->GetColliderList();

		if (!List.empty())
		{
			auto iter = List.begin();
			auto iterEnd = List.end();
			for (; iter != iterEnd; iter++)
			{
				if (iter->expired())
					continue;

				if (iter->lock()->GetCollisionProfile()->Name == "Monster" && iter->lock()->GetEnable())
				{
					MonsterList.push_back(*iter);
				}
			}

		}

		// 몬스터리스트가 있을 경우에만 동작한다.
		if (!MonsterList.empty())
		{
			mMoveDirEnable = true;

			float	Dist = FLT_MAX;

			auto	iter = MonsterList.begin();
			auto	iterEnd = MonsterList.end();

			for (; iter != iterEnd; ++iter)
			{
				auto	Obj = (*iter).lock();
				FVector3	TargetDir = Obj->GetWorldPos() - GetWorldPos();
				if (mMoveDir.x > 0 && TargetDir.x < 0)
					continue;
				if (mMoveDir.x < 0 && TargetDir.x > 0)
					continue;
				float	TargetDist = TargetDir.Length();

				float MaxYRange = 130.f;
				if (abs(TargetDir.y) > MaxYRange)
					continue;
				if (TargetDist < Dist && TargetDist <= SearchRange)
				{
					Dist = TargetDist;
					mMoveDir = TargetDir;
					mMoveDir.Normalize();
				}
			}
		}
	}
	
}

bool CShuriken::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mMovement = CreateComponent<CProjectileMovementComponent>("Movement");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Anim");
	auto	Movement = mMovement.lock();
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	if (Movement)
	{

		Movement->SetMoveDir(GetAxis(EAxis::X));
		Movement->SetRangeFunction<CShuriken>(this,
			&CShuriken::MoveEndFunction);
		Movement->SetRange(mDistance);
		Movement->SetSpeed(mSpeed);
		Movement->SetUpdateComponent(mMeshComponent);
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(30.f, 9.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer(0);
	}
	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();


	if (Body)
	{
		Body->SetCollisionBeginFunction<CShuriken>(this,&CShuriken::CollisionBegin);
		Body->SetCollisionEndFunction<CShuriken>(this,&CShuriken::CollisionEnd);
		Body->SetCollisionProfile("PlayerAttack");
		Body->SetBoxSize(30.f, 9.f);
		Body->SetDebugDraw(false);
		Body->SetInheritScale(false);
		Body->SetInheritRot(true);
	}
	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->AddAnimation("Shuriken");
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->SetLoop("Shuriken", true);
		Anim->SetSymmetry("Shuriken", true);

	}
	return true;
}

void CShuriken::SetSymmetry(bool Symmetry)
{
	auto Anim = mAnimation2DComponent.lock();
	Anim->SetSymmetry("Shuriken", Symmetry);
}

void CShuriken::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CShuriken::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CShuriken* CShuriken::Clone()
{
	return new CShuriken(*this);
}

void CShuriken::MoveEndFunction()
{
	Destroy();
}

void CShuriken::CollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	auto Owner = Dest->GetOwner().lock();
	auto Monster = std::dynamic_pointer_cast<CMonster>(Owner);
	if (Monster)
	{
		auto World = mWorld.lock();
		if (World)
		{
			auto Effect = World->CreateGameObject<CShurikenEffect>("Effect").lock();
			int RandYOffset = rand() % 40 - 20;

			Effect->SetWorldPos(HitPoint.x,HitPoint.y + RandYOffset,0);
		}
		auto AssetMng = mWorld.lock()->GetWorldAssetManager().lock();
		AssetMng->SoundPlay("Hit");
		Monster->Damage(mDamage);
	}


	Destroy();


}	

void CShuriken::CollisionEnd(CCollider* Dest)
{
}
