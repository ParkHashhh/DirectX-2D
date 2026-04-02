#include "Shadow.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"


CShadow::CShadow()
{
}

CShadow::CShadow(const CShadow& ref)
{
}

CShadow::CShadow(CShadow&& ref) noexcept
{
}

CShadow::~CShadow()
{
}

void CShadow::AnimStart()
{
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(70, 70);
	}
}

void CShadow::AnimEnd()
{
}

bool CShadow::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("ShadowMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ShadowAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	mAttackAnimList.push_back("ShadowAttack");
	mAttackAnimList.push_back("ShadowAttack1");
	mAttackAnimList.push_back("ShadowAttack2");
	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);


		Anim->AddAnimation("ShadowIdle");
		Anim->AddAnimation("ShadowWalk");
		Anim->AddAnimation("ShadowAttack");
		Anim->AddAnimation("ShadowAttack1");
		Anim->AddAnimation("ShadowAttack2");
		Anim->AddAnimation("ShadowAttack3");
		Anim->AddAnimation("ShadowDown");
		Anim->AddAnimation("ShadowJump");

		Anim->SetLoop("ShadowIdle", true);
		Anim->SetLoop("ShadowWalk", true);

		Anim->SetPlayRate("ShadowWalk", 1.05f);
		Anim->SetPlayRate("ShadowAttack", 1.2f);
		Anim->SetPlayRate("ShadowAttack1", 1.2f);
		Anim->SetPlayRate("ShadowAttack2", 1.2f);
		Anim->SetPlayRate("ShadowAttack3", 0.9f);


		Anim->AddNotify<CShadow>("ShadowIdle",
			"AttackNotify", 0, this, &CShadow::IdleNotify);

		Anim->AddNotify<CShadow>("ShadowWalk",
			"AttackNotify", 0, this, &CShadow::IdleNotify);
		Anim->AddNotify<CShadow>("ShadowAttack",
			"AttackNotify", 0, this, &CShadow::AnimStart);
		Anim->SetFinishNotify<CShadow>("ShadowAttack",
			this, &CShadow::AnimEnd);

		Anim->AddNotify<CShadow>("ShadowAttack1",
			"AttackNotify", 0, this, &CShadow::AnimStart);
		Anim->SetFinishNotify<CShadow>("ShadowAttack1",
			this, &CShadow::AnimEnd);

		Anim->AddNotify<CShadow>("ShadowAttack2",
			"AttackNotify", 0, this, &CShadow::AnimStart);
		Anim->SetFinishNotify<CShadow>("ShadowAttack2",
			this, &CShadow::AnimEnd);

		Anim->AddNotify<CShadow>("ShadowAttack3",
			"AttackNotify", 0, this, &CShadow::AnimStart);
		Anim->SetFinishNotify<CShadow>("ShadowAttack3",
			this, &CShadow::AnimEnd);

	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(50.f, 70.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialOpacity(0, 1.f);
		Mesh->SetRenderLayer("Player");


	}

	return true;
}

void CShadow::SetOpacity(float Opacity)
{
	auto Mesh = mMeshComponent.lock();
	Mesh->SetMaterialOpacity(0, Opacity);
}

void CShadow::IdleNotify()
{

	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(50, 70);
	}
}

void CShadow::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CShadow::SetHorntailWorld(bool HornTailWorld)
{
	mIsHorntailWorld = HornTailWorld;
}

