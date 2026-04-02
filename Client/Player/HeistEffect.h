#pragma once

#include "Object/GameObject.h"

class CHeistEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CHeistEffect();
	CHeistEffect(const CHeistEffect& ref);
	CHeistEffect(CHeistEffect&& ref)	noexcept;

public:
	virtual ~CHeistEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CHeistEffect* Clone();

private:
	void FinishAnimation();
};

