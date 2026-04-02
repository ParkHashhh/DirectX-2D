#include "TripleThrowEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CTripleThrowEffect::CTripleThrowEffect()
{
	SetClassType<CTripleThrowEffect>();
}

CTripleThrowEffect::CTripleThrowEffect(const CTripleThrowEffect& ref) :
	CGameObject(ref)
{
}

CTripleThrowEffect::CTripleThrowEffect(CTripleThrowEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CTripleThrowEffect::~CTripleThrowEffect()
{
}

bool CTripleThrowEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("TripleThrowEffect");
		Anim->SetPlayRate("TripleThrowEffect", 1.7f);
		Anim->SetFinishNotify<CTripleThrowEffect>("TripleThrowEffect",
			this, &CTripleThrowEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(80.f, 80.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

void CTripleThrowEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTripleThrowEffect::SetSymettry()
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("TripleThrowEffect", true);
	}
}

CTripleThrowEffect* CTripleThrowEffect::Clone()
{
	return new CTripleThrowEffect(*this);
}

void CTripleThrowEffect::FinishAnimation()
{
	Destroy();
}
