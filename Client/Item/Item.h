#pragma once
#include "Object/GameObject.h"

class CItem :

    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CItem();
	CItem(const CItem& ref);
	CItem(CItem&& ref)	noexcept;
	std::weak_ptr<class CMeshComponent> mMesh;
	std::weak_ptr<class CColliderSphere2D> mBody;

public:
	virtual ~CItem();


	virtual bool Init() = 0;
	virtual void GetItem(std::weak_ptr<class CPlayer> Player) = 0;
	virtual void Update(float DeltaTime);
	virtual void CollisionBegin(const FVector3& HitPoint, class CCollider* Dest);
protected:
	EItemType mItemType;
};

