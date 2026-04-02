#pragma once

#include "Object/GameObject.h"

class CShadow :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CShadow();
	CShadow(const CShadow& ref);
	CShadow(CShadow&& ref)	noexcept;

public:
	virtual ~CShadow();

private:
	std::weak_ptr<class CMeshComponent>		mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::vector<std::string> mAttackAnimList;
	bool mIsHorntailWorld = false;
	void AnimStart();
	void AnimEnd();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetHorntailWorld(bool HornTailWorld);
	std::weak_ptr<class CAnimation2DComponent> GetShadowAnim()
	{
		return mAnimation2DComponent;
	}
	void SetOpacity(float Opacity);
	void IdleNotify();

	std::string GetAttackAnimName(int Index)
	{
		return mAttackAnimList[Index];
	}
};

