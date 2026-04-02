#pragma once

#include "Object/GameObject.h"

class CShadowEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CShadowEffect();
	CShadowEffect(const CShadowEffect& ref);
	CShadowEffect(CShadowEffect&& ref)	noexcept;

public:
	virtual ~CShadowEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CShadowEffect* Clone();

private:
	void FinishAnimation();
};

