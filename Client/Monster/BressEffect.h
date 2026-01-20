#pragma once
#include "Object/GameObject.h"

class CBressEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CBressEffect();
	CBressEffect(const CBressEffect& ref);
	CBressEffect(CBressEffect&& ref)	noexcept;

public:
	virtual ~CBressEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	virtual CBressEffect* Clone();

private:
	void FinishAnimation();
};

