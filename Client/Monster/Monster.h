#pragma once

#include "Object/GameObject.h"
#include "../Interface/StateInterface.h"

class CMonster :
    public CGameObject,
	public CStateInterface
{
	friend class CWorld;
	friend class CObject;

protected:
	CMonster();
	CMonster(const CMonster& ref);
	CMonster(CMonster&& ref)	noexcept;

public:
	virtual ~CMonster();

protected:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CStateComponent>	mStateComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CObjectMovementComponent>	mMovement;
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CColliderLine2D>	mLine2D;

	MonsterType::Type mType = MonsterType::Goblin;
	float	mFireTime = 0.f;
	std::weak_ptr<CGameObject>	mTargetObject;
	float	mDetectRange = 100.f;
	int		mHP = 5;
	int mGoblinAttackDistance = 10;
	int mOgreAttackDistance = 13;
	bool mParring = false;
	bool mIsAttack = false;
	float mDefaultSpeed = 0;
	std::string mAttackAnimName;
	std::string mIdleAnimName;

public:
	void SetParring(bool Parring)
	{
		mParring = Parring;
	}

	void Damage(int Dmg)
	{
		mHP -= Dmg;
		if (mHP <= 0)
			Destroy();
		char	Test[256] = {};
		sprintf_s(Test, "HP : %d\n", mHP);
		OutputDebugStringA(Test);
	}
	void SetSpeed(int Speed)
	{
		mDefaultSpeed = Speed;
	}

	std::weak_ptr<CGameObject>	GetTargetObject()
	{
		return mTargetObject;
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
public:
	virtual void SetMonsterData();
	virtual void OnHit();

protected:
	virtual CMonster* Clone();

private:
	virtual float GetDefaultSpeed();
	void CollisionMonster(const FVector3& HitPoint,
		class CCollider* Dest);
	virtual void AttackNotify();
	virtual void AttackFinish();
	
};

