#include "ArrowForce.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "Component/ColliderSphere2D.h"
#include "../Player/Player.h"
CArrowForce::CArrowForce()
{
}

CArrowForce::CArrowForce(const CArrowForce& ref)
	: CItem(ref)
{
}

CArrowForce::CArrowForce(CArrowForce&& ref) noexcept
	: CItem(std::move(ref))
{
}

CArrowForce::~CArrowForce()
{
}

void CArrowForce::GetItem(std::weak_ptr<CPlayer> Player)
{
	auto sPlayer = Player.lock();
	if (sPlayer)
	{
		sPlayer->AddArrow(1);
	}
}

void CArrowForce::Update(float DeltaTime)
{

}
bool CArrowForce::Init()
{
	CItem::Init();
	mMesh = FindComponent<CMeshComponent>("ItemMesh");
	mBody = FindComponent<CColliderSphere2D>("ItemBody");
	mItemType = EItemType::ArrowForce;
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	//텍스쳐적용
	auto	Mesh = mMesh.lock();
	auto	Body = mBody.lock();
	if (Mesh)
	{
		auto Tex = AssetMng->FindTexture("ArrowForce");
		Mesh->SetRelativeScale(25.f, 25.f);
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, 255, 255, 255, 0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	return true;
}

