#include "HpPotion.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "Component/ColliderSphere2D.h"
#include "../Player/Player.h"
CHpPotion::CHpPotion()
{
}

CHpPotion::CHpPotion(const CHpPotion& ref)
	: CItem(ref)
{
}

CHpPotion::CHpPotion(CHpPotion&& ref) noexcept
	: CItem(std::move(ref))
{
}

CHpPotion::~CHpPotion()
{
}

void CHpPotion::GetItem(std::weak_ptr<CPlayer> Player)
{
	auto sPlayer = Player.lock();
	if (sPlayer)
	{
		sPlayer->AddHP(1);
	}
}

void CHpPotion::Update(float DeltaTime)
{

}
bool CHpPotion::Init()
{
	CItem::Init();
	mMesh = FindComponent<CMeshComponent>("ItemMesh");
	mBody = FindComponent<CColliderSphere2D>("ItemBody");
	mItemType = EItemType::HpPotion;

	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	//텍스쳐적용
	auto	Mesh = mMesh.lock();
	if (Mesh)
	{
		auto Tex = AssetMng->FindTexture("HpPotion");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, 255, 255, 255, 0);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRelativeScale(100, 60);
	}

	return true;
}

