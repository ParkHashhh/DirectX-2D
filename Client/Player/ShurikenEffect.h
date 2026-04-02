#pragma once

#include "Object/GameObject.h"

class CShurikenEffect :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CShurikenEffect();
	CShurikenEffect(const CShurikenEffect& ref);
	CShurikenEffect(CShurikenEffect&& ref)	noexcept;

public:
	virtual ~CShurikenEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CShurikenEffect* Clone();

private:
	void FinishAnimation();
};

