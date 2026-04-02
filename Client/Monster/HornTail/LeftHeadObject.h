#pragma once

#include "Object/GameObject.h"

class CLeftHeadObject :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CLeftHeadObject();
	CLeftHeadObject(const CLeftHeadObject& ref);
	CLeftHeadObject(CLeftHeadObject&& ref)	noexcept;

public:
	virtual ~CLeftHeadObject();

private:

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CLeftHeadObject* Clone();
	void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	void EnableBody();
	void DisableBody();

private:
	void FinishAnimation();
};

