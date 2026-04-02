#include "Tail.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"
#include "../../Component/StateComponent.h"
#include "Poison.h"

CTail::CTail()
{
	SetClassType<CTail>();
}

CTail::CTail(const CTail& ref) :
	CHornTailPart(ref)
{
}

CTail::CTail(CTail&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CTail::~CTail()
{
}

bool CTail::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailTailMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailTailAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailTailBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailTailState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailTail_Idle");
		Anim->AddAnimation("HornTailTail_Attack");
		Anim->AddAnimation("HornTailTail_Die");

		Anim->SetPlayRate("HornTailTail_Attack", 0.5f);
		Anim->SetPlayRate("HornTailTail_Die", 0.3f);

		Anim->AddNotify<CTail>("HornTailTail_Attack",
			"HornTailTail_Attack", 0, this, &CTail::SkillStart);
		Anim->SetFinishNotify<CTail>("HornTailTail_Attack", this, &CTail::SkillEnd);


		Anim->AddNotify<CTail>("HornTailTail_Die",
			"HornTailTail_Die", 0, this, &CTail::StartDie);
		Anim->SetFinishNotify<CTail>("HornTailTail_Die", this, &CTail::EndDie);

		Anim->ChangeAnimation("HornTailTail_Idle");
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(512.f, 175.f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(300.f, 50.f);
		Body->AddRelativePos(100.f, -50.f);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CTail>(this, &CTail::OnHit);
	}
	SetHPMax(300000.f);

	return true;
}

void CTail::OnHit(const FVector3& HitPoint, class CCollider* Dest)
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

CTail* CTail::Clone()
{
	return new CTail(*this);
}

void CTail::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetWorldScale(512.4f, 175.2f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	mIsDead = true;

}

void CTail::EndDie()
{
}


void CTail::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(522.f, 267.6f);
		Mesh->AddRelativePos(0.f, 50.f);
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(false);
	}
	auto World = mWorld.lock();
	if (World)
	{
		auto Poison = World->CreateGameObject<CPoison>("Poison").lock();
		Poison->SetWorldPos(GetWorldPos().x + 100, GetWorldPos().y + 30);
	}
}

void CTail::SkillEnd()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(512.f, 175.f);
		Mesh->AddRelativePos(0.f, -50.f);
	}
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailTail_Idle");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(true);
	}
}

void CTail::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailTail_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x + 100, GetWorldPos().y - 50);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}


void CTail::ChangeAttack()
{
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailTail_Attack");
	}

}