#pragma once

#include "Object/GameObject.h"

class CFireBall :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFireBall();
	CFireBall(const CFireBall& ref);
	CFireBall(CFireBall&& ref)	noexcept;

public:
	virtual ~CFireBall();

private:
	std::weak_ptr<class CMeshComponent>		mMeshComponent;
	std::weak_ptr<class CProjectileMovementComponent>	mMovement;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderSphere2D>	mBall;
	float		mDistance = 600;
	float		mSpeed = 300.f;
	bool mMoveDirEnable = false;
	FVector3	mMoveDir;
	std::weak_ptr<CGameObject>	mTarget;

	std::string	mCollisionTargetName;
	float		mCollisionRange = 0.f;
	bool		mMoveEnable = true;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	void SetCollisionName(const std::string& Name);
	void SetMoveEnable(bool Enable);
	void SetDistance(float Distance);
	void SetSpeed(float Speed);
	void SetMoveDir(const FVector3& MoveDir);
	void SetNearTarget(const std::string& Name);

	void ComputeCollisionRange()
	{
		FVector3	Scale = GetWorldScale();

		Scale /= 2.f;

		mCollisionRange = sqrtf(Scale.x * Scale.x + Scale.y * Scale.y);
	}

	void SetCollisionTargetName(const std::string& Name)
	{
		mCollisionTargetName = Name;
	}

protected:
	virtual CFireBall* Clone();
private:
	void MoveEndFunction();

private:
	void CollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void CollisionEnd(class CCollider* Dest);
};

