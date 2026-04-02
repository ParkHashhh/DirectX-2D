#include "FieldItem.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderLine2D.h"
#include "World/World.h"
#include "Item.h"
#include "../Player/RayTracing.h"
#include "../Monster/Zakum/Zakum.h"
CFieldItem::CFieldItem()
{
	SetClassType<CFieldItem>();
}

CFieldItem::CFieldItem(const CFieldItem& ref) :
	CGameObject(ref)
{
}

CFieldItem::CFieldItem(CFieldItem&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CFieldItem::~CFieldItem()
{
}

bool CFieldItem::Init()
{
	CGameObject::Init();
	

	mMeshComponent = CreateComponent<CMeshComponent>("DropItemMesh");
	mBody = CreateComponent<CColliderBox2D>("DropItemBody");

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(40.f, 40.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
		Mesh->SetRenderLayer("DeBuff");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(40.f, 40.f);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Item");
		Body->SetEnable(true);
	}
	auto World = mWorld.lock();
	if (World)
	{
		mRay = World->CreateGameObject<CRayTracing>("ItemRay" + std::to_string(rand())).lock();
		auto RayBox = mRay->GetBox2D().lock();
		if (RayBox)
		{
			Mesh->AddChild(RayBox);
			RayBox->SetRelativePos(0, 20.f);
			RayBox->SetBoxSize(20.f, 5.f);
			RayBox->SetInheritScale(false);
			RayBox->SetCollisionBeginFunction<CFieldItem>(this, &CFieldItem::RayCollisionBegin);
			RayBox->SetCollisionEndFunction<CFieldItem>(this, &CFieldItem::RayCollisionEnd);
			RayBox->SetRelativePos(0, -20);
			RayBox->SetDebugDraw(false);
			RayBox->SetEnable(false);
		}
	}
	return true;
}
void CFieldItem::DropInventory()
{
	mIsOnGround = false;
	DropDir.y = 200.f;
	mIsDrop = true;
}
void CFieldItem::StartDrop()
{
	mIsOnGround = false;
	DropDir.x = 0.f;
	DropDir.y = 500.f;
}

void CFieldItem::StartDropArray(float X, float Y)
{
	mIsOnGround = false;
	DropDir.x = X;
	DropDir.y = Y;
	
}

void CFieldItem::CreateZakum(float DeltaTime)
{
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		float Opacity = Mesh->GetMaterialOpacity(0);
		Opacity -= 0.3f * DeltaTime;
		Mesh->SetMaterialOpacity(0, Opacity);
		if (Opacity <= 0)
		{
			std::weak_ptr<CZakum>	Zakum = mWorld.lock()->CreateGameObject<CZakum>("Zakum");
			if (DestCollider)
			{
				auto DestOwner = DestCollider->GetOwner().lock();
				if (DestOwner)
					DestOwner->Destroy();
				Destroy();
			}
		}
	}
}

void CFieldItem::DefaultMove(float DeltaTime)
{
	mFloatingTime += DeltaTime;
	float OffsetY = sinf(mFloatingTime * 2.f) * 5.f;
	AddWorldPos(0, OffsetY * DeltaTime, 0);
}

void CFieldItem::AddInventory(float DeltaTime)
{
	DropDir.y -= 1700 * DeltaTime;
	AddWorldPos(0, DropDir.y * DeltaTime);
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		float Opacity = Mesh->GetMaterialOpacity(0);
		Opacity -= 3 * DeltaTime;
		Mesh->SetMaterialOpacity(0, Opacity);
	}
	if (DropDir.y <= 0.f)
	{
		Destroy();
	}
}

void CFieldItem::DropItems(float DeltaTime)
{
	DropDir.y -= 1700 * DeltaTime;
	AddWorldPos(DropDir * DeltaTime);
	AddWorldRotationZ(720 * DeltaTime);

	if (DropDir.y <= 0.f)
	{
		auto RayBody = mRay->GetBox2D().lock();
		if (RayBody)
			RayBody->SetEnable(true);
	}
}


void CFieldItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	
	if (mIsOnGround)
	{
		DropDir.y = 0.f;
		auto RayBody = mRay->GetBox2D().lock();
		if (RayBody)
			RayBody->SetEnable(false);
		if (mIsRecall)
		{
			CreateZakum(DeltaTime);
		}
		else
		{
			DefaultMove(DeltaTime);
		}
	}
	else
	{
		if (!mIsDrop)
		{
			DropItems(DeltaTime);
		}
		else
		{
			AddInventory(DeltaTime);
		}
	}
}


void CFieldItem::SetItem(std::shared_ptr<class CItem> Item)
{
	mItem = Item;
	auto Mesh = mMeshComponent.lock();
	auto Data = mItem->GetData();
	if (Mesh)
	{
		Mesh->AddTextureFullPath(0, Data.Name, Data.IconPath.c_str());
	}
}

void CFieldItem::RayCollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	mBody.lock()->SetEnable(true);
	if (Dest->GetCollisionProfile()->Name == "Gravity")
	{
		mIsOnGround = true;
		DropDir.y = 0.f;
		SetWorldRotationZ(0);
		auto Body = mRay->GetBox2D().lock();
		if (Body)
			Body->SetEnable(false);
	}
	else if (Dest->GetCollisionProfile()->Name == "BossGround")
	{
		mIsOnGround = true;
		DropDir.y = 0.f;
		SetWorldRotationZ(0);
		auto Body = mRay->GetBox2D().lock();
		if (Body)
			Body->SetEnable(false);
		if (mItem)
		{
			if (mItem->GetData().Name == "FireEye")
			{
				DestCollider = Dest;
				mIsRecall = true;
			}
		}
	}
}

void CFieldItem::RayCollisionEnd(CCollider* Dest)
{

}
