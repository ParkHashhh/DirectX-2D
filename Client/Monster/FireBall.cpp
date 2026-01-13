#include "FireBall.h"
#include "Component/MeshComponent.h"
#include "Component/CameraComponent.h"
#include "Device.h"
#include "World/World.h"
#include "World/CameraManager.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ProjectileMovementComponent.h"
#include "Component/Animation2DComponent.h"


CFireBall::CFireBall()
{
	SetClassType<CFireBall>();
}

CFireBall::CFireBall(const CFireBall& ref) :
	CGameObject(ref)
{
}

CFireBall::CFireBall(CFireBall&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CFireBall::~CFireBall()
{
}

void CFireBall::SetCollisionName(const std::string& Name)
{
	auto	Ball = mBall.lock();

	if (Ball)
	{
		Ball->SetCollisionProfile(Name);
	}
}
bool CFireBall::Init()
{
	CGameObject::Init();


	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mMovement = CreateComponent<CProjectileMovementComponent>("Movement");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Anim");
	mBall = CreateComponent<CColliderSphere2D>("FireBall");
	auto	Mesh = mMeshComponent.lock();
	auto	Movement = mMovement.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Ball = mBall.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation("OrcFireBall");
		Anim->ChangeAnimation("OrcFireBall");
		Anim->SetLoop("OrcFireBall", true);
	}
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Movement->SetMoveDir(GetAxis(EAxis::X));
		Mesh->SetRelativeScale(50.f, 50.f);
	}
	if (Movement)
	{
		Movement->SetRangeFunction<CFireBall>(this,
			&CFireBall::MoveEndFunction);
		Movement->SetSpeed(mSpeed);
		Movement->SetRange(mDistance);
		Movement->SetUpdateComponent(mMeshComponent);
	}
	if (Ball)
	{
		Ball->SetCollisionBeginFunction<CFireBall>(this, &CFireBall::CollisionBegin);
		Ball->SetCollisionEndFunction<CFireBall>(this, &CFireBall::CollisionEnd);
		Ball->SetCollisionProfile("MonsterAttack");
		Ball->SetRadius(20);
		Ball->SetDebugDraw(true);
		Ball->SetInheritScale(false);
		Ball->SetInheritRot(true);
	}
	

	return true;
}

CFireBall* CFireBall::Clone()
{
	return new CFireBall(*this);
}

void CFireBall::SetNearTarget(const std::string& Name)
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
void CFireBall::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CFireBall::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}


void CFireBall::MoveEndFunction()
{
	Destroy();
}

void CFireBall::CollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	Destroy();
}

void CFireBall::CollisionEnd(CCollider* Dest)
{
}


void CFireBall::SetMoveEnable(bool Enable)
{
	mMoveEnable = Enable;

	auto	Movement = mMovement.lock();

	Movement->SetEnable(Enable);
}

void CFireBall::SetDistance(float Distance)
{
	mDistance = Distance;

	auto	Movement = mMovement.lock();

	Movement->SetRange(Distance);
}

void CFireBall::SetSpeed(float Speed)
{
	mSpeed = Speed;

	auto	Movement = mMovement.lock();

	Movement->SetSpeed(Speed);
}

void CFireBall::SetMoveDir(const FVector3& MoveDir)
{
	mMoveDirEnable = true;
	mMoveDir = MoveDir;

	auto	Movement = mMovement.lock();

	Movement->SetMoveDir(mMoveDir);
	float Radian = atan2f(mMoveDir.y, mMoveDir.x);

	float Degree = Radian * (180.f / 3.141592f);

	SetWorldRotation(0.f, 0.f, Degree);
}
