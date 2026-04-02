#pragma once

#include "Component/ObjectComponent.h"

class CStateComponent :
    public CObjectComponent
{
	friend class CGameObject;
	friend class CObject;

protected:
	CStateComponent();
	CStateComponent(const CStateComponent& ref);
	CStateComponent(CStateComponent&& ref)	noexcept;

public:
	virtual ~CStateComponent();

protected:
	float		mHP = 1;
	float		mHPMax= 1;


	float		mMP = 0;
	float		mMPMax = 0;

	float mSpeed = 200.f;
	float mMaxSpeed = 500.f;
	int mArrow = 1;
	int mMaxArrow = 3;

public:
	void SetMPMax(float MPMax)
	{
		mMPMax = MPMax;
		mMP = mMPMax;
	}
	float GetMP() const
	{
		return mMP;
	}
	void SetHP(float HP)
	{
		mHP = HP;
	}
	float GetMPMax() const
	{
		return mMPMax;
	}
	void SetHPMax(float HPMax)
	{
		mHPMax = HPMax;
		mHP = mHPMax;
	}
	float GetHP() const
	{
		return mHP;
	}
	float GetHPMax() const
	{
		return mHPMax;
	}
	bool IsDead() const
	{
		return mHP <= 0.f;
	}
	void AddSpeed(float Speed)
	{
		mSpeed += Speed;
		if (mMaxSpeed <= mSpeed)
			mSpeed = mMaxSpeed;
		char	Test[256] = {};
		sprintf_s(Test, "CurrentSpeed : %f\n", mSpeed);
		OutputDebugStringA(Test);
	}
	void SetSpeed(float Speed)
	{
		mSpeed = Speed;
		char	Test[256] = {};
		sprintf_s(Test, "CurrentSpeed : %f\n", mSpeed);
		OutputDebugStringA(Test);
	}
	void AddArrow(int Arrow)
	{
		mArrow += Arrow;
		if (mMaxArrow <= mArrow)
			mArrow = mMaxArrow;
	}
	void AddMP(float MP)
	{
		mMP += MP;
		if (mMP >= mMPMax)
			mMP = mMPMax;
	}
	void AddHP(float HP)
	{
		mHP += HP;
		if (mHP >= mHPMax)
			mHP = mHPMax;
		if (mHP <= 0)
			mHP = 0;
	}
	float GetSpeed() const
	{
		return mSpeed;
	}
protected:
	virtual CStateComponent* Clone()	const;
};

