#include "SparkEffect.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CSparkEffect::CSparkEffect()
{
	SetClassType<CSparkEffect>();
}

CSparkEffect::CSparkEffect(const CSparkEffect& ref) :
	CGameObject(ref)
{
}

CSparkEffect::CSparkEffect(CSparkEffect&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CSparkEffect::~CSparkEffect()
{
}

bool CSparkEffect::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("Spark");
		Anim->SetPlayRate("Spark", 5.f);
		Anim->SetFinishNotify<CSparkEffect>("Spark",
			this, &CSparkEffect::FinishAnimation);
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

void CSparkEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CSparkEffect* CSparkEffect::Clone()
{
	return new CSparkEffect(*this);
}

void CSparkEffect::FinishAnimation()
{
	Destroy();
}
