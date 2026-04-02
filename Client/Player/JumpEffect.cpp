#include "JumpEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CJumpEffect::CJumpEffect()
{
	SetClassType<CJumpEffect>();
}

CJumpEffect::CJumpEffect(const CJumpEffect& ref) :
	CGameObject(ref)
{
}

CJumpEffect::CJumpEffect(CJumpEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CJumpEffect::~CJumpEffect()
{
}

bool CJumpEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("DashMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("DashAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("JumpEffect");
		Anim->SetPlayRate("JumpEffect", 1.5f);
		Anim->SetFinishNotify<CJumpEffect>("JumpEffect",
			this, &CJumpEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(100.f, 100.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

void CJumpEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CJumpEffect::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("JumpEffect",Symmetry);
	}
}

CJumpEffect* CJumpEffect::Clone()
{
	return new CJumpEffect(*this);
}

void CJumpEffect::FinishAnimation()
{
	Destroy();
}
