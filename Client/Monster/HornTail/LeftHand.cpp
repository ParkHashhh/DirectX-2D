#include "LeftHand.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../../Component/StateComponent.h"
#include "../DamageObject.h"


CLeftHand::CLeftHand()
{
	SetClassType<CLeftHand>();
}

CLeftHand::CLeftHand(const CLeftHand& ref) :
	CHornTailPart(ref)
{
}

CLeftHand::CLeftHand(CLeftHand&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CLeftHand::~CLeftHand()
{
}

bool CLeftHand::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailLeftHandMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailLeftHandAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailLeftHandBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailLeftHandState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailLeftHand_Idle");
		Anim->AddAnimation("HornTailLeftHand_Attack");
		Anim->AddAnimation("HornTailLeftHand_Die");

		Anim->SetLoop("HornTailLeftHand_Idle", true);

		Anim->SetPlayRate("HornTailLeftHand_Idle", 0.3f);
		Anim->SetPlayRate("HornTailLeftHand_Attack", 0.5f);
		Anim->SetPlayRate("HornTailLeftHand_Die", 0.3f);

		Anim->AddNotify<CLeftHand>("HornTailLeftHand_Attack",
			"HornTailLeftHand_Attack", 0, this, &CLeftHand::SkillStart);
		Anim->SetFinishNotify<CLeftHand>("HornTailLeftHand_Attack", this, &CLeftHand::SkillEnd);

		Anim->AddNotify<CLeftHand>("HornTailLeftHand_Die",
			"HornTailLeftHand_Die", 20, this, &CLeftHand::StartDie);
		Anim->SetFinishNotify<CLeftHand>("HornTailLeftHand_Die", this, &CLeftHand::EndDie);

		Anim->ChangeAnimation("HornTailLeftHand_Idle");

	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(258.f, 377.f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(150.f, 150.f);	
		Body->AddWorldPos(-50, 100);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CLeftHand>(this, &CLeftHand::OnHit);
	}
	SetHPMax(500000.f);

	return true;
}

void CLeftHand::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(252.f, 385.2f);
		Mesh->SetRelativePos(mDiePos);
		Mesh->AddWorldPos(2, -2.f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	mIsDead = true;

}

void CLeftHand::EndDie()
{
}

void CLeftHand::ChangeAttack()
{
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeftHand_Attack");
	}
}


void CLeftHand::OnHit(const FVector3& HitPoint, class CCollider* Dest)
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

CLeftHand* CLeftHand::Clone()
{
	return new CLeftHand(*this);
}

void CLeftHand::SkillStart()
{
	auto Target = std::dynamic_pointer_cast<CPlayer>(mTargetObject.lock());

	if (Target)
	{
		Target->SetSturnDebuff(true);
	}

	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(379.2f, 441.6f);
		Mesh->AddWorldPos(-60, 21);
	}
}
void CLeftHand::SkillEnd()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(258.f, 377.f);
		Mesh->AddWorldPos(60, -21);
	}
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeftHand_Idle");
	}
}


void CLeftHand::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailLeftHand_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x - 50, GetWorldPos().y + SizeY + 100);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}