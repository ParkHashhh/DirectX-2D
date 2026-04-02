#pragma once

#include "Object/GameObject.h"

class CDarkDebuffEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CDarkDebuffEffect();
	CDarkDebuffEffect(const CDarkDebuffEffect& ref);
	CDarkDebuffEffect(CDarkDebuffEffect&& ref)	noexcept;

public:
	virtual ~CDarkDebuffEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

	void Resume();
	void Pause();


protected:
	virtual CDarkDebuffEffect* Clone();
};

