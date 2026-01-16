#pragma once
#include "Monster.h"

class CBallock :
    public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CBallock();
	CBallock(const CBallock& ref);
	CBallock(CBallock&& ref)	noexcept;
	float mDefaultSpeed = 80.f;

	std::weak_ptr<class CFireBall> mFireBall;

public:
	virtual ~CBallock();
protected:
	virtual CBallock* Clone();

public:
	virtual void OnHit();
	void ThrowNotify();
	virtual float GetDefaultSpeed()
	{
		return mDefaultSpeed;
	}
	virtual bool Init();
	virtual void AttackNotify();
	virtual void AttackFinish();
	virtual void SetMonsterData();
};

