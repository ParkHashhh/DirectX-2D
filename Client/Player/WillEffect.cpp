#include "WillEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CWillEffect::CWillEffect()
{
	SetClassType<CWillEffect>();
}

CWillEffect::CWillEffect(const CWillEffect& ref) :
	CGameObject(ref)
{
}

CWillEffect::CWillEffect(CWillEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CWillEffect::~CWillEffect()
{
}

bool CWillEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("WillMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("WillAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("WillEffect");
		Anim->SetPlayRate("WillEffect", 1.f);
		Anim->SetFinishNotify<CWillEffect>("WillEffect",
			this, &CWillEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(250, 422);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

void CWillEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWillEffect::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("JumpEffect", Symmetry);
	}
}

CWillEffect* CWillEffect::Clone()
{
	return new CWillEffect(*this);
}

void CWillEffect::FinishAnimation()
{
	Destroy();
}
