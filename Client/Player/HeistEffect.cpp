#include "HeistEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CHeistEffect::CHeistEffect()
{
	SetClassType<CHeistEffect>();
}

CHeistEffect::CHeistEffect(const CHeistEffect& ref) :
	CGameObject(ref)
{
}

CHeistEffect::CHeistEffect(CHeistEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CHeistEffect::~CHeistEffect()
{
}

bool CHeistEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HeistMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HeistAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HeistEffect");
		Anim->SetPlayRate("HeistEffect", 1.f);
		Anim->SetFinishNotify<CHeistEffect>("HeistEffect",
			this, &CHeistEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(100.f, 100.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRenderLayer("Player");
	}

	return true;
}

void CHeistEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CHeistEffect::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("JumpEffect", Symmetry);
	}
}

CHeistEffect* CHeistEffect::Clone()
{
	return new CHeistEffect(*this);
}

void CHeistEffect::FinishAnimation()
{
	Destroy();
}
