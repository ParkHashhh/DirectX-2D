#include "MiddleHead.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../../Component/StateComponent.h"
#include "../DamageObject.h"

CMiddleHead::CMiddleHead()
{
	SetClassType<CMiddleHead>();
}

CMiddleHead::CMiddleHead(const CMiddleHead& ref) :
	CHornTailPart(ref)
{
}

CMiddleHead::CMiddleHead(CMiddleHead&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CMiddleHead::~CMiddleHead()
{
}

bool CMiddleHead::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailMiddleHeadMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailMiddleHeadAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailMiddleHeadBody", "HornTailMiddleHeadMesh");
	mAttackBody = CreateComponent<CColliderBox2D>("HornTailMiddleHeadAttackBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailMiddleHeadState");

	// 애니메이션 지정

	
	auto	Mesh = mMeshComponent.lock();
	
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(193.f, 754.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer(0);

	}	
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(200.f, 200.f);
		Body->SetWorldScale(200, 800);

		Body->SetRelativePos(0, 180.f);
		Body->SetInheritScale(false);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CMiddleHead>(this, &CMiddleHead::OnHit);
	}
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailMiddleHead_Idle");
		Anim->AddAnimation("HornTailMiddleHead_Attack");
		Anim->AddAnimation("HornTailMiddleHead_Die");

		Anim->SetLoop("HornTailMiddleHead_Idle", true);

		Anim->SetPlayRate("HornTailMiddleHead_Idle", 0.2f);
		Anim->SetPlayRate("HornTailMiddleHead_Attack", 0.3f);
		Anim->SetPlayRate("HornTailMiddleHead_Die", 0.3f);

		Anim->AddNotify<CMiddleHead>("HornTailMiddleHead_Attack",
			"HornTailMiddleHead_Attack", 0, this, &CMiddleHead::SkillStart);
		Anim->AddNotify<CMiddleHead>("HornTailMiddleHead_Attack",
			"HornTailMiddleHead_Attack", 19, this, &CMiddleHead::EnableAttackBody);

		Anim->AddNotify<CMiddleHead>("HornTailMiddleHead_Attack",
			"HornTailMiddleHead_Attack", 21, this, &CMiddleHead::DisableAttackBody);
		Anim->SetFinishNotify<CMiddleHead>("HornTailMiddleHead_Attack", this, &CMiddleHead::SkillEnd);



		Anim->AddNotify<CMiddleHead>("HornTailMiddleHead_Die",
			"HornTailMiddleHead_Die", 20, this, &CMiddleHead::StartDie);
		Anim->SetFinishNotify<CMiddleHead>("HornTailMiddleHead_Die", this, &CMiddleHead::EndDie);

		Anim->ChangeAnimation("HornTailMiddleHead_Idle");
	}
	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->SetBoxSize(500.f, 300.f);
		AttackBody->SetRelativePos(0, -320);
		AttackBody->SetWorldScale(200, 300);
		AttackBody->SetInheritScale(false);
		AttackBody->SetEnable(false);
		AttackBody->SetDebugDraw(false);
		AttackBody->SetCollisionProfile("MonsterAttack");
		AttackBody->SetCollisionBeginFunction<CMiddleHead>(this, &CMiddleHead::OnHitAttack);
	}
	SetHPMax(500000.f);


	return true;
}


void CMiddleHead::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetWorldScale(193.2f, 754.8f);
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

void CMiddleHead::EndDie()
{
}

void CMiddleHead::OnHit(const FVector3& HitPoint, class CCollider* Dest)
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

void CMiddleHead::OnHitAttack(const FVector3& HitPoint, CCollider* Dest)
{
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.7f,true);
		}
	}
}

CMiddleHead* CMiddleHead::Clone()
{
	return new CMiddleHead(*this);
}


void CMiddleHead::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(662.4f, 932.4f);
		Mesh->AddRelativePos(0, 22.f);
	}
}
void CMiddleHead::SkillEnd()
{
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto AttackBody = mAttackBody.lock();
	auto Body = mBody.lock();
	if (Body)
	{
		Body->AddWorldPos(0, 150);
	}
	if (AttackBody)
	{
		AttackBody->ClearCollisionList();
	}
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailMiddleHead_Idle");
	}
	if (Mesh)
	{
		Mesh->AddRelativePos(0, -22.f);
		Mesh->SetWorldScale(193.f, 754.f);
	}
}

void CMiddleHead::EnableAttackBody()
{
	auto AttackBody = mAttackBody.lock();
	auto Body = mBody.lock();
	if (Body)
	{
		Body->AddWorldPos(0, -150);
	}
	if (AttackBody)
	{
		AttackBody->SetEnable(true);
	}
}

void CMiddleHead::DisableAttackBody()
{
	auto AttackBody = mAttackBody.lock();
	if (AttackBody)
	{
		AttackBody->SetEnable(false);
	}
}

void CMiddleHead::ChangeAttack()
{

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailMiddleHead_Attack");
		
	}
}


void CMiddleHead::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailMiddleHead_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x, GetWorldPos().y + SizeY + 200);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}