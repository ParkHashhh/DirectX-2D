#pragma once
#include "Object/GameObject.h"

class CBallockEffect :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CBallockEffect();
	CBallockEffect(const CBallockEffect& ref);
	CBallockEffect(CBallockEffect&& ref)	noexcept;

public:
	virtual ~CBallockEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CBallockEffect* Clone();

private:
	void FinishAnimation();
};

