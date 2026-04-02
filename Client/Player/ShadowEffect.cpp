#include "ShadowEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CShadowEffect::CShadowEffect()
{
	SetClassType<CShadowEffect>();
}

CShadowEffect::CShadowEffect(const CShadowEffect& ref) :
	CGameObject(ref)
{
}

CShadowEffect::CShadowEffect(CShadowEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CShadowEffect::~CShadowEffect()
{
}

bool CShadowEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("ShadowMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ShadowAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("ShadowEffect");
		Anim->SetPlayRate("ShadowEffect", 1.f);
		Anim->SetFinishNotify<CShadowEffect>("ShadowEffect",
			this, &CShadowEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(150.f, 150.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
		Mesh->SetRenderLayer(0);
	}

	return true;
}

void CShadowEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CShadowEffect::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("ShadowEffect", Symmetry);
	}
}

CShadowEffect* CShadowEffect::Clone()
{
	return new CShadowEffect(*this);
}

void CShadowEffect::FinishAnimation()
{
	Destroy();
}
