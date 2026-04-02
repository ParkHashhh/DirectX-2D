#include "ShurikenEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CShurikenEffect::CShurikenEffect()
{
	SetClassType<CShurikenEffect>();
}

CShurikenEffect::CShurikenEffect(const CShurikenEffect& ref) :
	CGameObject(ref)
{
}

CShurikenEffect::CShurikenEffect(CShurikenEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CShurikenEffect::~CShurikenEffect()
{
}

bool CShurikenEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HitEffect");
		Anim->SetPlayRate("HitEffect", 1.7f);
		Anim->SetFinishNotify<CShurikenEffect>("HitEffect",
			this, &CShurikenEffect::FinishAnimation);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(50.f,50.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

void CShurikenEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CShurikenEffect* CShurikenEffect::Clone()
{
	return new CShurikenEffect(*this);
}

void CShurikenEffect::FinishAnimation()
{
	Destroy();
}
