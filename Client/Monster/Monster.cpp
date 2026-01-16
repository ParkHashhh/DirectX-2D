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
#include "../Item/HpPotion.h"
#include "../Item/ArrowForce.h"
#include "../Item/SpeedUp.h"
#include "../Item/Item.h"

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
		Body->SetBoxSize(60.f, 60.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionMonster);

	}
	if (World)
	{
		mTargetObject = World->FindObject<CGameObject>("Player");
	}
	SetMonsterData();
	return true;
}

void CMonster::Update(float DeltaTime)
{
	SetWorldRotation(0.f, 0.f, 0.f);
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

	if (mParring)
	{
		OnHit();
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
				Line2D->SetLineDistance(Frame * mGoblinAttackDistance);
				break;
			case MonsterType::Ogre:
				Line2D->SetLineDistance(Frame * mOgreAttackDistance);
				break;
			}
			if (Line2D->GetDistance() >= 100)
			{
				Line2D->SetLineDistance(100.f);
			}
		}
	}
	auto Movement = mMovement.lock();


	if (!mIsAttack)
	{
		Movement->SetSpeed(GetDefaultSpeed());
		Anim->ChangeAnimation(mIdleAnimName);
	}
	auto Target = mTargetObject.lock();
	if (GetName() == "Ballock")
	{
		if (Target->GetWorldPos().x < GetWorldPos().x)
		{
			Anim->SetSymmetry(mIdleAnimName, false);
			Anim->SetSymmetry(mAttackAnimName, false);
		}
		else
		{
			Anim->SetSymmetry(mIdleAnimName, true);
			Anim->SetSymmetry(mAttackAnimName, true);
		}
	}
	else
	{
		if (Target->GetWorldPos().x < GetWorldPos().x)
		{
			Anim->SetSymmetry(mIdleAnimName, true);
			Anim->SetSymmetry(mAttackAnimName, true);
		}
		else
		{
			Anim->SetSymmetry(mIdleAnimName, false);
			Anim->SetSymmetry(mAttackAnimName, false);
		}
	}
	
	FVector3	TargetPos = Target->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	float TargetDistance = TargetDir.Length();
	if (!mIsAttack)
	{
		Movement->AddMove(TargetDir);
	}
	// 탐지반경 안에 들어왔을 경우
	if (TargetDistance <= mDetectRange && GetWorldPos().x <=600 && GetWorldPos().y <= 320
		&& GetWorldPos().x >= -600 && GetWorldPos().y >= -320)
	{
		mFireTime -= DeltaTime;
		if (!mIsAttack)
		{
			if (GetName() == "Ballock")
			{
				if (TargetDistance <= mDetectRange / 2)
				{
					Anim->ChangeAnimation(mAttackAnimName);

				}
			}
			else
			{
				if (mType == MonsterType::Orc)
				{
					if (mFireTime > 0)
						return;
					else
						mFireTime += 5;
				}
				Movement->SetSpeed(0);
				Anim->ChangeAnimation(mAttackAnimName);
				mIsAttack = true;
			}
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
void CMonster::OnHit()
{

}

void CMonster::Damage(int Damage)
{
	mHP -= Damage;
	if (mHP <= 0)
	{
		Destroy();
		if (rand() % 100 < mItemDropPercent)
		{
			auto World = mWorld.lock();
			EItemType ItemType = (EItemType)(rand() % 3);
			switch (ItemType)
			{
			case EItemType::HpPotion:
			{
				auto Item = World->CreateGameObject<CHpPotion>("HpPotion");
				char	Test[256] = {};
				sprintf_s(Test, "HpPotion 생성\n");
				OutputDebugStringA(Test);
				Item.lock()->SetWorldPos(GetWorldPos());
			}
			break;
			case EItemType::ArrowForce:
			{
				auto Item = World->CreateGameObject<CArrowForce>("ArrowItem");
				char	Test[256] = {};
				sprintf_s(Test, "ArrowForce 생성\n");
				OutputDebugStringA(Test);
				Item.lock()->SetWorldPos(GetWorldPos());
			}
			break;
			case EItemType::SpeedUp:
				auto Item = World->CreateGameObject<CSpeedUp>("SpeedItem");
				char	Test[256] = {};
				sprintf_s(Test, "SpeedUp 생성\n");
				OutputDebugStringA(Test);
				Item.lock()->SetWorldPos(GetWorldPos());
				break;
			}
		}
	}
	char	Test[256] = {};
	sprintf_s(Test, "HP : %d\n", mHP);
	OutputDebugStringA(Test);
}
