#include "ShadowEffect2.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CShadowEffect2::CShadowEffect2()
{
	SetClassType<CShadowEffect2>();
}

CShadowEffect2::CShadowEffect2(const CShadowEffect2& ref) :
	CGameObject(ref)
{
}

CShadowEffect2::CShadowEffect2(CShadowEffect2&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CShadowEffect2::~CShadowEffect2()
{
}

bool CShadowEffect2::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Shadow2Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Shadow2Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("ShadowEffect2");
		Anim->SetPlayRate("ShadowEffect2", 1.f);
		Anim->SetFinishNotify<CShadowEffect2>("ShadowEffect2",
			this, &CShadowEffect2::FinishAnimation);
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

void CShadowEffect2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CShadowEffect2::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("ShadowEffect2", Symmetry);
	}
}

CShadowEffect2* CShadowEffect2::Clone()
{
	return new CShadowEffect2(*this);
}

void CShadowEffect2::FinishAnimation()
{
	Destroy();
}
