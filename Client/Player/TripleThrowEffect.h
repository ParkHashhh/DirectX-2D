#pragma once

#include "Object/GameObject.h"

class CTripleThrowEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CTripleThrowEffect();
	CTripleThrowEffect(const CTripleThrowEffect& ref);
	CTripleThrowEffect(CTripleThrowEffect&& ref)	noexcept;

public:
	virtual ~CTripleThrowEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymettry();
protected:
	virtual CTripleThrowEffect* Clone();

private:
	void FinishAnimation();
};

