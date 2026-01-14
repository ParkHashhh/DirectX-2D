#pragma once
#include "Monster.h"
class COgre :
    public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	COgre();
	COgre(const COgre& ref);
	COgre(COgre&& ref)	noexcept;
	float mDefaultSpeed = 50.f;
public:
	virtual ~COgre();
protected:
	virtual COgre* Clone();

public:
	virtual void OnHit();
	virtual float GetDefaultSpeed()
	{
		return mDefaultSpeed;
	}
	virtual bool Init();
	virtual void AttackNotify();
	virtual void AttackFinish();
	virtual void SetMonsterData();
};

