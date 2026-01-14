#include "Player.h"
#include "Component/MeshComponent.h"
#include "Component/CameraComponent.h"
#include "Bullet.h"
#include "World/World.h"
#include "Device.h"
#include "../Component/StateComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/Input.h"
#include "Timer.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "../Monster/Monster.h"
#include "../Monster/Fireball.h"
CPlayer::CPlayer()
{
	SetClassType<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& ref)	:
	CGameObject(ref)
{
}

CPlayer::CPlayer(CPlayer&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("PlayerMesh");
	mCameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");

	mStateComponent = CreateComponent<CStateComponent>("PlayerState");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("PlayerAnimation2D");
	mMovement = CreateComponent<CObjectMovementComponent>("PlayerMovement");
	SetMovement();
	SetAnimation();
	SetMesh();
	SetCollision();
	SetCamera();
	SetKey();
	return true;
}
void CPlayer::SetMovement()
{
	auto	Movement = mMovement.lock();

	if (Movement)
	{
		Movement->SetUpdateComponent(mMeshComponent);
	}
}

void CPlayer::SetAnimation()
{
	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("PlayerIdle");
		Anim->AddAnimation("PlayerRun");
		Anim->AddAnimation("PlayerShoot");
		Anim->AddAnimation("PlayerRunShoot");
		Anim->AddAnimation("PlayerSlide");
		Anim->AddAnimation("PlayerDead");
		Anim->AddAnimation("PlayerShield");
		Anim->SetPlayRate("PlayerShield", 10.f);
		Anim->SetPlayRate("PlayerShoot", 1.f);

		Anim->AddNotify<CPlayer>("PlayerShoot",
			"AttackNotify", 1, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerShoot",
			this, &CPlayer::AttackFinish);

		Anim->AddNotify<CPlayer>("PlayerDead",
			"AttackNotify", 1, this, &CPlayer::DeadNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerDead",
			this, &CPlayer::DeadFinish);

		Anim->AddNotify<CPlayer>("PlayerShoot",
			"AttackEndNotify", 8, this, &CPlayer::AttackEndNotify);

		Anim->AddNotify<CPlayer>("PlayerRunShoot",
			"AttackEndNotify", 11, this, &CPlayer::AttackEndNotify);

		Anim->AddNotify<CPlayer>("PlayerSlide",
			"SlideEndNotify", 5, this, &CPlayer::SlideEndNotify);


		Anim->SetLoop("PlayerIdle", true);

		Anim->SetLoop("PlayerRun", true);

		

	}
}

void CPlayer::SetMesh()
{
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetRelativePos(0, 0,0);
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
		Mesh->SetWorldScale(100.f, 100.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetPivot(0.5f, 0.5f);
	}
}

void CPlayer::SetCollision()
{
	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile("Player");
		Body->SetBoxSize(60.f, 70.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::OnHit);
	}
	mShield = CreateComponent<CColliderSphere2D>("Shield");
	auto Shield = mShield.lock();
	if (Shield)
	{
		Shield->SetCollisionProfile("PlayerShield");
		Shield->SetRadius(50);
		Shield->SetDebugDraw(false);
		Shield->SetInheritScale(false);
		Shield->SetEnable(false);
		Shield->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::OnHitShield);
	}
}

void CPlayer::SetCamera()
{
	auto	Camera = mCameraComponent.lock();

	if (Camera)
	{
		const FResolution& RS = CDevice::GetInst()->GetResolution();
		Camera->SetProjection(ECameraProjectionType::Ortho,
			90.f, (float)RS.Width, (float)RS.Height, 1000.f);

		Camera->SetInheritRot(false);
	}
}


void CPlayer::SetKey()
{
	
	// 플레이어에서 사용할 키를 등록한다.
	auto	World = mWorld.lock();

	auto	Input = World->GetInput().lock();

	Input->AddBindKey("MoveUp", VK_UP);
	Input->SetBindFunction<CPlayer>("MoveUp",
		EInputType::Hold, this, &CPlayer::MoveUp);
	Input->SetBindFunction<CPlayer>("MoveUp", EInputType::Release, this, &CPlayer::MoveUpRelease);

	Input->AddBindKey("MoveDown", VK_DOWN);
	Input->SetBindFunction<CPlayer>("MoveDown",
		EInputType::Hold, this, &CPlayer::MoveDown);
	Input->SetBindFunction<CPlayer>("MoveDown", EInputType::Release, this, &CPlayer::MoveDownRelease);


	Input->AddBindKey("MoveLeft", VK_LEFT);
	Input->SetBindFunction<CPlayer>("MoveLeft",
		EInputType::Hold, this, &CPlayer::MoveLeft);
	Input->SetBindFunction<CPlayer>("MoveLeft", EInputType::Release, this, &CPlayer::MoveLeftRelease);


	Input->AddBindKey("MoveRight", VK_RIGHT);
	Input->SetBindFunction<CPlayer>("MoveRight",
		EInputType::Hold, this, &CPlayer::MoveRight);
	Input->SetBindFunction<CPlayer>("MoveRight", EInputType::Release, this, &CPlayer::MoveRightRelease);


	Input->AddBindKey("Attack", 'A');
	Input->SetBindFunction<CPlayer>("Attack", EInputType::Press, this, &CPlayer::AttackPress);

	Input->AddBindKey("Slide", 'S');
	Input->SetBindFunction<CPlayer>("Slide", EInputType::Press, this, &CPlayer::SlidePress);


	Input->AddBindKey("MultiShot", 'D');
	Input->SetBindFunction<CPlayer>("MultiShot", EInputType::Press, this, &CPlayer::MultiShotPress);

	Input->AddBindKey("Shield", 'F');
	Input->SetBindFunction<CPlayer>("Shield", EInputType::Press, this, &CPlayer::ShieldPress);

	Input->AddBindKey("Shield", 'F');
	Input->SetBindFunction<CPlayer>("Shield", EInputType::Hold, this, &CPlayer::ShieldHold);

	Input->AddBindKey("Shield", 'F');
	Input->SetBindFunction<CPlayer>("Shield", EInputType::Release, this, &CPlayer::ShieldRelease);

}
void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	auto Body = mBody.lock();
	auto Shield = mShield.lock();
	

	if (mEnd)
	{
		return;
	}
	if(mHP <= 0)
	{
		OutputDebugStringA("Player Dead!\n");
		Anim->ChangeAnimation("PlayerDead");
	}
	
	if (Shield->GetEnable())
		return;
	

	if (mIsInvincible)
	{
		mInvincibleTime -= CTimer::GetDeltaTime();
		float Alpha = (sinf(mInvincibleTime * 20.f) + 1.f) * 0.4f + 0.2f;

		auto Mesh = mMeshComponent.lock();
		Mesh->SetMaterialOpacity(0, Alpha);

		if (mInvincibleTime <= 0.f)
		{
			Mesh->SetMaterialOpacity(0, 1);
			mIsInvincible = false;

		}
	}

	if (mKnockbackSpeed > 0.f)
	{
		AddWorldPos(mKnockbackDir * mKnockbackSpeed * DeltaTime);
		mKnockbackSpeed -= 1500.f * DeltaTime;
	}


	if (mIsSlide)
	{
		Movement->SetSpeed(300);
		Movement->AddMove(mDir);
		return;
	}
	FVector3 Current;
	if (mUpKey) Current.y += 1.f;
	if (mDownKey) Current.y -= 1.f;
	if (mLeftKey) Current.x -= 1.f;
	if (mRightKey) Current.x += 1.f;
	if (!Current.IsZero())
	{
		Current.Normalize();
		mDir = Current;
	}
	if (Current.x < 0.f)
	{
		mLastHorizonKey = -1;
		Anim->SetSymmetry("PlayerRun", true);
		Anim->SetSymmetry("PlayerIdle", true);
		Anim->SetSymmetry("PlayerShoot", true);
		Anim->SetSymmetry("PlayerRunShoot", true);
	}
	else if (Current.x > 0.f)
	{
		mLastHorizonKey = 1;
		Anim->SetSymmetry("PlayerRun", false);
		Anim->SetSymmetry("PlayerIdle", false);
		Anim->SetSymmetry("PlayerShoot", false);
		Anim->SetSymmetry("PlayerRunShoot", false);
	}

	if (!mIsShoot)
	{
		if (!Current.IsZero())
		{
			Anim->ChangeAnimation("PlayerRun");
			mAutoIdle = true;
		}
		else if (mAutoIdle && Mesh->GetSpeed() <= 0.1f)
		{
			Anim->ChangeAnimation("PlayerIdle");
			mAutoIdle = false;
		}
	}
	
	
}

void CPlayer::Destroy()
{
}


void CPlayer::AttackNotify()
{
	// 몇프레임에 무슨 반응할지.
}

void CPlayer::AttackFinish()
{
	mAutoIdle = true;
}

void CPlayer::DeadNotify()
{
	auto World = mWorld.lock();
	World->SetPlayerIsDead(true);

}

void CPlayer::DeadFinish()
{
	mEnd = true;
}

void CPlayer::MoveUp()
{
	mUpKey = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();	
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;
	Movement->AddMove(FVector3(0.f, 1.f, 0.f));

	
	if (mIsShoot)
		return;

}

void CPlayer::MoveDown()
{
	mDownKey = true;

	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;
	Movement->AddMove(FVector3(0.f, -1.f, 0.f));
	if (mIsShoot)
		return;
}

void CPlayer::MoveLeft()
{
	
	mLeftKey = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;
	Movement->AddMove(FVector3(-1.f, 0.f, 0.f));
	if (mIsShoot)
		return;
}

void CPlayer::MoveRight()
{
	mRightKey = true;
	auto	Movement = mMovement.lock();
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;
	auto	Anim = mAnimation2DComponent.lock();
	Movement->AddMove(FVector3(1.f, 0.f, 0.f));
	if (mIsShoot)
		return;
}

void CPlayer::MoveUpRelease()
{
	mAutoIdle = true;
	mUpKey = false;

}

void CPlayer::MoveDownRelease()
{
	mAutoIdle = true;
	mDownKey = false;

}

void CPlayer::MoveLeftRelease()
{
	mAutoIdle = true;
	mLeftKey = false;
}

void CPlayer::MoveRightRelease()
{
	mAutoIdle = true;
	mRightKey = false;
}



void CPlayer::AttackPress()
{
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;

	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	auto	 World = mWorld.lock();
	if (World)
	{
		mIsShoot = true;
		FVector3 ResultDir = FVector3::Zero;
		if (mUpKey)
			ResultDir.y += 1;
		if (mDownKey)
			ResultDir.y -= 1;
		if (mLeftKey)
			ResultDir.x -= 1;
		if (mRightKey)
			ResultDir.x += 1;
		if (ResultDir.IsZero())
			ResultDir = mDir;
		else
			ResultDir.Normalize();
		if (Anim)
		{
			std::string AnimName = "PlayerShoot";
			if (mUpKey || mDownKey || mLeftKey || mRightKey)
				AnimName = "PlayerRunShoot";

			if (mDir.x < 0)
				Anim->SetSymmetry(AnimName, true);
			else if (mDir.x > 0.f)
				Anim->SetSymmetry(AnimName, false);

			Anim->ChangeAnimation(AnimName);

		}

		std::weak_ptr<CBullet> mBullet = World->CreateGameObject<CBullet>("Bullet");
		std::shared_ptr<CBullet>	Bullet = mBullet.lock();
		if (Bullet)
		{
			Bullet->SetCollisionName("PlayerAttack");
			Bullet->SetWorldRotation(GetWorldRot());
			Bullet->SetWorldPos(GetWorldPos() + ResultDir * 75.f);
			Bullet->SetCollisionTargetName("Monster");
			Bullet->ComputeCollisionRange();
			Bullet->SetMoveDir(ResultDir);
			Bullet->SetSpeed(500);
		}
	}
}

void CPlayer::AttackHold()
{
}

void CPlayer::AttackRelease()
{

}

void CPlayer::SlidePress()
{
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;
	mIsSlide = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	char buf[256];
	sprintf_s(buf, " mDirx: %.2f,mDiry %.2f\n",mDir.x, mDir.y);
	OutputDebugStringA(buf);
	if (Anim)
	{
		if(mLastHorizonKey < 0)
			Anim->SetSymmetry("PlayerSlide", true);
		else
			Anim->SetSymmetry("PlayerSlide", false);
		Anim->ChangeAnimation("PlayerSlide");
	}
}

void CPlayer::SlideEndNotify()
{
	mAutoIdle = true;
	mIsSlide = false;

	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	if (Anim)
	{
		if (mDir.x == -1 || mDir.y == -1)
			Anim->SetSymmetry("PlayerIdle", true);
		else
			Anim->SetSymmetry("PlayerIdle", false);

		Anim->ChangeAnimation("PlayerIdle");
	}
}


void CPlayer::ShieldPress()
{
	if (mIsSlide)
		return;
	auto Shield = mShield.lock();
	auto Anim = mAnimation2DComponent.lock();
	Shield->SetEnable(true);
	Shield->SetDebugDraw(true);
	if(mLastHorizonKey < 0)
		Anim->SetSymmetry("PlayerShield",true);
	else
		Anim->SetSymmetry("PlayerShield", false);
	Anim->ChangeAnimation("PlayerShield");
	mAutoIdle = false;
}
void CPlayer::ShieldHold()
{

}

void CPlayer::ShieldRelease()
{
	auto Shield = mShield.lock();
	mAutoIdle = true;
	Shield->SetEnable(false);
}


void CPlayer::AttackEndNotify()
{
	mIsShoot = false;
	mAutoIdle = true;
}

void CPlayer::MultiShotPress()
{
	auto Shield = mShield.lock();
	if (Shield->GetEnable() || mIsSlide)
		return;

	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	auto	 World = mWorld.lock();
	if (World)
	{
		mIsShoot = true;
		FVector3 ResultDir = FVector3::Zero;
		if (mUpKey)
			ResultDir.y += 1;
		if (mDownKey)
			ResultDir.y -= 1;
		if (mLeftKey)
			ResultDir.x -= 1;
		if (mRightKey)
			ResultDir.x += 1;
		if (ResultDir.IsZero())
			ResultDir = mDir;
		else
			ResultDir.Normalize();
		if (Anim)
		{
			std::string AnimName = "PlayerShoot";
			if (mUpKey || mDownKey || mLeftKey || mRightKey)
				AnimName = "PlayerRunShoot";

			if (mDir.x < 0)
				Anim->SetSymmetry(AnimName, true);
			else if (mDir.x > 0.f)
				Anim->SetSymmetry(AnimName, false);

			Anim->ChangeAnimation(AnimName);

		}
		for (int i = 0; i < 3; i++)
		{
			std::weak_ptr<CBullet> mBullet = World->CreateGameObject<CBullet>("Bullet");
			std::shared_ptr<CBullet>	Bullet = mBullet.lock();
			auto	Mesh = mMeshComponent.lock();
			if (Bullet)
			{

				float FinalRotation = 20.f * (float)(i - 1);
				FMatrix RotMatrix;
				RotMatrix.RotationZ(FinalRotation);
				FVector3 ShootDir = ResultDir.TransformNormal(RotMatrix);
				ShootDir.Normalize();
				Bullet->SetCollisionName("PlayerAttack");
				float BaseAngle = atan2f(ResultDir.y, ResultDir.x) * (180.f / 3.141592f);
				Bullet->SetWorldRotationZ(BaseAngle + FinalRotation);
				Bullet->SetWorldPos(GetWorldPos() + ShootDir * 35.f);
				Bullet->SetCollisionTargetName("Monster");
				Bullet->ComputeCollisionRange();
				Bullet->SetMoveDir(ShootDir);
				Bullet->SetSpeed(500);
			}
		}
		
	}
}

void CPlayer::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	// 무적이 아닐때 맞고 여기서 데미지가 0일때를 체크해야함.
	auto Body = mBody.lock();
	if (!mIsInvincible)
	{
		if (!Damage(1))
			return;
		mInvincibleTime = 1.0f;
		mIsInvincible = true;
		Body->ClearCollisionList();
	}


	auto DestOwner = Dest->GetOwner().lock();
	FVector3	TargetPos = DestOwner->GetWorldPos();
	FVector3	TargetDir = TargetPos - GetWorldPos();
	FVector3 NormDir = TargetDir;
	NormDir.Normalize();
	mKnockbackDir = -NormDir;
	mKnockbackSpeed = 500.f;
}

void CPlayer::OnHitShield(const FVector3& HitPoint, CCollider* Dest)
{
	// 내 위치에서 상대방의 위치를 빼고 상대 Line2D의 위치를 내쉴드 좌표까지 한계를 지정해 못들어오게한다.
	auto Shield = mShield.lock();
	auto Owner = Dest->GetOwner().lock();
	if (!Owner) 
		return;
	FVector3 MyPos = GetWorldPos();
	float Radius = Shield->GetRadius();
	float ParryingLimit = Radius * 0.85f;
	FVector3 Dir = HitPoint - MyPos;
	float HitLength = Dir.Length();
	auto Monster = std::dynamic_pointer_cast<CMonster>(Owner);
	if (Monster)
	{
		if (HitLength >= ParryingLimit)
		{
			Monster->SetParring(true);
			FVector3 PushDir = Dir;
			PushDir.Normalize();
			Monster->AddWorldPos(PushDir * 10.f);
		}
		else
		{
			OnHit(HitPoint, Dest);
		}
	}
	auto FireBall = std::dynamic_pointer_cast<CFireBall>(Owner);
	if (FireBall)
	{
		if (HitLength >= ParryingLimit)
		{
			FireBall->Destroy();
		}
		else
		{
			OnHit(HitPoint, Dest);
		}
	}
}

bool CPlayer::Damage(int Dmg)
{
	{
		mHP -= Dmg;
		if (mHP <= 0)
			return false;
		char buf[256];
		sprintf_s(buf, "Player HP: %d\n", mHP);
		OutputDebugStringA(buf);
		return true;
	}
}