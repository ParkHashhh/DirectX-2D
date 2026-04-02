#include "DestinationStone.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "../../Player/RayTracing.h"
#include "World/World.h"
#include "FallingStone.h"

CDestinationStone::CDestinationStone()
{
	SetClassType<CDestinationStone>();
}

CDestinationStone::CDestinationStone(const CDestinationStone& ref) :
	CGameObject(ref)
{
}

CDestinationStone::CDestinationStone(CDestinationStone&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CDestinationStone::~CDestinationStone()
{
}

bool CDestinationStone::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("DestinationStoneMesh" + std::to_string(rand()));
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("DestinationStoneAnimation2D" + std::to_string(rand()));

	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->AddAnimation("DestinationStone");
		Anim->SetFinishNotify<CDestinationStone>("DestinationStone",
			this, &CDestinationStone::FinishAnimation);
		
		Anim->AddNotify<CDestinationStone>("DestinationStone",
			"DestinationStone",7, this, &CDestinationStone::CreateFallingStone);

	}
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(200.f,300.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
	}
	return true;
}

void CDestinationStone::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}


CDestinationStone* CDestinationStone::Clone()
{
	return new CDestinationStone(*this);
}
void CDestinationStone::CreateFallingStone()
{
	auto World = mWorld.lock();
	if (World)
	{

		auto _Stone = World->CreateGameObject<CFallingStone>("Stone" + std::to_string(rand()));
		auto Stone = _Stone.lock();
		Stone->SetTargetPos(mTargetPos);
		Stone->SetWorldPos(GetWorldPos().x, 400.f);
		
	}
}
void CDestinationStone::FinishAnimation()
{
	Destroy();
}
