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
	int		mHP = 5;
	int		mHPMax = 5;
	float mSpeed = 300.f;
	float mMaxSpeed = 500.f;
	int mArrow = 1;
	int mMaxArrow = 3;

public:
	bool IsDead()
	{
		return mHP <= 0;
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
	bool AddHP(int HP)
	{
		mHP += HP;
		if (mHP >= mHPMax)
			mHP = mHPMax;
		else if (mHP <= 0)
		{
			mHP = 0;
			return false;
		}
		char	Test[256] = {};
		sprintf_s(Test, "HP : %d\n", mHP);
		OutputDebugStringA(Test);

		return true;
	}
	int GetArrowCount()
	{
		return  mArrow;
	}
	float GetSpeed()
	{
		return mSpeed;
	}
protected:
	virtual CStateComponent* Clone()	const;
};

