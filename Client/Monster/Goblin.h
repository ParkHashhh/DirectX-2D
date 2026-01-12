#pragma once
#include "Monster.h"

class CGoblin :
    public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CGoblin();
	CGoblin(const CGoblin& ref);
	CGoblin(CGoblin&& ref)	noexcept;
	float mDefaultSpeed = 20.f;
public:
	virtual ~CGoblin();

protected:
	virtual CGoblin* Clone();
public:
	virtual float GetDefaultSpeed()
	{
		return mDefaultSpeed;
	}
	virtual bool Init();
	virtual void AttackNotify();
	virtual void AttackFinish();
	virtual void SetMonsterData();

};

