#include "Npc.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../UI/NpcWidget.h"


CNpc::CNpc()
{
	SetClassType<CNpc>();
}

CNpc::CNpc(const CNpc& ref) :
	CGameObject(ref)
{
}

CNpc::CNpc(CNpc&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CNpc::~CNpc()
{
}


bool CNpc::Init()
{
	CGameObject::Init();

	auto World = mWorld.lock();
	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	auto AssetMng = World->GetWorldAssetManager().lock();
	mBody = CreateComponent<CColliderBox2D>("Body");
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(50.f, 80.f);
	}

	auto	Body = mBody.lock();
	if (Body)
	{
		Body->SetCollisionMouseBeginFunction<CNpc>(this, &CNpc::MouseCollisionBegin);
		Body->SetCollisionMouseEndFunction<CNpc>(this, &CNpc::MouseCollisionEnd);
		Body->SetCollisionProfile("Player");
		Body->SetBoxSize(50.f, 80.f);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
	}
	return true;
}

void CNpc::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	auto World = mWorld.lock();
	auto Input = World->GetInput().lock();
	
	if (Input)
	{
		if (mIsMouseHoverd)
		{
			if (Input->GetMouseState(EMouseType::LButton, EInputType::Press))
			{
				// CreateWidget 
				std::weak_ptr<CNpcWidget>	EndingWidget =
					World->GetUIManager().lock()->CreateWidget<CNpcWidget>("NpcWidget");
			}
		}
	}

}

void CNpc::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNpc* CNpc::Clone()
{
	return new CNpc(*this);
}

void CNpc::MouseCollisionBegin(const FVector3& HitPoint)
{
	mIsMouseHoverd = true;
}

void CNpc::MouseCollisionEnd(const FVector3& HitPoint)
{
	mIsMouseHoverd = false;
}
