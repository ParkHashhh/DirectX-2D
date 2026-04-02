#pragma once

#include "Object/GameObject.h"

class CBossGround :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CBossGround();
	CBossGround(const CBossGround& ref);
	CBossGround(CBossGround&& ref)	noexcept;

public:
	virtual ~CBossGround();

private:
	std::weak_ptr<class CColliderBox2D>	mBody;

	bool mIsRecall = false;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	void OnHit(const FVector3& HitPoint, class CCollider* Dest);

private:
	void FinishAnimation();
};

