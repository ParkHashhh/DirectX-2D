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
	mBody = CreateComponent<CColliderSphere2D>("MonsterBody");
	auto	Line2D = mLine2D.lock();
	

	if (Line2D)
	{
		Line2D->SetCollisionProfile("Monster");
		Line2D->SetLineDistance(100.f);
		Line2D->SetDebugDraw(false);
		Line2D->SetInheritScale(false);
		Line2D->SetEnable(false);
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetRelativeScale(100.f, 100.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
	}
	auto Movement = mMovement.lock();
	if (Movement)
	{
		Movement->SetUpdateComponent(Mesh);
	}
	
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionMonster);
		Body->SetRadius(sqrtf(75.f * 75.f + 75.f * 75.f) * 0.5f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
	}

	// Target을 구한다.
	std::shared_ptr<CWorld>	World = mWorld.lock();

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

	auto World = mWorld.lock();
	if (World)
	{
		if (World->GetPlayerIsDead())
			return;
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
	auto	Line2D = mLine2D.lock();
	// 탐지반경 안에 들어왔을 경우
	if (TargetDistance <= mDetectRange)
	{
		if (!mIsAttack)
		{
			Movement->SetSpeed(0);
			Anim->ChangeAnimation(mAttackAnimName);
			mIsAttack = true;
		}
	}
	else
	{
		Line2D->SetEnable(false);
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
