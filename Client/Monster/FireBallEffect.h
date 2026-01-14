#pragma once
#include "Object/GameObject.h"
class CFireBallEffect :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFireBallEffect();
	CFireBallEffect(const CFireBallEffect& ref);
	CFireBallEffect(CFireBallEffect&& ref)	noexcept;

public:
	virtual ~CFireBallEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CFireBallEffect* Clone();

private:
	void FinishAnimation();
};

