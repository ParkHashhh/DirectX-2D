#include "DarkDebuffEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CDarkDebuffEffect::CDarkDebuffEffect()
{
	SetClassType<CDarkDebuffEffect>();
}

CDarkDebuffEffect::CDarkDebuffEffect(const CDarkDebuffEffect& ref) :
	CGameObject(ref)
{
}

CDarkDebuffEffect::CDarkDebuffEffect(CDarkDebuffEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CDarkDebuffEffect::~CDarkDebuffEffect()
{
}

bool CDarkDebuffEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("DarkDebuffMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("DarkDebuffAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->AddAnimation("DarkDebuff");
		Anim->SetLoop("DarkDebuff", true);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(48.f, 55.f);
		Mesh->SetRenderLayer("DeBuff");
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}


void CDarkDebuffEffect::Resume()
{
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetEnable(true);
	}
}
void CDarkDebuffEffect::Pause()
{

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetEnable(false);
	}
}
void CDarkDebuffEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CDarkDebuffEffect* CDarkDebuffEffect::Clone()
{
	return new CDarkDebuffEffect(*this);
}

