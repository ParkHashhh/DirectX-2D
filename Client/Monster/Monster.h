#pragma once

#include "Object/GameObject.h"
#include "../Interface/StateInterface.h"
#include "../ClientInfo.h"
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
	std::weak_ptr<class CWidgetComponent> mWidget;
	std::vector<std::function<void(float, float)>>	mHPWidgetFunc;

	std::vector<FDropItemData> mDropItemArray;
	std::vector<std::shared_ptr<class CFieldItem>> mDropResultArray;


	std::weak_ptr<CGameObject>	mTargetObject;
	float	mDetectRange = 100.f;
	float mGetDamage = 0.f;
	bool mIsAttack = false;


public:
	virtual void Damage(float Dmg);
	std::weak_ptr<CGameObject>	GetTargetObject()
	{
		return mTargetObject;
	}
	std::weak_ptr<class CColliderBox2D> GetBody()
	{
		return mBody;
	}
public:	
	virtual bool Init();
	virtual void Update(float DeltaTime);
public:
	virtual void OnHit(const FVector3& HitPoint,class CCollider* Dest);

protected:
	virtual CMonster* Clone();
	virtual void AttackNotify();
	virtual void AttackFinish();
	
};

