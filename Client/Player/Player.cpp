#include "Player.h"
#include "Component/MeshComponent.h"
#include "Component/CameraComponent.h"
#include "Bullet.h"
#include "World/World.h"
#include "Device.h"
#include "Missile.h"
#include "../Component/StateComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/Input.h"
#include "Timer.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"

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

	mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
	mRot = CreateComponent<CSceneComponent>("Rot1");
	mCameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");

	mStateComponent = CreateComponent<CStateComponent>("State");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	mMovement = CreateComponent<CObjectMovementComponent>("Movement");

	auto	Movement = mMovement.lock();

	if (Movement)
	{
		Movement->SetUpdateComponent(mMeshComponent);
	}

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
		Anim->ChangeAnimation("PlayerRun");
		Anim->SetPlayRate("PlayerShoot", 1.f);

		Anim->AddNotify<CPlayer>("PlayerShoot",
			"AttackNotify", 1, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerShoot",
			this, &CPlayer::AttackFinish);
		Anim->AddNotify<CPlayer>("PlayerShoot",
			"AttackEndNotify", 8, this, &CPlayer::AttackEndNotify);

		Anim->AddNotify<CPlayer>("PlayerRunShoot",
			"AttackEndNotify", 11, this, &CPlayer::AttackEndNotify);

		Anim->AddNotify<CPlayer>("PlayerSlide",
			"SlideEndNotify", 5, this, &CPlayer::SlideEndNotify);



		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerRun", true);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
		Mesh->SetWorldScale(100.f, 100.f);
		Mesh->SetMaterialBaseColor(0, 255, 255, 255, 0);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetPivot(0.5f, 0.5f);
	}	

	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile("Player");
		Body->SetBoxSize(75.f, 80.f);
		Body->SetDebugDraw(true);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::OnHit);
	}

	auto	RotCom = mRot.lock();

	if (RotCom)
	{
		RotCom->SetInheritRot(false);
		RotCom->SetInheritScale(false);
	}

	auto	Camera = mCameraComponent.lock();

	if (Camera)
	{
		const FResolution& RS = CDevice::GetInst()->GetResolution();
		Camera->SetProjection(ECameraProjectionType::Ortho,
			90.f, (float)RS.Width, (float)RS.Height, 1000.f);

		Camera->SetInheritRot(false);
	}


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
	

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
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
		Anim->SetSymmetry("PlayerRun", true);
		Anim->SetSymmetry("PlayerIdle", true);
		Anim->SetSymmetry("PlayerShoot", true);
		Anim->SetSymmetry("PlayerRunShoot", true);
	}
	else if (Current.x > 0.f)
	{
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

void CPlayer::MoveUp()
{
	if (mIsSlide)
		return;
	mUpKey = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();	
	Movement->AddMove(FVector3(0.f, 1.f, 0.f));
	if (mIsShoot)
		return;

}

void CPlayer::MoveDown()
{
	if (mIsSlide)
		return;
	mDownKey = true;

	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	Movement->AddMove(FVector3(0.f, -1.f, 0.f));
	if (mIsShoot)
		return;

}

void CPlayer::MoveLeft()
{
	if (mIsSlide)
		return;
	mLeftKey = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	Movement->AddMove(FVector3(-1.f, 0.f, 0.f));
	if (mIsShoot)
		return;
}

void CPlayer::MoveRight()
{
	if (mIsSlide)
		return;
	mRightKey = true;
	auto	Movement = mMovement.lock();
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
	if (mIsSlide)
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
		auto	Mesh = mMeshComponent.lock();
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
	if (mIsSlide)
		return;
	mIsSlide = true;
	auto	Movement = mMovement.lock();
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		if (mDir.x == -1 || mDir.y == -1)
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

void CPlayer::AttackEndNotify()
{
	mIsShoot = false;
	mAutoIdle = true;
}

void CPlayer::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	if (!mIsInvincible)
	{
		mHP--;
		if (!mHP)
			return;
		mInvincibleTime = 1.0f;
		mIsInvincible = true;
	}
}
