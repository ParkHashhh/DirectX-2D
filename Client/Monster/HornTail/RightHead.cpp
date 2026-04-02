#include "RightHead.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../../Component/StateComponent.h"
#include "../DamageObject.h"
#include "Wyvern.h"


CRightHead::CRightHead()
{
	SetClassType<CRightHead>();
}

CRightHead::CRightHead(const CRightHead& ref) :
	CHornTailPart(ref)
{
}

CRightHead::CRightHead(CRightHead&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CRightHead::~CRightHead()
{
}

bool CRightHead::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailRightHeadMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailRightHeadAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailRightHeadBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailRightHeadState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailRightHead_Idle");
		Anim->AddAnimation("HornTailRightHead_Attack");
		Anim->AddAnimation("HornTailRightHead_Die");

		Anim->SetLoop("HornTailRightHead_Idle", true);

		Anim->SetPlayRate("HornTailRightHead_Idle", 0.15f);
		Anim->SetPlayRate("HornTailRightHead_Attack", 0.5f);
		Anim->SetPlayRate("HornTailRightHead_Die", 0.3f);

		Anim->AddNotify<CRightHead>("HornTailRightHead_Attack",
			"HornTailRightHead_Attack", 0, this, &CRightHead::SkillStart);
		Anim->SetFinishNotify<CRightHead>("HornTailRightHead_Attack", this, &CRightHead::SkillEnd);


		Anim->AddNotify<CRightHead>("HornTailRightHead_Die",
			"HornTailRightHead_Die", 20, this, &CRightHead::StartDie);
		Anim->SetFinishNotify<CRightHead>("HornTailRightHead_Die", this, &CRightHead::EndDie);


		Anim->ChangeAnimation("HornTailRightHead_Idle");
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(313.f, 618.f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(150.f, 150.f);
		Body->AddWorldPos(50, 200);

		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CRightHead>(this, &CRightHead::OnHit);
	}
	SetHPMax(500000.f);

	return true;
}


void CRightHead::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetRelativeScale(303.6f, 606.f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	mIsDead = true;

}

void CRightHead::EndDie()
{
}
void CRightHead::OnHit(const FVector3& HitPoint, class CCollider* Dest)
{

	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.25f,false);
		}
	}
}

void CRightHead::ChangeAttack()
{
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailRightHead_Attack");
	}
}

CRightHead* CRightHead::Clone()
{
	return new CRightHead(*this);
}


void CRightHead::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(418.8f, 606.f);
	}

	// 몬스터 5마리 소환 후 위아래로 움직이고 플레이어 쫓아가는 로직
	auto World = mWorld.lock();
	if (World)
	{
		for (int i = 0; i < mMonsterCount; i++)
		{

			auto Wyvern = World->CreateGameObject<CWyvern>("Wyvern" + std::to_string(rand())).lock();
			FVector3 Pos = GetWorldPos();
			Pos.y += i * 30;
			Pos.x += i * 20;
			Wyvern->SetWorldPos(Pos);
		}
	}
}

void CRightHead::SkillEnd()
{
	auto	Anim = mAnimation2DComponent.lock();
	auto	Mesh = mMeshComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailRightHead_Idle");
		if (Mesh)
		{
			Mesh->SetWorldScale(313.f, 618.f);
		}
	}
}



void CRightHead::Damage(float Damage)
{
	CMonster::Damage(Damage);
	auto State = mStateComponent.lock();

	if (State)
	{
		if (State->IsDead())
		{
			auto Anim = mAnimation2DComponent.lock();
			auto Mesh = mMeshComponent.lock();

			if (Anim)
			{
				Anim->ChangeAnimation("HornTailRightHead_Die");
			}

		}
	}
	auto World = mWorld.lock();
	auto Body = mBody.lock();
	float SizeY = 0;
	if (Body)
	{
		SizeY = Body->GetInfo().HalfSize.y;
	}
	auto DamageObj = World->CreateGameObject<CDamageObject>("Damage" + std::to_string(rand())).lock();
	DamageObj->SetWorldPos(GetWorldPos().x + 70, GetWorldPos().y + SizeY + 200);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}