#include "ArmSkill.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "../../Player/RayTracing.h"
#include "World/World.h"
#include "FallingStone.h"
#include "Component/ColliderBox2D.h"
#include "ZakumArm.h"
#include "../../Player/Player.h"


CArmSkill::CArmSkill()
{
	SetClassType<CArmSkill>();
}

CArmSkill::CArmSkill(const CArmSkill& ref) :
	CGameObject(ref)
{
}

CArmSkill::CArmSkill(CArmSkill&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CArmSkill::~CArmSkill()
{
}

bool CArmSkill::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("ArmSkillMesh" + std::to_string(rand()));
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ArmSkillAnimation2D" + std::to_string(rand()));
	mBody = CreateComponent<CColliderBox2D>("ArmSkillBody" + std::to_string(rand()));

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->AddAnimation("ZakumArm_Skill");
		Anim->SetPlayRate("ZakumArm_Skill", 0.5f);
		Anim->SetFinishNotify<CArmSkill>("ZakumArm_Skill",
			this, &CArmSkill::FinishAnimation);

		Anim->AddNotify<CArmSkill>("ZakumArm_Skill",
			"ZakumArm_Skill", 55, this, &CArmSkill::EnableBox);

		Anim->AddNotify<CArmSkill>("ZakumArm_Skill",
			"ZakumArm_Skill", 63, this, &CArmSkill::DisableBox);

	}
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(200.f, 200.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(100.f, 100.f);
		Body->SetEnable(false);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("MonsterAttack");
		Body->SetCollisionBeginFunction<CArmSkill>(this, &CArmSkill::OnHit);
	}
	return true;
}
void CArmSkill::EnableBox()
{
	auto Body = mBody.lock();
	if (Body)
	{

		Body->SetEnable(true);
		if (mSymmetry)
		{
			Body->AddRelativePos(30, 0);
		}
		else
		{
			Body->AddRelativePos(-30, 0);

		}
	}
}
void CArmSkill::DisableBox()
{

	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(false);
	}
}
void CArmSkill::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();
	mSymmetry = Symmetry;
	if (Anim)
	{
		Anim->SetSymmetry("ZakumArm_Skill", Symmetry);
	}
}
void CArmSkill::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}
void CArmSkill::OnHit(const FVector3& HitPoint, class CCollider* Dest)
{
	
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.50f);
		}
	}
}

CArmSkill* CArmSkill::Clone()
{
	return new CArmSkill(*this);
}
void CArmSkill::FinishAnimation()
{
	if (!mOwner.expired())
	{
		mOwner.lock()->SetMeshEnable(true);
		mOwner.lock()->SetBodyEnable(true);
	}

	Destroy();
}
