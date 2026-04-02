#pragma once

#include "Object/GameObject.h"

class CJumpEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CJumpEffect();
	CJumpEffect(const CJumpEffect& ref);
	CJumpEffect(CJumpEffect&& ref)	noexcept;

public:
	virtual ~CJumpEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CJumpEffect* Clone();

private:
	void FinishAnimation();
};

