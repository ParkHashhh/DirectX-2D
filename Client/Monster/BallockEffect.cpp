#include "BallockEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CBallockEffect::CBallockEffect()
{
	SetClassType<CBallockEffect>();
}

CBallockEffect::CBallockEffect(const CBallockEffect& ref) :
	CGameObject(ref)
{
}

CBallockEffect::CBallockEffect(CBallockEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CBallockEffect::~CBallockEffect()
{
}

bool CBallockEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("BallockEffect");
		Anim->SetFinishNotify<CBallockEffect>("BallockEffect",
			this, &CBallockEffect::FinishAnimation);
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

void CBallockEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CBallockEffect* CBallockEffect::Clone()
{
	return new CBallockEffect(*this);
}

void CBallockEffect::FinishAnimation()
{
	Destroy();
}