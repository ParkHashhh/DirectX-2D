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
	class CGameObject* mTargetObject = nullptr;

	std::weak_ptr<class CMeshComponent>		mMeshComponent;
	std::weak_ptr<class CSceneComponent>	mRot;
	std::weak_ptr<class CMeshComponent>		mSubMeshComponent;
	std::weak_ptr<class CCameraComponent>	mCameraComponent;
	std::weak_ptr<class CStateComponent>	mStateComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CObjectMovementComponent>	mMovement;
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CColliderBox2D>	mAttackColider;
	std::weak_ptr<class CColliderSphere2D>	mSphere2D;
	std::weak_ptr<class CColliderLine2D>	mLine2D;
	std::weak_ptr<class CInventory>			mInventory;
	std::weak_ptr<class CEquipment>			mEquipment;
	std::weak_ptr<class CStatement>			mStatement;
	std::weak_ptr<class CPlayerStateWidget>	mPlayerStateWidget;
	std::weak_ptr<class CPlayerSkillWidget> mPlayerSkillWidget;
	std::weak_ptr<class CPlayerSkillStateWidget> mPlayerSkillStateWidget;
	std::shared_ptr<class CRayTracing> mRay;
	std::shared_ptr<class CRayTracing> mWallRay;

	std::weak_ptr<class CCollider> mBottom;

	std::shared_ptr<class CSturnDebuffEffect> mSturnEffect;
	std::shared_ptr<class CDarkDebuffEffect> mDarkEffect;


	std::shared_ptr<class CShadow> mShadow;
	std::weak_ptr<class CAnimation2DComponent> mShadowAnimation;


	std::vector <CCollider*> mNearCollisionList;
	int mNearCount = 0;
	bool	mAutoIdle = true;
	FVector3 mDir = FVector3(0.f, 1.f, 0.f);
	float mInvincibleTime = 1.0f;
	float mJumpVelocity = 1.0f;
	float mFlashJumpVelocity = 1.0f;
	int mJumpCount = 0;
	bool mIsInvincible = false;
	bool mIsOnGround = false;
	bool mIsDown = false;
	bool mUpKey = false;
	bool mDownKey = false;
	bool mLeftKey = false;
	bool mRightKey = false;
	bool mIsAttack = false;
	bool mIsNearAttack = false;

	bool mIsJumping = false;

	bool IsHornTailWorld = false;

	int mLastHorizonKey = -1;
	int mAttackCount = 0;
	std::vector<std::string> mAttackAnimList;
	int mShurikenCount = 0;
	float mShurikenDelayTime = 0.f;
	float mShurikenDelayMax = 0.1f;
	int mShurikenLastHorizon = -1;
	float mShurikenY = 0.f;
	float mShurikenX = 0.f;
	FVector3 mShurikenDir = FVector3::Zero;

	int mDamageRange = 2000;
	
	float mThrowDamage = 3000.f;
	float mAvengerDamage = 12000.f;

	bool mSturnDebuff = false;
	bool mDarkDebuff = false;

	float mSturnDebuffTime = 2.f;
	float mDarkDebuffTime = 3.f;

	float mWillSkillCoolTime = 15.f;
	bool mWillSkillUse = true;

	float mAvengerCooltime = 2.f;
	bool mAvengerUse = true;

	int   mShadowAvengerCount = 0;   
	float mShadowAvengerDelay = 0.5f;
	int mShadowAvengerDir = -1;

	FVector3 mKnockbackDir;
	float mKnockbackSpeed = 0.f;

	bool mIsHeist = false;
	bool mIsBoost = false;
	bool mIsDead = false;
	bool mIsKnockBack = false;

	float mTargetHP = 0.f;      
	float mCurrentShowHP = 0.f;
	float mTargetMP = 0.f;
	float mCurrentShowMP = 0.f;

	std::weak_ptr<class CShuriken>	mShuriken;
	std::vector<std::function<void(float, float)>>	mHPWidgetFunc;
	std::function<void(float, float)>	mMPWidgetFunc;
	std::function<void(float, float)>	mAvengerWidgetFunc;
	std::function<void(float, float)>	mWillSkillWidgetFunc;
public:
	void SetHornTailWorld(bool HornTailWorld);
	void SetTargetObject(CGameObject* Obj)
	{
		mTargetObject = Obj;
	}
	void SetSturnDebuff(bool Debuff)
	{
		mSturnDebuff = Debuff;
	}

	void SetDarkDebuff(bool Debuff)
	{
		mDarkDebuff = Debuff;
	}
	void AddFireEye();
public:
	void SetHP(float HP, bool KnockBack = true);
	float GetHPMax();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Destroy();
	virtual float TakeDamage(float Damage, bool KnockBack = true);
	void ResumeAvengerAnim();
	void EndBuff(std::string& BuffName);
	void SetZakumEquip();
	void SetHorntailEquip();
	void AddDragonMilk();
	
private:
	void AttackNotify();
	void AttackFinish();
	void DeadNotify();
	void DeadFinish();
	void AvengerNotify();
	void DropKey();

	

private:
	void CreateComponents();
	void CreatePlayerEffects();
	void SetBottom();
	void SetRay();
	void SetKey();
	void SetCamera();
	void SetCollision();
	void SetMesh();
	void SetAnimation();
	void SetMovement();
	void SetWidget();
	void SetState();
	void SetDir(FVector3 Dir)
	{
		mDir = Dir;
	}
	void Jump();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveUpRelease();
	void MoveDownRelease();
	void MoveLeftRelease();
	void MoveRightRelease();
	void InsertKey();
	void HomeKey();
	void PageUpKey();
	void PageDownKey();
	void AvengerKey();
	void DeleteKey();
	void EndKey();
	void EscKey();
	void CreateShuriken();

	void AttackPress();
	void AttackHold();
	void AttackRelease();
	void AttackEndNotify();
	void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);
	void RayCollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void RayCollisionEnd(class CCollider* Dest);

	void WallRayCollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void WallRayCollisionEnd(class CCollider* Dest);

	void OpenInventory();
	void OpenEquipment();
	void OpenStatement();

private:
	void CheckBreakBottom();

	void UpdateSturnDebuff(float DeltaTime);
	void UpdateDarkDebuff(float DeltaTime);
	void UpdateStateWidget(float DeltaTime);
	void UpdateVelocity(float DeltaTime);
	void CheckNearMonster();
	void UpdateInvincible(float DeltaTime);
	bool CheckDead();
	void UpdateKnockBack(float DeltaTime);
	void UpdateAvengerWidget(float DeltaTime);
	void UpdateWillWidget(float DeltaTime);
	void UpdateCreateShuriken(float DeltaTime);
	void UpdateCreateAvenger(float DeltaTime);
	void UpdateDirection();
	void UpdateAnimationDirection();
	void UpdateAttackBodyPosition();
	void UpdateWallRay();
	void UpdateMoveAnimation();
	void UpdateShadowPosition();
	void CheckShadowEnable();
};

