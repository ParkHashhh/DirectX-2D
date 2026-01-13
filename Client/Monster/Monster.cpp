#include "Monster.h"
#include "Component/MeshComponent.h"
#include "../Player/Bullet.h"
#include "World/World.h"
#include "../Component/StateComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "Component/ObjectMovementComponent.h"


CMonster::CMonster()
{
	SetClassType<CMonster>();
}

CMonster::CMonster(const CMonster& ref) :
	CGameObject(ref)
{
	mDefaultSpeed = ref.mDefaultSpeed;
	mDetectRange = ref.mDetectRange;
	mTargetObject = ref.mTargetObject;
}

CMonster::CMonster(CMonster&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("MonsterMesh");
	mStateComponent = CreateComponent<CStateComponent>("MonsterState");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("MonsterAnimation2D");
	mMovement = CreateComponent<CObjectMovementComponent>("MonsterMovement");
	mLine2D = CreateComponent<CColliderLine2D>("MonsterLine2D");
	mBody = CreateComponent<CColliderBox2D>("MonsterBody");

	auto	Line2D = mLine2D.lock();
	auto	Mesh = mMeshComponent.lock();
	auto    Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	auto	Body = mBody.lock();
	std::shared_ptr<CWorld>	World = mWorld.lock();



	if (Line2D)
	{
		Line2D->SetCollisionProfile("Monster");
		Line2D->SetLineDistance(100.f);
		Line2D->SetDebugDraw(false);
		Line2D->SetInheritScale(false);
		Line2D->SetEnable(false);
	}
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetRelativeScale(100.f, 100.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
	}
	if (Movement)
	{
		Movement->SetUpdateComponent(Mesh);
	}
	

	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionMonster);
		Body->SetBoxSize(75.f, 80.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
	}


	// Target을 구한다.

	if (World)
	{
		mTargetObject = World->FindObject<CGameObject>("Player");
	}
	SetMonsterData();
	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	auto	Anim = mAnimation2DComponent.lock();
	auto	Line2D = mLine2D.lock();
	auto	Body = mBody.lock();
	auto World = mWorld.lock();
	if (World)
	{
		if (World->GetPlayerIsDead())
			return;
	}


	if (mIsAttack)
	{
		if (Line2D->GetDistance() <= 100)
		{
			float Frame = (float)Anim->GetAnimationFrame();
			switch (mType)
			{
			case MonsterType::Goblin:
				Line2D->SetLineDistance(Frame * 5);
				break;
			case MonsterType::Ogre:
				Line2D->SetLineDistance(Frame * 8);
				break;
			}
			if (Line2D->GetDistance() >= 100)
			{
				Line2D->SetLineDistance(100.f);
			}

		}
	}
	auto Target = mTargetObject.lock();
	auto Movement = mMovement.lock();


	if (!mIsAttack)
	{
		Movement->SetSpeed(GetDefaultSpeed());
		Anim->ChangeAnimation(mIdleAnimName);
	}

	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	float TargetDistance = TargetDir.Length();
	float Angle = GetWorldPos().GetViewTargetAngle2D(Target->GetWorldPos(), EAxis::Y);
	SetWorldRotationZ(Angle);
	if (!mIsAttack)
	{
		Movement->AddMove(GetAxis(EAxis::Y));
	}
	mFireTime -= DeltaTime;
	// 탐지반경 안에 들어왔을 경우
	if (TargetDistance <= mDetectRange)
	{
		if (!mIsAttack)
		{
			if (mType == MonsterType::Orc)
			{
				if (mFireTime > 0)
					return;
				else
					mFireTime += 3;
			}
		Movement->SetSpeed(0);
		Anim->ChangeAnimation(mAttackAnimName);
		mIsAttack = true;
		}
	}
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::CollisionMonster(const FVector3& HitPoint, CCollider* Dest)
{
	Damage(1);
}

void CMonster::AttackNotify()
{

}

void CMonster::AttackFinish()
{
}

void CMonster::SetMonsterData()
{
}
float CMonster::GetDefaultSpeed()
{
	return mDefaultSpeed;
}
