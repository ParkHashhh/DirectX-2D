#pragma once

#include "Object/GameObject.h"

class CNpc :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CNpc();
	CNpc(const CNpc& ref);
	CNpc(CNpc&& ref)	noexcept;

public:
	virtual ~CNpc();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;
	
	bool mIsMouseHoverd = false;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);

protected:
	virtual CNpc* Clone();

private:
	void MouseCollisionBegin(const FVector3& HitPoint);
	void MouseCollisionEnd(const FVector3& HitPoint);
};

