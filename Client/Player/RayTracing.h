#pragma once

#include "Object/GameObject.h"

class CRayTracing :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CRayTracing();
	CRayTracing(const CRayTracing& ref);
	CRayTracing(CRayTracing&& ref)	noexcept;

public:
	virtual ~CRayTracing();

private:
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::string mCollsionName;

public:
	void SetEnable(bool Enable);
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void SetCollisionName(const std::string& Name)
	{
		mCollsionName = Name;
	}
	const std::string& GetCollisionName()
	{
		return mCollsionName;
	}
	void ClearCollision();
	void SetBoxSize(float X,float Y);
	std::weak_ptr<class CColliderBox2D> GetBox2D()
	{
		return mBody;
	}

};

