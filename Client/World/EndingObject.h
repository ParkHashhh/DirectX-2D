#pragma once

#include "Object/GameObject.h"

class CEndingObject :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CEndingObject();
	CEndingObject(const CEndingObject& ref);
	CEndingObject(CEndingObject&& ref)	noexcept;

public:
	virtual ~CEndingObject();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void FinishAnimation();
};

