#include "Wing.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"


CWing::CWing()
{
	SetClassType<CWing>();
}

CWing::CWing(const CWing& ref) :
	CHornTailPart(ref)
{
}

CWing::CWing(CWing&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CWing::~CWing()
{
}

bool CWing::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailWingMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailWingAnim");

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->AddAnimation("HornTailWing_Idle");
		Anim->AddAnimation("HornTailWing_Attack");
		Anim->AddAnimation("HornTailWing_Die");

		Anim->SetPlayRate("HornTailWing_Idle", 0.3f);
		Anim->SetPlayRate("HornTailWing_Attack", 0.3f);
		Anim->SetPlayRate("HornTailWing_Die", 0.3f);

		Anim->AddNotify<CWing>("HornTailWing_Attack",
			"HornTailWing_Attack", 2, this, &CWing::SkillStart);
		Anim->SetFinishNotify<CWing>("HornTailWing_Attack", this, &CWing::SkillEnd);

		Anim->AddNotify<CWing>("HornTailWing_Die",
			"HornTailWing_Die", 2, this, &CWing::StartDie);
		Anim->SetFinishNotify<CWing>("HornTailWing_Die", this, &CWing::EndDie);


		Anim->SetLoop("HornTailWing_Idle", true);

		Anim->ChangeAnimation("HornTailWing_Idle");

	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(708.f, 436.8f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	SetHPMax(0);
	
	
	return true;
}


CWing* CWing::Clone()
{
	return new CWing(*this);
}

void CWing::ChangeAttack()
{
	auto Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	Mesh->AddWorldPos(0.f, 180.f);
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailWing_Attack");
	}
}
void CWing::ChangeDie()
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailWing_Die");
	}
}


void CWing::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetWorldScale(688.8f, 436.8f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}

}

void CWing::EndDie()
{
	mIsDead = true;
}


void CWing::SkillStart()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(1219.2f, 751.2f);
	}
}
void CWing::SkillEnd()
{
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(708.f, 436.8f);
		Mesh->AddWorldPos(0.f, -180.f);
	}
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("HornTailWing_Idle");
	}
}
