#include "EndingObject.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Engine.h"
CEndingObject::CEndingObject()
{
	SetClassType<CEndingObject>();
}

CEndingObject::CEndingObject(const CEndingObject& ref) :
	CGameObject(ref)
{
}

CEndingObject::CEndingObject(CEndingObject&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CEndingObject::~CEndingObject()
{
}

bool CEndingObject::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("EndingCreditMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("EndingCreditAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(4, 4.f);
		Mesh->AddWorldPos(-0.13f, 0.18f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
		Mesh->SetRenderLayer(0);
	}
	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("EndingCredit");
		Anim->SetPlayRate("EndingCredit", 0.05f);
		Anim->SetFinishNotify<CEndingObject>("EndingCredit",
			this, &CEndingObject::FinishAnimation);
	}

	return true;
}

void CEndingObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEndingObject::FinishAnimation()
{
	CEngine::GetInst()->Destroy();
}
