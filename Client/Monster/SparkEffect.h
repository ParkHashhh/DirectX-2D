#pragma once
#include "Object/GameObject.h"

class CSparkEffect :

    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CSparkEffect();
	CSparkEffect(const CSparkEffect& ref);
	CSparkEffect(CSparkEffect&& ref)	noexcept;

public:
	virtual ~CSparkEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CSparkEffect* Clone();

private:
	void FinishAnimation();
};


