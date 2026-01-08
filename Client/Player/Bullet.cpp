#include "Bullet.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Player.h"
#include "../Monster/Monster.h"
#include "../Interface/StateInterface.h"
#include "BulletEffect.h"
#include "Component/ProjectileMovementComponent.h"
#include "Component/ColliderBox2D.h"

CBullet::CBullet()
{
	SetClassType<CBullet>();
}

CBullet::CBullet(const CBullet& ref)	:
	CGameObject(ref)
{
}

CBullet::CBullet(CBullet&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CBullet::~CBullet()
{
}

void CBullet::SetCollisionName(const std::string& Name)
{
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile(Name);
	}
}

void CBullet::SetMoveEnable(bool Enable)
{
	mMoveEnable = Enable;

	auto	Movement = mMovement.lock();

	Movement->SetEnable(Enable);
}

void CBullet::SetDistance(float Distance)
{
	mDistance = Distance;

	auto	Movement = mMovement.lock();

	Movement->SetRange(Distance);
}

void CBullet::SetSpeed(float Speed)
{
	mSpeed = Speed;

	auto	Movement = mMovement.lock();

	Movement->SetSpeed(Speed);
}

void CBullet::SetMoveDir(const FVector3& MoveDir)
{
	mMoveDirEnable = true;
	mMoveDir = MoveDir;

	auto	Movement = mMovement.lock();

	Movement->SetMoveDir(mMoveDir);
}

void CBullet::SetNearTarget(const std::string& Name)
{
	std::list<std::weak_ptr<CGameObject>>	TargetList;

	std::shared_ptr<CWorld>	World = mWorld.lock();

	if (World)
	{
		// 타겟 후보가 있을 경우에만 동작한다.
		if (World->FindObjectList<CGameObject>(Name, TargetList))
		{
			mMoveDirEnable = true;

			// FLT_MAX는 float 타입으로 표현할 수 있는 최대치이다.
			float	Dist = FLT_MAX;

			auto	iter = TargetList.begin();
			auto	iterEnd = TargetList.end();

			for (; iter != iterEnd; ++iter)
			{
				auto	Obj = (*iter).lock();

				FVector3	TargetDir = Obj->GetWorldPos() - GetWorldPos();
				float	TargetDist = TargetDir.Length();

				if (TargetDist < Dist)
				{
					Dist = TargetDist;
					mMoveDir = TargetDir;
					mMoveDir.Normalize();
				}
			}
		}
	}
}

bool CBullet::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mMovement = CreateComponent<CProjectileMovementComponent>("Movement");

	auto	Movement = mMovement.lock();

	if (Movement)
	{
		Movement->SetMoveDir(GetAxis(EAxis::Y));
		Movement->SetRangeFunction<CBullet>(this,
			&CBullet::MoveEndFunction);
		Movement->SetRange(mDistance);
		Movement->SetSpeed(mSpeed);
		Movement->SetUpdateComponent(mMeshComponent);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(50.f, 50.f);
	}

	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionBeginFunction<CBullet>(this,&CBullet::CollisionBegin);

		Body->SetCollisionEndFunction<CBullet>(this,&CBullet::CollisionEnd);

		Body->SetCollisionProfile("PlayerAttack");
		Body->SetBoxSize(50.f, 50.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
		//Body->SetInheritRot(false);
	}

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::MoveEndFunction()
{
	Destroy();
}

void CBullet::CollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	Destroy();

	std::shared_ptr<CWorld>	World = mWorld.lock();

	if (World)
	{
		std::weak_ptr<CBulletEffect> Effect = World->CreateGameObject<CBulletEffect>("BulletEffect");

		auto	_Effect = Effect.lock();

		_Effect->SetWorldPos(HitPoint);
	}
}

void CBullet::CollisionEnd(CCollider* Dest)
{
}
