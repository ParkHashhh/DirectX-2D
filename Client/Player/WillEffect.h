#pragma once

#include "Object/GameObject.h"

class CWillEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CWillEffect();
	CWillEffect(const CWillEffect& ref);
	CWillEffect(CWillEffect&& ref)	noexcept;

public:
	virtual ~CWillEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CWillEffect* Clone();

private:
	void FinishAnimation();
};

