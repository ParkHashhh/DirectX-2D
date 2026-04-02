#include "BoostEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CBoostEffect::CBoostEffect()
{
	SetClassType<CBoostEffect>();
}

CBoostEffect::CBoostEffect(const CBoostEffect& ref) :
	CGameObject(ref)
{
}

CBoostEffect::CBoostEffect(CBoostEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CBoostEffect::~CBoostEffect()
{
}

bool CBoostEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("BoostMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("BoostAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("BoostEffect");
		Anim->SetPlayRate("BoostEffect", 1.f);
		Anim->SetFinishNotify<CBoostEffect>("BoostEffect",
			this, &CBoostEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(79.f, 34.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer("Player");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
	}

	return true;
}

void CBoostEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}


CBoostEffect* CBoostEffect::Clone()
{
	return new CBoostEffect(*this);
}

void CBoostEffect::FinishAnimation()
{
	Destroy();
}
