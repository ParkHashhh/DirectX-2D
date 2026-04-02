#pragma once

#include "Object/GameObject.h"

class CFieldItem :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFieldItem();
	CFieldItem(const CFieldItem& ref);
	CFieldItem(CFieldItem&& ref)	noexcept;

public:
	virtual ~CFieldItem();

private:

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;

	std::shared_ptr<class CItem> mItem;
	std::shared_ptr<class CRayTracing> mRay;

	bool mIsOnGround = false;
	bool mIsDrop = false;
	bool mIsRecall = false;
	float mFloatingTime = 0.5f;
	class CCollider* DestCollider = nullptr;
	FVector3 DropDir = FVector3::Zero;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	std::shared_ptr<class CItem> GetItem()
	{
		return mItem;
	}

	void RayCollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void RayCollisionEnd(class CCollider* Dest);
	void SetItem(std::shared_ptr<class CItem> Item);

	void DropInventory();
	void StartDrop();
	void StartDropArray(float X,float Y);
	void CreateZakum(float DeltaTime);
	void DefaultMove(float DeltaTime);
	void AddInventory(float DeltaTime);
	void DropItems(float DeltaTime);
};

