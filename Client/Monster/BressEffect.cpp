#include "BressEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CBressEffect::CBressEffect()
{
	SetClassType<CBressEffect>();
}

CBressEffect::CBressEffect(const CBressEffect& ref) :
	CGameObject(ref)
{
}

CBressEffect::CBressEffect(CBressEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CBressEffect::~CBressEffect()
{
}

bool CBressEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("BressEffect");
		Anim->SetPlayTime("BressEffect", 5.f);
		Anim->SetFinishNotify<CBressEffect>("BressEffect",
			this, &CBressEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(600.f, 1000.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

void CBressEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CBressEffect* CBressEffect::Clone()
{
	return new CBressEffect(*this);
}

void CBressEffect::FinishAnimation()
{
	Destroy();
}