#pragma once

#include "Object/GameObject.h"

class CShadowEffect2 :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CShadowEffect2();
	CShadowEffect2(const CShadowEffect2& ref);
	CShadowEffect2(CShadowEffect2&& ref)	noexcept;

public:
	virtual ~CShadowEffect2();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CShadowEffect2* Clone();

private:
	void FinishAnimation();
};

