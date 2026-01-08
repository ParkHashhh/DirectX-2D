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
		Anim->AddAnimation("PlayerWalk");
		Anim->AddAnimation("PlayerAttack");
		//Anim->ChangeAnimation("PlayerWalk");
		Anim->SetPlayRate("PlayerAttack", 2.f);

		//Anim->AddNotify<CPlayer>("PlayerIdle", "TestNotify",
		//	4, this, &CPlayer::TestNotify);
		Anim->AddNotify<CPlayer>("PlayerAttack",
			"AttackNotify", 2, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack",
			this, &CPlayer::AttackFinish);

		//Anim->SetFinishNotify<CPlayer>("PlayerIdle", this,
		//	&CPlayer::TestNotify);

		//Anim->SetSymmetry("PlayerIdle", true);
		//Anim->SetSymmetry("PlayerWalk", true);

		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerWalk", true);
	}

	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
		Mesh->SetWorldScale(100.f, 100.f);
		Mesh->SetMaterialBaseColor(0, 255, 255, 255, 0);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetPivot(0.5f, 0.f);
	}	

	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile("Player");
		Body->SetBoxSize(100.f, 100.f);
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

	Input->AddBindKey("MoveDown", VK_DOWN);
	Input->SetBindFunction<CPlayer>("MoveDown",
		EInputType::Hold, this, &CPlayer::MoveDown);

	Input->AddBindKey("MoveLeft", VK_LEFT);
	Input->SetBindFunction<CPlayer>("MoveLeft",
		EInputType::Hold, this, &CPlayer::MoveLeft);

	Input->AddBindKey("MoveRight", VK_RIGHT);
	Input->SetBindFunction<CPlayer>("MoveRight",
		EInputType::Hold, this, &CPlayer::MoveRight);

	Input->AddBindKey("Attack", 'A');
	Input->SetBindFunction<CPlayer>("Attack", EInputType::Press, this, &CPlayer::AttackPress);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();


	if (Mesh->GetSpeed() == 0.f && mAutoIdle)
	{
		mAutoIdle = false;
		Anim->ChangeAnimation("PlayerIdle");
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
	std::shared_ptr<CWorld>	World = mWorld.lock();

	if (World)
	{
		std::weak_ptr<CBullet>	Bullet = World->CreateGameObject<CBullet>("Bullet");

		std::shared_ptr<CBullet>	BulletObj = Bullet.lock();

		if (BulletObj)
		{
			BulletObj->SetCollisionName("PlayerAttack");
			BulletObj->SetWorldPos(GetWorldPos() + GetAxis(EAxis::Y) * 75.f);
			BulletObj->SetWorldRotation(GetWorldRot());
			BulletObj->SetCollisionTargetName("Monster");
			BulletObj->ComputeCollisionRange();
		}
	}
}

void CPlayer::AttackFinish()
{
	mAutoIdle = true;
}

void CPlayer::MoveUp()
{
	mAutoIdle = true;

	auto	Movement = mMovement.lock();
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();

	Mesh->SetWorldRotation(0.f, 0.f,0.f);
	Movement->AddMove(FVector3(0.f, 1.f, 0.f));
	SetDir(FVector3(0.f, 1.f, 0.f));
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::MoveDown()
{
	mAutoIdle = true;

	auto	Movement = mMovement.lock();
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();

	Anim->ChangeAnimation("PlayerWalk");
	Mesh->SetWorldRotation(0.f, 0.f,180.f);
	Movement->AddMove(FVector3(0.f, -1.f, 0.f));
	SetDir(FVector3(0.f, -1.f, 0.f));


}

void CPlayer::MoveLeft()
{
	mAutoIdle = true;

	auto	Movement = mMovement.lock();
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();

	Anim->ChangeAnimation("PlayerWalk");
	Mesh->SetWorldRotation(0.f, 0.f, 90.f);

	Movement->AddMove(FVector3(-1.f, 0.f, 0.f));
	SetDir(FVector3(-1.f, 0.f, 0.f));
}

void CPlayer::MoveRight()
{
	mAutoIdle = true;
	auto	Movement = mMovement.lock();
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	Mesh->SetWorldRotation(0.f, 0.f, -90.f);

	Anim->ChangeAnimation("PlayerWalk");
	Movement->AddMove(FVector3(1.f,0.f,0.f));
	SetDir(FVector3(1.f, 0.f, 0.f));
}

void CPlayer::AttackPress()
{
	auto World = mWorld.lock();
	if (World)
	{
		std::weak_ptr<CBullet> mBullet = World->CreateGameObject<CBullet>("Bullet");
		std::shared_ptr<CBullet>	Bullet = mBullet.lock();
		auto	Mesh = mMeshComponent.lock();
		if (Bullet)
		{
			Bullet->SetCollisionName("PlayerAttack");
			Bullet->SetWorldRotation(GetWorldRot());
			Bullet->SetWorldPos(GetWorldPos() + mDir * 75.f);
			Bullet->SetCollisionTargetName("Monster");
			Bullet->ComputeCollisionRange();
			Bullet->SetMoveDir(mDir);
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
