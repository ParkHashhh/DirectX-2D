#include "FireBallEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CFireBallEffect::CFireBallEffect()
{
	SetClassType<CFireBallEffect>();
}

CFireBallEffect::CFireBallEffect(const CFireBallEffect& ref) :
	CGameObject(ref)
{
}

CFireBallEffect::CFireBallEffect(CFireBallEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CFireBallEffect::~CFireBallEffect()
{
}

bool CFireBallEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("FireBallEffect");
		Anim->SetPlayRate("FireBallEffect", 10.f);
		Anim->SetFinishNotify<CFireBallEffect>("FireBallEffect",
			this, &CFireBallEffect::FinishAnimation);
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

void CFireBallEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CFireBallEffect* CFireBallEffect::Clone()
{
	return new CFireBallEffect(*this);
}

void CFireBallEffect::FinishAnimation()
{
	Destroy();
}
