#pragma once

#include "Object/GameObject.h"

class CBoostEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CBoostEffect();
	CBoostEffect(const CBoostEffect& ref);
	CBoostEffect(CBoostEffect&& ref)	noexcept;

public:
	virtual ~CBoostEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CBoostEffect* Clone();

private:
	void FinishAnimation();
};

