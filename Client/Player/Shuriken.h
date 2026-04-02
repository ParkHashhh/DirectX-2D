#pragma once

#include "Object/GameObject.h"

class CShuriken :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CShuriken();
	CShuriken(const CShuriken& ref);
	CShuriken(CShuriken&& ref)	noexcept;

public:
	virtual ~CShuriken();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CProjectileMovementComponent>	mMovement;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;
	float		mDistance = 400.f;
	float		mSpeed = 400.f;
	FVector3	mMoveDir;
	bool		mMoveDirEnable = false;
	std::weak_ptr<CGameObject>	mTarget;

	float mDamage = 0.f;

	std::string	mCollisionTargetName;
	float		mCollisionRange = 0.f;
	bool		mMoveEnable = true;

public:
	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}
	FVector3 GetMoveDir()
	{
		return mMoveDir;
	}
	void SetCollisionName(const std::string& Name);

	void SetMoveEnable(bool Enable);

	void SetDistance(float Distance);

	void SetSpeed(float Speed);

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

	void SetMoveDir(const FVector3& MoveDir);

	void SetNearTarget(const std::string& Name, float Horizon);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	void SetSymmetry(bool Symmetry);

protected:
	virtual CShuriken* Clone();

private:
	void MoveEndFunction();

private:
	void CollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void CollisionEnd(class CCollider* Dest);
};

