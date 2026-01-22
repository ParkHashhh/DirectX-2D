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

	static bool mMonsterStop;

	MonsterType::Type mType = MonsterType::Goblin;
	float	mFireTime = 0.f;
	std::weak_ptr<CGameObject>	mTargetObject;
	float	mDetectRange = 100.f;
	int		mHP = 5;
	int mGoblinAttackDistance = 10;
	int mOgreAttackDistance = 13;
	int mBallockAttackDistance = 20;
	bool mParring = false;
	bool mIsAttack = false;
	float mDefaultSpeed = 0;
	float mItemDropPercent = 50.f;
	float mSturnTime = 1.f;

	std::string mAttackAnimName;
	std::string mThrowAnimName;
	std::string mIdleAnimName;
	std::string mSturnAnimName;

public:
	
	void SetParring(bool Parring)
	{
		mParring = Parring;
	}

	void Damage(int Dmg);
	
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
	virtual float GetDefaultSpeed();
	void CollisionMonster(const FVector3& HitPoint,
		class CCollider* Dest);
	virtual void AttackNotify();
	virtual void AttackFinish();
	
};

