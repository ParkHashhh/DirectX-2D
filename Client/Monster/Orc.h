#pragma once
#include "Monster.h"
class COrc :
    public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	COrc();
	COrc(const COrc& ref);
	COrc(COrc&& ref)	noexcept;
	float mDefaultSpeed = 50.f;



public:
	virtual ~COrc();
protected:
	virtual COrc* Clone();

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

