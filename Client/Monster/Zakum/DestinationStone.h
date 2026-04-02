#pragma once

#include "Object/GameObject.h"

class CDestinationStone :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CDestinationStone();
	CDestinationStone(const CDestinationStone& ref);
	CDestinationStone(CDestinationStone&& ref)	noexcept;

public:
	virtual ~CDestinationStone();

private:

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

	FVector3 mTargetPos;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetTargetPos(FVector3 Pos)
	{
		mTargetPos = Pos;
	}

protected:
	virtual CDestinationStone* Clone();

private:
	void FinishAnimation();
	void CreateFallingStone();
};

