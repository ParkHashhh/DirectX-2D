#pragma once

#include "Object/GameObject.h"

class CPoison :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CPoison();
	CPoison(const CPoison& ref);
	CPoison(CPoison&& ref)	noexcept;

public:
	virtual ~CPoison();

private:

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;

	float mPoisonTime = 5.f;
	float mPoisonTickTime = 0.01f;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CPoison* Clone();
	void OnHit(const FVector3& HitPoint, class CCollider* Dest);
};

