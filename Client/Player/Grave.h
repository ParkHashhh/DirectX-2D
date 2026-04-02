#pragma once

#include "Object/GameObject.h"

class CGrave :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CGrave();
	CGrave(const CGrave& ref);
	CGrave(CGrave&& ref)	noexcept;

public:
	virtual ~CGrave();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CGrave* Clone();

private:
	void FinishAnimation();
};

