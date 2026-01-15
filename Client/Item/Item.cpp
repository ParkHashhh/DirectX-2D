#include "Item.h"
#include "Component/Collider.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderSphere2D.h"
#include "../Player/Player.h"
CItem::CItem()
{
}

CItem::CItem(const CItem& ref)
	:	CGameObject(ref)
{

}

CItem::CItem(CItem&& ref) noexcept
	: CGameObject(std::move(ref))
{
}

CItem::~CItem()
{
}

void CItem::GetItem(std::weak_ptr<CPlayer> Player)
{

}

void CItem::Update(float DeltaTime)
{

}
bool CItem::Init()
{
	if (!CGameObject::Init())
		return false;
	mMesh = CreateComponent<CMeshComponent>("ItemMesh");
	mBody = CreateComponent<CColliderSphere2D>("ItemBody");

	auto Mesh = mMesh.lock();
	auto Body = mBody.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetRelativeScale(50.f, 50.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	if (Body)
	{
		Body->SetCollisionProfile("Item");
		Body->SetRadius(15);
		Body->SetCollisionBeginFunction<CItem>(this, &CItem::CollisionBegin);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
		Body->SetInheritRot(false);
	}

	return true;
}
void CItem::CollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
    auto Profile = Dest->GetCollisionProfile();
    if (Profile->Name == "Player")
    {
        auto Player = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Player)
		{
			GetItem(Player);
			Destroy();
		}
    }
}