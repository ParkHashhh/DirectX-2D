#include "RightHand.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"
#include "../../Component/StateComponent.h"


CRightHand::CRightHand()
{
	SetClassType<CRightHand>();
}

CRightHand::CRightHand(const CRightHand& ref) :
	CHornTailPart(ref)
{
}

CRightHand::CRightHand(CRightHand&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CRightHand::~CRightHand()
{
}

bool CRightHand::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailRightHandMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailRightHandAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailRightHandBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailRightHandState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailRightHand_Idle");
		Anim->AddAnimation("HornTailRightHand_Attack");
		Anim->AddAnimation("HornTailRightHand_Die");

		Anim->SetLoop("HornTailRightHand_Idle", true);

		Anim->SetPlayRate("HornTailRightHand_Idle", 0.2f);
		Anim->SetPlayRate("HornTailRightHand_Attack", 0.5f);
		Anim->SetPlayRate("HornTailRightHand_Die",0.3f);

		Anim->AddNotify<CRightHand>("HornTailRightHand_Attack",
			"HornTailRightHand_Attack", 0, this, &CRightHand::SkillStart);
		Anim->SetFinishNotify<CRightHand>("HornTailRightHand_Attack", this, &CRightHand::SkillEnd);

		Anim->AddNotify<CRightHand>("HornTailRightHand_Die",
			"HornTailRightHand_Die", 20, this, &CRightHand::StartDie);
		Anim->SetFinishNotify<CRightHand>("HornTailRightHand_Die", this, &CRightHand::EndDie);

		Anim->ChangeAnimation("HornTailRightHand_Idle");
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
		Body->SetEnable(true);
		Body->AddWorldPos(50, 100);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CRightHand>(this, &CRightHand::OnHit);
	}
	SetHPMax(500000.f);

	return true;
}

void CRightHand::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(252.f, 385.2f);
		Mesh->SetRelativePos(mDiePos);
		Mesh->AddWorldPos(-2, -2.f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	mIsDead = true;

}

void CRightHand::EndDie()
{
}

void CRightHand::ChangeAttack()
{

	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailRightHand_Attack");
	}
}

void CRightHand::OnHit(const FVector3& HitPoint, class CCollider* Dest)
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

CRightHand* CRightHand::Clone()
{
	return new CRightHand(*this);
}

void CRightHand::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(362.4f, 474.f);
		Mesh->AddWorldPos(53, 42);
	}

	auto Target = std::dynamic_pointer_cast<CPlayer>(mTargetObject.lock());

	if (Target)
	{
		Target->SetDarkDebuff(true);
	}
}
void CRightHand::SkillEnd()	
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(258.f, 377.f);
		Mesh->AddWorldPos(-53, -42);
	}
	auto	Anim = mAnimation2DComponent.lock();

	

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailRightHand_Idle");
	}
}
void CRightHand::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailRightHand_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x + 50, GetWorldPos().y + SizeY + 100);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}