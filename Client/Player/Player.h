#pragma once

#include "Object/GameObject.h"
#include "../Interface/StateInterface.h"

class CPlayer :
    public CGameObject,
	public CStateInterface
{
	friend class CWorld;
	friend class CObject;

protected:
	CPlayer();
	CPlayer(const CPlayer& ref);
	CPlayer(CPlayer&& ref)	noexcept;

public:
	virtual ~CPlayer();

private:
	std::weak_ptr<class CMeshComponent>		mMeshComponent;
	std::weak_ptr<class CSceneComponent>	mRot;
	std::weak_ptr<class CMeshComponent>		mSubMeshComponent;
	std::weak_ptr<class CCameraComponent>	mCameraComponent;
	std::weak_ptr<class CStateComponent>	mStateComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CObjectMovementComponent>	mMovement;
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CColliderSphere2D>	mSphere2D;
	std::weak_ptr<class CColliderSphere2D>	mShield;
	std::weak_ptr<class CColliderLine2D>	mLine2D;
	bool	mAutoIdle = true;
	FVector3 mDir = FVector3(0.f, 1.f, 0.f);
	float mInvincibleTime = 1.0f;
	bool mIsInvincible = false;
	bool mIsSlide = false;
	bool mUpKey = false;
	bool mDownKey = false;
	bool mLeftKey = false;
	bool mRightKey = false;
	bool mIsShoot = false;
	int mLastHorizonKey = 1;

	FVector3 mKnockbackDir;
	float mKnockbackSpeed = 0.f;
	bool mEnd = false;
	bool mShieldEnable = false;

	std::weak_ptr<class CBullet>	mBullet;

public:
	bool AddHP(int HP);
	void AddSpeed(float Speed);
	void AddArrow(int Arrow);

public:
	
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Destroy();


private:
	void AttackNotify();
	void AttackFinish();
	void DeadNotify();
	void DeadFinish();

private:
	void SetKey();
	void SetCamera();
	void SetCollision();
	void SetMesh();
	void SetAnimation();
	void SetMovement();

	void SetDir(FVector3 Dir)
	{
		mDir = Dir;
	}
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveUpRelease();
	void MoveDownRelease();
	void MoveLeftRelease();
	void MoveRightRelease();


	void AttackPress();
	void AttackHold();
	void AttackRelease();
	void SlidePress();
	void SlideEndNotify();
	void ShieldPress();
	void ShieldHold();
	void ShieldRelease();
	void AttackEndNotify();
	void MultiShotPress();
	void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);
	void OnHitShield(const FVector3& HitPoint,
		class CCollider* Dest);

};

