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
	float mDefaultSpeed = 120.f;
	float mSkillTime = 3.f;

	int mBressPercent = 25;
	float mBressPauseTime = 2.f;
	int mLastBressHP = 0;
	bool mIsBress = false;
	float mBressTime = 5.f;
	int mHPMax = 100;
	std::weak_ptr<class CFireBall> mFireBall;
	std::weak_ptr<class CColliderBox2D>	mBressBody;
	std::shared_ptr<class CBressEffect> mBressEffect;

	FVector3	mTargetPos;
	FVector3	mTargetDir;
	float mAngle;
public:
	virtual ~CBallock();
protected:
	virtual CBallock* Clone();

public:

	virtual void Update(float DeltaTime);
	virtual void OnHit();
	void ThrowNotify();
	virtual float GetDefaultSpeed()
	{
		return mDefaultSpeed;
	}
	virtual bool Init();
	virtual void AttackNotify();
	virtual void SkillNotify();
	virtual void BressNotify();
	virtual void BressFirstNotify();
	virtual void AttackFinish();
	virtual void DeadFinish();
	virtual void DeadNotify();
	virtual void SetMonsterData();
};

