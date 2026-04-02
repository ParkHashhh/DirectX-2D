#include "SturnDebuffEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CSturnDebuffEffect::CSturnDebuffEffect()
{
	SetClassType<CSturnDebuffEffect>();
}

CSturnDebuffEffect::CSturnDebuffEffect(const CSturnDebuffEffect& ref) :
	CGameObject(ref)
{
}

CSturnDebuffEffect::CSturnDebuffEffect(CSturnDebuffEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CSturnDebuffEffect::~CSturnDebuffEffect()
{
}

bool CSturnDebuffEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("SturnDebuffMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("SturnDebuffAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("SturnDebuff");
		Anim->SetLoop("SturnDebuff",true);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(66.f, 18.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer("DeBuff");
	}

	return true;
}


void CSturnDebuffEffect::Resume()
{
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetEnable(true);
	}
}
void CSturnDebuffEffect::Pause()
{

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetEnable(false);
	}
}
void CSturnDebuffEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CSturnDebuffEffect* CSturnDebuffEffect::Clone()
{
	return new CSturnDebuffEffect(*this);
}

