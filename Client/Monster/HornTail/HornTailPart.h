#pragma once

#include "../Monster.h"

class CHornTailPart :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CHornTailPart();
	CHornTailPart(const CHornTailPart& ref);
	CHornTailPart(CHornTailPart&& ref) noexcept;
public:
	virtual ~CHornTailPart();

protected:
	bool mIsDead = false;
	FVector3 mDiePos;
public:
	bool GetIsDead()
	{
		return mIsDead;
	}
	void SetDiePos(FVector3 Pos)
	{
		mDiePos = Pos;
	}


	void SetHPMax(float HP);
	float GetHP();
	float GetHPMax();
	void AddHP(float HP);


	virtual void ChangeAttack() = 0;
	virtual bool Init();
};