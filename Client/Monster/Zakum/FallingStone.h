#pragma once

#include "Object/GameObject.h"

class CFallingStone :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFallingStone();
	CFallingStone(const CFallingStone& ref);
	CFallingStone(CFallingStone&& ref)	noexcept;

public:
	virtual ~CFallingStone();

private:

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;

	float mSpeed = 400.f;
	FVector3 mTargetPos;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetTargetPos(FVector3 Pos)
	{
		mTargetPos = Pos;
	}

protected:
	virtual CFallingStone* Clone();
	void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	void Pause();

private:
	void FinishAnimation();
};

