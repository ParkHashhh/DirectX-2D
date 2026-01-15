#include "SpeedUp.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "Component/ColliderSphere2D.h"
#include "../Player/Player.h"
CSpeedUp::CSpeedUp()
{
}

CSpeedUp::CSpeedUp(const CSpeedUp& ref)
	: CItem(ref)
{
}

CSpeedUp::CSpeedUp(CSpeedUp&& ref) noexcept
	: CItem(std::move(ref))
{
}

CSpeedUp::~CSpeedUp()
{
}

void CSpeedUp::GetItem(std::weak_ptr<CPlayer> Player)
{
	auto sPlayer = Player.lock();
	if (sPlayer)
	{
		sPlayer->AddSpeed(50.f);
	}
}

void CSpeedUp::Update(float DeltaTime)
{

}
bool CSpeedUp::Init()
{
	CItem::Init();
	mMesh = FindComponent<CMeshComponent>("ItemMesh");
	mBody = FindComponent<CColliderSphere2D>("ItemBody");
	mItemType = EItemType::SpeedUp;
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	//텍스쳐적용
	auto	Mesh = mMesh.lock();
	if (Mesh)
	{
		auto Tex = AssetMng->FindTexture("SpeedUp");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, 255, 255, 255, 0);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRelativeScale(25, 25);
	}

	return true;
}

