#include "Grave.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"

CGrave::CGrave()
{
	SetClassType<CGrave>();
}

CGrave::CGrave(const CGrave& ref) :
	CGameObject(ref)
{
}

CGrave::CGrave(CGrave&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CGrave::~CGrave()
{
}

bool CGrave::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("GraveMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("GraveAnimation2D");

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(50.f, 50.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
		Mesh->SetRenderLayer(0);
	}
	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("Grave");
		Anim->SetPlayRate("Grave", 1.f);
	}


	

	return true;
}

void CGrave::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CGrave::SetSymmetry(bool Symmetry)
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetSymmetry("Grave", Symmetry);
	}
}

CGrave* CGrave::Clone()
{
	return new CGrave(*this);
}

void CGrave::FinishAnimation()
{
	Destroy();
}
