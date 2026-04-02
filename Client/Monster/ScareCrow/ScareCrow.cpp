#include "ScareCrow.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "UI/NumberWidget.h"
#include "../DamageObject.h"
#include "Component/WidgetComponent.h"

CScareCrow::CScareCrow()
{
	SetClassType<CScareCrow>();
}

CScareCrow::CScareCrow(const CScareCrow& ref)
	: CMonster(ref)
{
}


CScareCrow::CScareCrow(CScareCrow&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CScareCrow::~CScareCrow()
{

}

bool CScareCrow::Init()
{
	if (!CMonster::Init())
		return false;
	mMeshComponent = CreateComponent<CMeshComponent>("ScareCrowMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ScareCrowAnimation2D");
	mBody = CreateComponent<CColliderBox2D>("ScareCrowBody");
	mWidget = CreateComponent<CWidgetComponent>("ScareCrowComponent");
	auto	Mesh = mMeshComponent.lock();
	auto    Anim = mAnimation2DComponent.lock();
	auto	Body = mBody.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetWorldScale(120, 100);
	}
	if (Body)
	{
		Body->SetWorldScale(100, 100);
		Body->SetCollisionProfile("Monster");
		Body->SetInheritScale(false);
		Body->SetBoxSize(100.f, 100.f);
		Body->SetDebugDraw(false);
		Body->SetCollisionBeginFunction<CScareCrow>(this, &CScareCrow::OnHit);
	}


	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation("ScareCrowStand");
		Anim->AddAnimation("ScareCrowHit");
		Anim->ChangeAnimation("ScareCrowStand");
		Anim->SetLoop("ScareCrowStand", true);
		Anim->SetFinishNotify<CScareCrow>("ScareCrowHit", this, &CScareCrow::HitFinish);
	}

	return true;
}


void CScareCrow::OnHit(const FVector3& HitPoint,
	class CCollider* Dest)
{
	CMonster::OnHit(HitPoint, Dest);
	if (Dest->GetCollisionProfile()->Name == "PlayerAttack")
	{
		auto Anim = mAnimation2DComponent.lock();
		if (Anim)
			Anim->ChangeAnimation("ScareCrowHit");
	}
}

void CScareCrow::HitFinish()
{
	auto	Anim = mAnimation2DComponent.lock();
	auto Body = mBody.lock();
	if (Body)
		Body->ClearCollisionList();
	if (Anim)
	{
		Anim->ChangeAnimation("ScareCrowStand");
	}
}

CScareCrow* CScareCrow::Clone()
{
	return new CScareCrow(*this);
}