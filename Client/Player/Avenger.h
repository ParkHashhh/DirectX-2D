#pragma once

#include "Object/GameObject.h"

class CAvenger :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CAvenger();
	CAvenger(const CAvenger& ref);
	CAvenger(CAvenger&& ref)	noexcept;

public:
	virtual ~CAvenger();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CProjectileMovementComponent>	mMovement;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::vector<class CCollider*> mColliderList;
	class CPlayer* mOwner = nullptr;

	float		mDistance = 400.f;
	float		mSpeed = 600.f;
	FVector3	mMoveDir;
	bool		mMoveDirEnable = false;
	std::weak_ptr<CGameObject>	mTarget;

	std::string	mCollisionTargetName;
	float		mCollisionRange = 0.f;
	bool		mMoveEnable = true;
	bool		mAnimEnd = false;
	float mDamage = 0;

public:
	void AddDistance(float Distance);
	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}
	void SetOwner(CPlayer* Owner) 
	{
		mOwner = Owner; 
	}

	FVector3 GetMoveDir()
	{
		return mMoveDir;
	}
	void SetCollisionName(const std::string& Name);

	void SetMoveEnable(bool Enable);

	void SetDistance(float Distance);

	void SetSpeed(float Speed);

	void SetMoveDir(const FVector3& MoveDir);
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CAvenger* Clone();

private:
	void MoveEndFunction();
	void AnimEnd();
	void AnimResume();
private:
	void CollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void CollisionEnd(class CCollider* Dest);
};

