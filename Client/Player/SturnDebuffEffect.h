#pragma once

#include "Object/GameObject.h"

class CSturnDebuffEffect :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CSturnDebuffEffect();
	CSturnDebuffEffect(const CSturnDebuffEffect& ref);
	CSturnDebuffEffect(CSturnDebuffEffect&& ref)	noexcept;

public:
	virtual ~CSturnDebuffEffect();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

	void Resume();
	void Pause();


protected:
	virtual CSturnDebuffEffect* Clone();
};

