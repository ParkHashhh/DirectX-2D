#include "Leg.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"
#include "../../Component/StateComponent.h"


CLeg::CLeg()
{
	SetClassType<CLeg>();
}

CLeg::CLeg(const CLeg& ref) :
	CHornTailPart(ref)
{
}

CLeg::CLeg(CLeg&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CLeg::~CLeg()
{
}

bool CLeg::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailLegMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailLegAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailLegBody");
	mAttackBody = CreateComponent<CColliderBox2D>("HornTailLegAttackBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailLegState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailLeg_Idle");
		Anim->AddAnimation("HornTailLeg_Attack");
		Anim->AddAnimation("HornTailLeg_Die");

		Anim->SetPlayRate("HornTailLeg_Attack", 0.5f);
		Anim->SetPlayRate("HornTailLeg_Die", 0.3f);


		Anim->AddNotify<CLeg>("HornTailLeg_Attack",
			"HornTailLeg_Attack", 0, this, &CLeg::SkillStart);

		Anim->AddNotify<CLeg>("HornTailLeg_Attack",
			"HornTailLeg_Attack", 12, this, &CLeg::EnableBody);

		Anim->AddNotify<CLeg>("HornTailLeg_Attack",
			"HornTailLeg_Attack", 13, this, &CLeg::DisableBody);
		Anim->SetFinishNotify<CLeg>("HornTailLeg_Attack", this, &CLeg::SkillEnd);

		Anim->AddNotify<CLeg>("HornTailLeg_Die",
			"HornTailLeg_Die", 20, this, &CLeg::StartDie);
		Anim->SetFinishNotify<CLeg>("HornTailLeg_Die", this, &CLeg::EndDie);

		Anim->ChangeAnimation("HornTailLeg_Idle");
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(617.f, 230.f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(150.f, 200.f);
		Body->SetEnable(true);
		Body->AddRelativePos(-200.f,0);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CLeg>(this, &CLeg::OnHit);
	}
	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->SetBoxSize(400.f, 10.f);
		AttackBody->SetEnable(false);
		AttackBody->AddRelativePos(-450.f, -100);
		AttackBody->SetDebugDraw(false);
		AttackBody->SetCollisionProfile("Monster");
		AttackBody->SetCollisionBeginFunction<CLeg>(this, &CLeg::OnHitAttack);
	}
	SetHPMax(500000.f);

	return true;
}


void CLeg::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetWorldScale(616.8f, 236.4f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->SetEnable(false);
	}
	mIsDead = true;

}

void CLeg::EndDie()
{
}

void CLeg::OnHit(const FVector3& HitPoint, class CCollider* Dest)
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

void CLeg::OnHitAttack(const FVector3& HitPoint, class CCollider* Dest)
{

	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->SetHP(1);

		}
	}
}

CLeg* CLeg::Clone()
{
	return new CLeg(*this);
}

void CLeg::ChangeAttack()
{
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeg_Attack");

	}
}

void CLeg::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(708.f, 308.4f);
		Mesh->AddRelativePos(-46, 15.f);
	}

}
void CLeg::SkillEnd()
{

	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeg_Idle");
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(617.f, 230.f);
		Mesh->AddRelativePos(46, -15.f);
	}

	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->ClearCollisionList();
	}
}
void CLeg::EnableBody()
{
	auto AttackBody = mAttackBody.lock();
	auto Target = std::dynamic_pointer_cast<CPlayer>(mTargetObject.lock());

	if (AttackBody)
	{
		AttackBody->SetEnable(true);
		if (Target)
			AttackBody->SetWorldPos(Target->GetWorldPos().x, AttackBody->GetWorldPos().y);
	}
}

void CLeg::DisableBody()
{
	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->SetEnable(false);
	}
}

void CLeg::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailLeg_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x - 200, GetWorldPos().y + SizeY );
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}
