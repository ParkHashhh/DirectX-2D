#include "Player.h"
#include "Component/MeshComponent.h"
#include "Component/CameraComponent.h"
#include "Shuriken.h"
#include "World/World.h"
#include "Device.h"
#include "../Component/StateComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/Input.h"
#include "Timer.h"
#include "JumpEffect.h"
#include "Shadow.h"
#include "ShadowEffect.h"
#include "ShadowEffect2.h"
#include "HeistEffect.h"
#include "Avenger.h"
#include "BoostEffect.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "../Monster/Monster.h"
#include "../UI/Inventory.h"
#include "../UI/Equipment.h"
#include "../UI/PlayerStateWidget.h"
#include "../UI/PlayerSkillWidget.h"
#include "../UI/PlayerSkillStateWidget.h"
#include "RayTracing.h"
#include "CollisionInfoManager.h"
#include "Grave.h"
#include "../UI/EndingWidget.h"
#include "SturnDebuffEffect.h"
#include "DarkDebuffEffect.h"
#include "WillEffect.h"
#include "../Item/Item.h"
#include "../Item/FieldItem.h"
#include "../UI/InventoryManager.h"
#include "../UI/Statement.h"
#include "TripleThrowEffect.h"


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
	CreateComponents();
	SetState();
	SetRay();
	SetBottom();
	SetMesh();
	SetWidget();
	SetMovement();
	SetAnimation();
	SetCollision();
	SetCamera();
	SetKey();
	CreatePlayerEffects();

	
	return true;
}


float CPlayer::GetHPMax()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		return State->GetHPMax();
	}
	return 0;

}
void CPlayer::SetZakumEquip()
{
	auto Equip = mEquipment.lock();
	if (Equip)
	{
		Equip->SetZakumEquip();
	}
}
void CPlayer::SetHorntailEquip()
{
	auto Equip = mEquipment.lock();
	if (Equip)
	{
		Equip->SetHornTailEquip();
	}
}
void CPlayer::AddDragonMilk()
{
	std::shared_ptr<CItem> Item = std::make_shared<CItem>();
	
	FDropItemData Data;
	Data.Name = "DragonMilk";
	Data.IconPath = L"Slot/DragonMilk.png";

	Data.Type = EItemType::Consume;

	Item->SetData(Data);

	CInventoryManager::GetInst()->AddItem(Item);


}
void CPlayer::SetState()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		if (IsHornTailWorld)
		{
			State->SetHPMax(6358.f);
			State->SetMPMax(873.f);

			mThrowDamage = 7248.f;
			mAvengerDamage = 13434.f;
		}
		else
		{
			State->SetHPMax(4179.f);
			State->SetMPMax(625.f);
			mThrowDamage = 3248.f;
			mAvengerDamage = 8434.f;
		}
	}
}
void CPlayer::SetMovement()
{
	auto	Movement = mMovement.lock();
	auto State = mStateComponent.lock();
	if (Movement)
	{
		Movement->SetUpdateComponent(mMeshComponent);
		Movement->SetSpeed(State->GetSpeed());
	}
}
void CPlayer::SetWidget()
{
	auto	World = mWorld.lock();
	auto UIMng = World->GetUIManager().lock();
	if (UIMng)
	{
		mInventory = UIMng->CreateWidget<CInventory>("Inventory");
		auto Inventory = mInventory.lock();
		Inventory->AddPos(FVector2(540, 200));
		Inventory->SetEnable(false);
		Inventory->SetPlayer(mSelf);

		mEquipment = UIMng->CreateWidget<CEquipment>("Equipment");
		auto Equipment = mEquipment.lock();
		Equipment->AddPos(FVector2(340, 200));
		Equipment->SetEnable(false);
		Equipment->SetPlayer(mSelf);

		mStatement = UIMng->CreateWidget<CStatement>("Statement");
		auto Statement = mStatement.lock();
		Statement->AddPos(FVector2(140, 200));
		Statement->SetEnable(false);
		Statement->SetPlayer(mSelf);

		mPlayerStateWidget = UIMng->CreateWidget<CPlayerStateWidget>("StateWidget");
		mPlayerSkillWidget = UIMng->CreateWidget<CPlayerSkillWidget>("SkillWidget");
		mPlayerSkillStateWidget = UIMng->CreateWidget<CPlayerSkillStateWidget>("SkillStateWidget");
		auto PlayerSkillWidget = mPlayerSkillWidget.lock();
		auto PlayerStateWidget = mPlayerStateWidget.lock();
		auto PlayerSkillStateWidget = mPlayerSkillStateWidget.lock();
		PlayerSkillStateWidget->SetPlayer(std::dynamic_pointer_cast<CPlayer>(mSelf.lock()));

		PlayerSkillWidget->SetEnable(true);
		PlayerSkillWidget->SetPos(1050.f, 610.f);

		PlayerStateWidget->SetEnable(true);
		PlayerStateWidget->SetPos(500.f, 620.f);
		PlayerStateWidget->MultipleWidth(1.5f);
		PlayerStateWidget->MultipleHeight(1.2f);


		mHPWidgetFunc.push_back(std::bind(&CPlayerStateWidget::SetPlayerHP, PlayerStateWidget.get(),
			std::placeholders::_1, std::placeholders::_2));

		mMPWidgetFunc = std::bind(&CPlayerStateWidget::SetPlayerMP, PlayerStateWidget.get(),
			std::placeholders::_1, std::placeholders::_2);

		mAvengerWidgetFunc = std::bind(&CPlayerSkillWidget::SetAvengerSkillCoolTime, PlayerSkillWidget.get(),
			std::placeholders::_1, std::placeholders::_2);

		mWillSkillWidgetFunc = std::bind(&CPlayerSkillWidget::SetWillSkillCoolTime, PlayerSkillWidget.get(),
			std::placeholders::_1, std::placeholders::_2);
	}
}

void CPlayer::SetAnimation()
{
	// 애니메이션 지정
	auto	Anim = mAnimation2DComponent.lock();

	mAttackAnimList.push_back("PlayerAttack");
	mAttackAnimList.push_back("PlayerAttack1");
	mAttackAnimList.push_back("PlayerAttack2");

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);
		Anim->AddAnimation("PlayerIdle");
		Anim->AddAnimation("PlayerWalk");
		Anim->AddAnimation("PlayerAttack");
		Anim->AddAnimation("PlayerAttack1");
		Anim->AddAnimation("PlayerAttack2");
		Anim->AddAnimation("PlayerAttack3");
		Anim->AddAnimation("PlayerDown");
		Anim->AddAnimation("PlayerJump");
		Anim->AddAnimation("PlayerDead");
		Anim->AddAnimation("PlayerNearAttack");

		Anim->SetPlayRate("PlayerAttack", 1.3f);
		Anim->SetPlayRate("PlayerAttack1", 1.3f);
		Anim->SetPlayRate("PlayerAttack2", 1.3f);
		Anim->SetPlayRate("PlayerAttack3", 1.f);
		Anim->SetPlayRate("PlayerNearAttack", 1.3f);

		Anim->AddNotify<CPlayer>("PlayerAttack",
			"AttackNotify", 0, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack",
			this, &CPlayer::AttackFinish);

		Anim->AddNotify<CPlayer>("PlayerAttack1",
			"AttackNotify", 0, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack1",
			this, &CPlayer::AttackFinish);

		Anim->AddNotify<CPlayer>("PlayerAttack2",
			"AttackNotify", 0, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack2",
			this, &CPlayer::AttackFinish);

		Anim->AddNotify<CPlayer>("PlayerAttack3",
			"AttackNotify", 0, this, &CPlayer::AvengerNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack3",
			this, &CPlayer::AttackFinish);

		Anim->AddNotify<CPlayer>("PlayerNearAttack",
			"AttackNotify", 0, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerNearAttack",
			this, &CPlayer::AttackFinish);


		Anim->AddNotify<CPlayer>("PlayerDead",
			"AttackNotify", 1, this, &CPlayer::DeadNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerDead",
			this, &CPlayer::DeadFinish);

		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerWalk", true);

	}
}
	
void CPlayer::SetMesh()
{
	auto	Mesh = mMeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
		Mesh->SetWorldScale(65.f, 70.f);
		Mesh->SetWorldPos(-300,-300);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetPivot(0.5f, 0.5f);
		Mesh->SetRenderLayer("Player");
	}
}

void CPlayer::SetCollision()
{
	mBody = CreateComponent<CColliderBox2D>("Body");
	auto	Body = mBody.lock();

	if (Body)
	{
		Body->SetCollisionProfile("Player");
		Body->SetBoxSize(65.f, 70.f);
		Body->SetDebugDraw(false);
		Body->SetInheritScale(false);
		Body->SetEnable(true);
	}
	mAttackColider = CreateComponent<CColliderBox2D>("Attack");
	auto	AttackColider = mAttackColider.lock();

	if (AttackColider)
	{
		AttackColider->SetCollisionProfile("NearAttack");
		AttackColider->SetBoxSize(30.f,50.f);
		AttackColider->SetDebugDraw(false);
		AttackColider->SetInheritScale(false);
		AttackColider->SetEnable(true);
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


void CPlayer::CreateComponents()
{
	mMeshComponent = CreateComponent<CMeshComponent>("PlayerMesh");
	mCameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");

	mStateComponent = CreateComponent<CStateComponent>("PlayerState");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("PlayerAnimation2D");
	mMovement = CreateComponent<CObjectMovementComponent>("PlayerMovement");
}

void CPlayer::CreatePlayerEffects()
{
	auto World = mWorld.lock();

	mShadow = World->CreateGameObject<CShadow>("Shadow").lock();
	mShadow->SetEnable(false);
	mSturnEffect = World->CreateGameObject<CSturnDebuffEffect>("SturnEffect").lock();
	mSturnEffect->Pause();

	mDarkEffect = World->CreateGameObject<CDarkDebuffEffect>("DarkEffect").lock();
	mDarkEffect->Pause();
}

void CPlayer::SetBottom()
{
	auto World = mWorld.lock();
	if (World)
	{
		auto Collision = World->GetCollision().lock();
		if (!Collision)
			return;
		auto List = Collision->GetColliderList();

		if (!List.empty())
		{
			auto iter = List.begin();
			auto iterEnd = List.end();
			for (; iter != iterEnd; iter++)
			{
				auto Collider = iter->lock();
				if (Collider->GetCollisionProfile()->Name == "Gravity" && Collider->GetName() == "Bottom")
				{
					mBottom = *iter;
					break;
				}
			}

		}
	}
}

void CPlayer::SetRay()
{
	auto World = mWorld.lock();
	mRay = World->CreateGameObject<CRayTracing>("Ray").lock();

	auto RayBox = mRay->GetBox2D().lock();
	if (RayBox)
	{
		RayBox->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::RayCollisionBegin);
		RayBox->SetCollisionEndFunction<CPlayer>(this, &CPlayer::RayCollisionEnd);
		mMeshComponent.lock()->AddChild(RayBox);
		RayBox->SetRelativePos(0, -37.f);
		RayBox->SetBoxSize(20.f, 5.f);
	}

	mWallRay = World->CreateGameObject<CRayTracing>("WallRay").lock();
	mWallRay->SetEnable(true);

	auto WallBox = mWallRay->GetBox2D().lock();
	if (WallBox)
	{
		WallBox->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::WallRayCollisionBegin);
		WallBox->SetCollisionEndFunction<CPlayer>(this, &CPlayer::WallRayCollisionEnd);
		WallBox->SetBoxSize(5, 60.f);
	}
}

void CPlayer::SetKey()
{
	
	// 플레이어에서 사용할 키를 등록한다.
	auto	World = mWorld.lock();

	auto	Input = World->GetInput().lock();


	Input->AddBindKey("MoveUp", VK_UP);
	Input->SetBindFunction<CPlayer>("MoveUp",
		EInputType::Press, this, &CPlayer::MoveUp);
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

	Input->AddBindKey("InsertKey", VK_INSERT);
	Input->SetBindFunction<CPlayer>("InsertKey",
		EInputType::Press, this, &CPlayer::InsertKey);

	Input->AddBindKey("EscKey", VK_ESCAPE);
	Input->SetBindFunction<CPlayer>("EscKey",
		EInputType::Press, this, &CPlayer::EscKey);
	
	Input->AddBindKey("HomeKey", VK_HOME);
	Input->SetBindFunction<CPlayer>("HomeKey",
		EInputType::Press, this, &CPlayer::HomeKey);

	Input->AddBindKey("PageUpKey", VK_PRIOR);
	Input->SetBindFunction<CPlayer>("PageUpKey",
		EInputType::Press, this, &CPlayer::PageUpKey);

	Input->AddBindKey("PageDownKey", VK_NEXT);
	Input->SetBindFunction<CPlayer>("PageDownKey",
		EInputType::Press, this, &CPlayer::PageDownKey);

	Input->AddBindKey("DeleteKey", VK_DELETE);
	Input->SetBindFunction<CPlayer>("DeleteKey",
		EInputType::Press, this, &CPlayer::DeleteKey);

	Input->AddBindKey("EndKey", VK_END);
	Input->SetBindFunction<CPlayer>("EndKey",
		EInputType::Press, this, &CPlayer::EndKey);


	Input->AddBindKey("Attack", (unsigned char)1);
	Input->SetBindFunction<CPlayer>("Attack", EInputType::Hold, this, &CPlayer::AttackPress);

	Input->AddBindKey("Jump", (unsigned char)2);
	Input->SetBindFunction<CPlayer>("Jump",
		EInputType::Press, this, &CPlayer::Jump);

	Input->AddBindKey("AvengerKey", (unsigned char)3);
	Input->SetBindFunction<CPlayer>("AvengerKey",
		EInputType::Hold, this, &CPlayer::AvengerKey);

	Input->AddBindKey("Inventory", 'I');
	Input->SetBindFunction<CPlayer>("Inventory",
		EInputType::Press, this, &CPlayer::OpenInventory);

	Input->AddBindKey("Equipment", 'E');
	Input->SetBindFunction<CPlayer>("Equipment",
		EInputType::Press, this, &CPlayer::OpenEquipment);

	Input->AddBindKey("DropKey", 'Z');
	Input->SetBindFunction<CPlayer>("DropKey",
		EInputType::Press, this, &CPlayer::DropKey);

	Input->AddBindKey("Statement", 'S');
	Input->SetBindFunction<CPlayer>("Statement",
		EInputType::Press, this, &CPlayer::OpenStatement);
}
void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!CheckDead())
		return;

	CheckShadowEnable();
	// Status
	UpdateSturnDebuff(DeltaTime);
	UpdateDarkDebuff(DeltaTime);
	UpdateInvincible(DeltaTime);


	// Physics
	UpdateVelocity(DeltaTime);
	UpdateKnockBack(DeltaTime);
	UpdateWallRay();

	// Widget
	UpdateStateWidget(DeltaTime);
	UpdateAvengerWidget(DeltaTime);
	UpdateWillWidget(DeltaTime);

	// ? ??
	CheckNearMonster();
	UpdateCreateShuriken(DeltaTime);
	UpdateCreateAvenger(DeltaTime);

	// Check Dir
	UpdateDirection();
	UpdateAnimationDirection();
	UpdateAttackBodyPosition();
	UpdateMoveAnimation();
	UpdateShadowPosition();

}

void CPlayer::Destroy()
{
}


void CPlayer::AttackNotify()
{
	mIsAttack = true;
	if (!mIsNearAttack)
	{
		auto World = mWorld.lock();
		auto AssetMng = World->GetWorldAssetManager().lock();
		AssetMng->SoundPlay("TripleThrow");
		auto Effect = World->CreateGameObject<CTripleThrowEffect>("Effect").lock();
		int RandYOffset = rand() % 40 - 20;
		auto AttackBody = mAttackColider.lock();
		if (mLastHorizonKey > 0)
			Effect->SetSymettry();
		FVector3 EffectPos = AttackBody->GetWorldPos();
		EffectPos.x += mLastHorizonKey * -20.f;
		Effect->SetWorldPos(EffectPos);
	}
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		if (IsHornTailWorld)
		{
			mMeshComponent.lock()->SetWorldScale(110.f, 110.f);
		}
		else
		{
			Mesh->SetWorldScale(90, 70);
		}
	}
}

void CPlayer::AttackFinish()
{
	mIsAttack = false;
	mAutoIdle = true;
	mIsNearAttack = false;
	auto AttackColider = mAttackColider.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(65, 70);
		if (IsHornTailWorld)
		{
			mMeshComponent.lock()->SetWorldScale(110.f, 110.f);
		}
	}

	if (Anim)
	{
		Anim->ChangeAnimation("PlayerIdle");
		auto ShadowAnim = mShadowAnimation.lock();
		if (ShadowAnim)
			ShadowAnim->ChangeAnimation("ShadowIdle");
	}
}

void CPlayer::DeadNotify()
{

}

void CPlayer::DeadFinish()
{
}

void CPlayer::Jump()
{	
	if (mIsDead || mIsAttack || mSturnDebuff)
		return;
	auto Anim = mAnimation2DComponent.lock();
	auto World = mWorld.lock();
	if (!mIsJumping)
	{
		auto AssetMng = World->GetWorldAssetManager().lock();
	
		AssetMng->SoundPlay("Jump");
		if (mIsOnGround)
		{
			if (mIsDown)
			{
				if (mRay->GetCollisionName() != "Bottom")
				{
					MoveDownRelease();

					mIsJumping = true;
					auto ShadowAnim = mShadowAnimation.lock();
					if (ShadowAnim)
						ShadowAnim->ChangeAnimation("ShadowJump");
					Anim->ChangeAnimation("PlayerJump");
					mIsOnGround = false;
					return;
				}
				else
				{
					MoveDownRelease();
				}
			}
			auto Body = mRay->GetBox2D().lock();
			if (Body)
				Body->SetEnable(false);
			mRay->ClearCollision();
			mIsJumping = true;
			auto ShadowAnim = mShadowAnimation.lock();
			if (ShadowAnim)
				ShadowAnim->ChangeAnimation("ShadowJump");
			Anim->ChangeAnimation("PlayerJump");
			mIsOnGround = false;
			mJumpVelocity = 530.f;
			mJumpCount = 1;
			if (mIsHeist)
				mJumpVelocity += 100;
		}
	}

	else if (mJumpCount == 1)
	{

		if (mUpKey)	// DoubleJump
		{
			auto AssetMng = World->GetWorldAssetManager().lock();
			AssetMng->SoundPlay("FlashJump");

			if (World)
			{
				std::weak_ptr<CJumpEffect> Effect = World->CreateGameObject<CJumpEffect>("JumpEffect");
				
				auto	_Effect = Effect.lock();
				_Effect->SetWorldRotationZ(90);
				_Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - 50);
				_Effect->SetSymmetry(false);
			}
			mJumpVelocity = 800.f;
			mJumpCount = 2;
			mIsOnGround = false;
			auto Body = mRay->GetBox2D().lock();
			if (Body)
				Body->SetEnable(false);
		}
		else // FlashJump
		{

			// 점프 이펙트 추가
			if (World)
			{

				auto AssetMng = World->GetWorldAssetManager().lock();
				AssetMng->SoundPlay("FlashJump");
				std::weak_ptr<CJumpEffect> Effect = World->CreateGameObject<CJumpEffect>("JumpEffect");

				auto	_Effect = Effect.lock();

				if (mLastHorizonKey < 0)
				{
					_Effect->SetWorldPos(GetWorldPos().x + 50, GetWorldPos().y);
					_Effect->SetSymmetry(false);
				}
				else
				{
					_Effect->SetWorldPos(GetWorldPos().x - 50, GetWorldPos().y);
					_Effect->SetSymmetry(true);
				}

			}
			mJumpCount = 2;
			mFlashJumpVelocity = (float)mLastHorizonKey * 700.f;
			mJumpVelocity += 100.f;
		}
	}
}


void CPlayer::MoveUp()
{
	if (mIsDead || mSturnDebuff)
		return;
	mUpKey = true;
	if (mTargetObject)
	{
		mTargetObject->Interact();
	}
}

void CPlayer::MoveDown()
{
	if (mIsDead || mSturnDebuff)
		return;
	mDownKey = true;
	if (!mIsOnGround || mIsDown || mIsAttack)
		return;

	auto	Anim = mAnimation2DComponent.lock();
	auto	Body = mBody.lock();
	auto	Mesh = mMeshComponent.lock();
	if (mIsJumping)
		return;
	mIsDown = true;
	if (Anim)
	{
		Anim->ChangeAnimation("PlayerDown");
		auto ShadowAnim = mShadowAnimation.lock();
		if (ShadowAnim)
			ShadowAnim->ChangeAnimation("ShadowDown");
	}
	auto RayBody = mRay->GetBox2D().lock();
	
	if (Mesh)
	{
		if (IsHornTailWorld)
		{
			Mesh->SetWorldScale(110.f, 110.f);
			Mesh->SetRelativePos(Mesh->GetRelativePos().x, Mesh->GetRelativePos().y);
			auto Body = mBody.lock();
			if (Body)
			{
				Body->AddRelativePos(0, -15);
			}
		}
		else
		{
			Mesh->SetRelativePos(Mesh->GetRelativePos().x, Mesh->GetRelativePos().y - 17.5f);
			Mesh->SetWorldScale(65, 40);

			if (RayBody)
				RayBody->AddRelativePos(0, 15.f);
		}
		if (mShadow)
		{
			mShadow->SetRelativePos(mShadow->GetRelativePos().x, mShadow->GetRelativePos().y - 17.5f);
			mShadow->SetWorldScale(65, 40);
		}
	}
	if (Body)
	{
		Body->SetBoxSize(65, 40);
	}
}

void CPlayer::MoveDownRelease()
{
	if (mIsDead|| mSturnDebuff)
		return;
	mAutoIdle = true;
	mDownKey = false;
	auto	Mesh = mMeshComponent.lock();
	if (!mIsJumping && !mIsAttack|| mIsDown)
	{
		auto Body = mBody.lock();
		auto Anim = mAnimation2DComponent.lock();
		auto RayBody = mRay->GetBox2D().lock();
		
		if (Mesh)
		{
			if (IsHornTailWorld)
			{
				Mesh->SetRelativePos(Mesh->GetRelativePos().x, Mesh->GetRelativePos().y);
				Mesh->SetWorldScale(110.f, 110.f);
				if (Body)
				{
					Body->AddRelativePos(0, 15);
				}
			}
			else
			{
				Mesh->SetRelativePos(Mesh->GetRelativePos().x, Mesh->GetRelativePos().y + 17.5f);
				Mesh->SetWorldScale(65, 70);

				if (RayBody)
					RayBody->SetRelativePos(0, -39);
			}
			if (mShadow)
			{
				mShadow->SetRelativePos(mShadow->GetRelativePos().x, mShadow->GetRelativePos().y + 17.5f);
				mShadow->SetWorldScale(65, 70);
			}
		}
		if (Body)
		{
			Body->SetBoxSize(65, 70);
		}
		if (Anim)
		{
			Anim->ChangeAnimation("PlayerIdle");
			auto ShadowAnim = mShadowAnimation.lock();
			if (ShadowAnim)
				ShadowAnim->ChangeAnimation("ShadowIdle");
		}

		mIsDown = false;
	}

}


void CPlayer::MoveLeft()
{
	if (mIsDead || mSturnDebuff)
		return;
	auto World = mWorld.lock();
	if (mIsDown || mIsAttack || mIsKnockBack)
		return;
	mLeftKey = true;
	auto	Movement = mMovement.lock();
	Movement->AddMove(FVector3(-1.f, 0.f, 0.f));

}

void CPlayer::MoveRight()
{
	if (mIsDead || mSturnDebuff)
		return;
	auto World = mWorld.lock();
	if (mIsDown || mIsAttack || mIsKnockBack)
		return;
	mRightKey = true;
	auto	Movement = mMovement.lock();
	Movement->AddMove(FVector3(1.f, 0.f, 0.f));
}

void CPlayer::MoveUpRelease()
{
	mAutoIdle = true;
	mUpKey = false;
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

void CPlayer::InsertKey()
{
	if (mIsDead || mSturnDebuff)
		return;
	auto Movement = mMovement.lock();
	auto State = mStateComponent.lock();
	auto World = mWorld.lock();
	mIsHeist = true;
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	AssetMng->SoundPlay("Heist");

	std::weak_ptr<CHeistEffect> Effect = World->CreateGameObject<CHeistEffect>("HeistEffect");
	if (IsHornTailWorld)
	{
		Effect.lock()->SetWorldPos(GetWorldPos().x, GetWorldPos().y - 20.f);
	}
	else
	{
		Effect.lock()->SetWorldPos(GetWorldPos());
	}
	if (Movement)
	{
		Movement->SetSpeed(Movement->GetSpeed() + 75);
	}
	auto SkillStateWidget = mPlayerSkillStateWidget.lock();
	if (SkillStateWidget)
	{
		SkillStateWidget->CreateSkillSlot("Heist", TEXT("Slot/Heist.png"), 180.f);
	}
}

void CPlayer::HomeKey()
{
	if (mIsDead || mSturnDebuff)
		return;
	auto World = mWorld.lock();
	
	if (mIsAttack || mIsJumping || mIsDown)
		return;
	if (World)
	{

		std::weak_ptr<CShadowEffect> Effect = World->CreateGameObject<CShadowEffect>("ShadowEffect");
		std::weak_ptr<CShadowEffect2> Effect2 = World->CreateGameObject<CShadowEffect2>("ShadowEffect2");
		auto AssetMng = World->GetWorldAssetManager().lock();
		
		AssetMng->SoundPlay("Shadow");
		auto _Effect2 = Effect2.lock();
		auto	_Effect = Effect.lock();
		if (IsHornTailWorld)
		{
			if (mLastHorizonKey < 0)
			{
				_Effect->SetWorldPos(GetWorldPos().x + 30, GetWorldPos().y + 10);
				_Effect2->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 10);
				_Effect->SetSymmetry(false);
			}
			else
			{
				_Effect->SetWorldPos(GetWorldPos().x - 30, GetWorldPos().y + 10);
				_Effect2->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 10);
				_Effect->SetSymmetry(true);
			}
		}
		else
		{
			if (mLastHorizonKey < 0)
			{
				_Effect->SetWorldPos(GetWorldPos().x + 30, GetWorldPos().y + 30);
				_Effect2->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30);
				_Effect->SetSymmetry(false);
			}
			else
			{
				_Effect->SetWorldPos(GetWorldPos().x - 30, GetWorldPos().y + 30);
				_Effect2->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30);
				_Effect->SetSymmetry(true);
			}
		}
		
		auto SkillStateWidget = mPlayerSkillStateWidget.lock();
		if (SkillStateWidget)
		{
			SkillStateWidget->CreateSkillSlot("Shadow", TEXT("Slot/Shadow.png"), 180.f);
		}
		mShadow->SetEnable(true);
		mShadow->SetOpacity(1.f);
		mShadow->SetWorldPos(GetWorldPos().x + (mLastHorizonKey * -30.f), GetWorldPos().y);
		auto ShadowAnim = mShadow->GetShadowAnim().lock();
		if (ShadowAnim && mIsBoost)
		{
			ShadowAnim->SetPlayRate("ShadowAttack", 2.0f);
			ShadowAnim->SetPlayRate("ShadowAttack1", 2.0f);
			ShadowAnim->SetPlayRate("ShadowAttack2", 2.0f);
			ShadowAnim->SetPlayRate("ShadowAttack3", 3.0f);
		}
		if (mLastHorizonKey > 0)
			mShadow->GetShadowAnim().lock()->SetSymmetry("ShadowIdle", true);
		
	}
}

void CPlayer::PageUpKey()
{
	if (mIsDead || mSturnDebuff)
		return;
	auto Anim = mAnimation2DComponent.lock();
	auto ShadowAnim = mShadowAnimation.lock();
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	AssetMng->SoundPlay("Boost");
	std::weak_ptr<CBoostEffect> Effect = World->CreateGameObject<CBoostEffect>("BoostEffect");
	auto	_Effect = Effect.lock();
	if (IsHornTailWorld)
	{
		_Effect->SetWorldPos(GetWorldPos().x + 15, GetWorldPos().y - 37);

	}
	else
	{
		_Effect->SetWorldPos(GetWorldPos().x + 15, GetWorldPos().y - 17);
	}
	mIsBoost = true;
	auto SkillStateWidget = mPlayerSkillStateWidget.lock();
	if (SkillStateWidget)
	{
		SkillStateWidget->CreateSkillSlot("Boost", TEXT("Slot/Boost.png"), 180.f);
	}
	if (Anim)
	{
		Anim->SetPlayRate("PlayerAttack", 2.0f);
		Anim->SetPlayRate("PlayerAttack1", 2.0f);
		Anim->SetPlayRate("PlayerAttack2", 2.0f);
		Anim->SetPlayRate("PlayerNearAttack", 1.7f);
		Anim->SetPlayRate("PlayerAttack3", 3.0f);

	}
	if (ShadowAnim)
	{
		ShadowAnim->SetPlayRate("ShadowAttack", 2.0f);
		ShadowAnim->SetPlayRate("ShadowAttack1", 2.0f);
		ShadowAnim->SetPlayRate("ShadowAttack2", 2.0f);
		ShadowAnim->SetPlayRate("ShadowAttack3", 3.0f);

	}

}

void CPlayer::PageDownKey()
{
	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	AssetMng->SoundPlay("Will");
	if (World && mWillSkillUse)
	{
		auto Effect = World->CreateGameObject<CWillEffect>("WillEffect").lock();
		FVector3 Pos = GetWorldPos();
		Pos.y += 120.f;
		if (IsHornTailWorld)
		{
			Effect->SetWorldPos(Pos.x,Pos.y - 20.f);
		}
		else
		{
			Effect->SetWorldPos(Pos);
		}
		mWillSkillUse = false;
		if (mDarkDebuff)
		{
			mDarkDebuffTime = 3.f;
			mDarkDebuff = false;
			mDarkEffect->Pause();
		}
		if(mSturnDebuff)
		{
			mSturnDebuffTime = 1.f;
			mSturnDebuff = false;
			mSturnEffect->Pause();
		}
	}

}

void CPlayer::AvengerKey()
{
	/*
	어벤져 애니메이션 모션을 가져와서
	팔을 뒤로 한 모션에서 어벤져를 생성하고 어벤져의 생성 다 끝난 후에 
	팔을 앞으로 가는 애니메이션으로 진행하고 어벤져가 앞으로 가는 로직을 짜야함.
	*/

	if (mIsDead || mSturnDebuff)
		return;
	auto	Anim = mAnimation2DComponent.lock();
	auto	State = mStateComponent.lock();
	if (mIsAttack || !mAvengerUse)
		return;
	if (State)
	{

		
		if (State->GetMP() < 200)
			return;
		State->AddMP(-200.f);
		float mMP = (float)State->GetMP();
		float mMPMax = (float)State->GetMPMax();
		mMPWidgetFunc(mMP, mMPMax);
	}
	mAvengerUse = false;
	if (mIsDown)
		MoveDownRelease();
	if (Anim)
	{
		Anim->ChangeAnimation("PlayerAttack3");
		auto ShadowAnim = mShadowAnimation.lock();
		if (ShadowAnim)
			ShadowAnim->ChangeAnimation("ShadowAttack3");
	}
}

void CPlayer::DeleteKey()
{
	if (mIsDead || mSturnDebuff)
		return;

	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	AssetMng->SoundPlay("HP");
	auto State = mStateComponent.lock();
	{
		State->AddHP(1500.f);
	}
}

void CPlayer::EndKey()
{

	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();
	
	AssetMng->SoundPlay("MP");

	if (mIsDead || mSturnDebuff)
		return;
	auto State = mStateComponent.lock();
	{
		State->AddMP(300.f);
	}

}

void CPlayer::EscKey()
{
	auto Inventory = mInventory.lock();
	if (Inventory)
	{
		if (Inventory->GetEnable())
		{
			Inventory->SetEnable(false);
			return;
		}
		
	}
	auto Equip = mEquipment.lock();
	if (Equip)
	{
		if (Equip->GetEnable())
		{
			Equip->SetEnable(false);
			return;
		}
	}
	auto State = mStatement.lock();
	if (State)
	{
		if (State->GetEnable())
		{
			State->SetEnable(false);
			return;
		}
	}
}


void CPlayer::CheckBreakBottom()
{
	auto Bottom = mBottom.lock();
	auto Mesh = mMeshComponent.lock();
	if (!Bottom || !Mesh)
		return;

	auto Box = std::dynamic_pointer_cast<CColliderBox2D>(Bottom);
	if (!Box)
		return;
	float BoxTopPos = Box->GetInfo().HalfSize.y + Box->GetWorldPos().y;
	float MeshBottomPos = Mesh->GetWorldPos().y - Mesh->GetWorldScale().y * 0.5f;
	if (MeshBottomPos < BoxTopPos)
	{
		mJumpVelocity = 0.f;
		mIsOnGround = true;
		auto Body = mRay->GetBox2D().lock();
		if (Body)
			Body->SetEnable(false);
		Mesh->SetWorldPos(Mesh->GetWorldPos().x, BoxTopPos + Mesh->GetWorldScale().y * 0.5f);
	}
}
void CPlayer::AvengerNotify()
{
	auto Mesh = mMeshComponent.lock();
	auto State = mStateComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto ShadowAnim = mShadowAnimation.lock();

	auto World = mWorld.lock();
	auto AssetMng = World->GetWorldAssetManager().lock();

	AssetMng->SoundPlay("Avenger");

	
	if (Mesh)
	{
		Mesh->SetWorldScale(90, 70);
		if (IsHornTailWorld)
		{
			Mesh->SetWorldScale(110, 110);

		}

	}
	if (Anim)
	{
		Anim->SetPlayRate("PlayerAttack3", 0.f);
	}

	if (ShadowAnim)
		ShadowAnim->SetPlayRate("ShadowAttack3", 0.f);
	mIsAttack = true;
	mShurikenY = GetWorldPos().y;
	if (IsHornTailWorld)
		mShurikenY -= 20.f;
	mShurikenX = GetWorldPos().x + mLastHorizonKey * -50;
	std::weak_ptr<CAvenger> Avenger = mWorld.lock()->CreateGameObject<CAvenger>("Avenger");
	
	auto _Avenger = Avenger.lock();
	if (_Avenger)
	{
		_Avenger->SetDistance(500.f);
		_Avenger->SetOwner(this);
		_Avenger->SetMoveDir(FVector3((float)mLastHorizonKey, 0, 0));
		_Avenger->SetWorldPos(mShurikenX, mShurikenY);
		if (mDarkDebuff)
		{
			_Avenger->SetDamage(0);
		}
		else
		{
			_Avenger->SetDamage(float(rand() % mDamageRange + mAvengerDamage));
		}

	}
	if (mShadow->GetEnable())
	{
		mShadowAvengerCount = 1;     
		mShadowAvengerDelay = 0.5f;
		mShadowAvengerDir = mLastHorizonKey;

	}
}

void CPlayer::DropKey()
{
	std::vector <std::weak_ptr<CCollider>>ItemList;


	auto CollisionMap = mBody.lock()->GetCollisionObjectMap();

	if (!CollisionMap.empty())
	{
		auto iter = CollisionMap.begin();
		auto iterEnd = CollisionMap.end();
		for (; iter != iterEnd; iter++)
		{
			auto Collider = iter->second.lock();
			if (!Collider || !Collider->GetEnable())
				continue;

			if (Collider->GetCollisionProfile()->Name == "Item")
			{
				auto Inventory = mInventory.lock();
				if (Inventory)
				{
					auto Item = std::dynamic_pointer_cast<CFieldItem>(Collider->GetOwner().lock());
					if (Item)
					{
						if (Inventory->GetEmptyIndex(Item->GetItem()->GetData().Type) != -1)
						{
							Inventory->AddItem(Item->GetItem());
							Item->DropInventory();
							break;
						}
					}
				}
			}
		}
	}
}

void CPlayer::ResumeAvengerAnim()
{

	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		if(mIsBoost)
			Anim->SetPlayRate("PlayerAttack3", 3.f);
		else
			Anim->SetPlayRate("PlayerAttack3", 1.7f);
	}
	auto ShadowAnim = mShadowAnimation.lock();
	if (ShadowAnim)
	{
		if (mIsBoost)
			ShadowAnim->SetPlayRate("ShadowAttack3", 3.f);
		else
			ShadowAnim->SetPlayRate("ShadowAttack3", 1.7f);
	}
}
void CPlayer::EndBuff(std::string& BuffName)
{
	auto Movement = mMovement.lock();
	auto Anim = mAnimation2DComponent.lock();
	if (BuffName == "Heist")
	{
		Movement->SetSpeed(Movement->GetSpeed() - 75);
		mIsHeist = false;
	}
	else if (BuffName == "Shadow")
	{
		mShadow->SetEnable(false);
		mShadow->SetOpacity(0);
	}
	else if (BuffName == "Boost")
	{

		Anim->SetPlayRate("PlayerAttack", 1.3f);
		Anim->SetPlayRate("PlayerAttack1", 1.3f);
		Anim->SetPlayRate("PlayerAttack2", 1.3f);
		Anim->SetPlayRate("PlayerAttack3", 1.f);
		Anim->SetPlayRate("PlayerNearAttack", 1.3f);
		Anim->SetPlayRate("ShadowAttack", 1.3f);
		Anim->SetPlayRate("ShadowAttack1", 1.3f);
		Anim->SetPlayRate("ShadowAttack2", 1.3f);
		Anim->SetPlayRate("ShadowAttack3", 1.0f);

	}
}
void CPlayer::CreateShuriken()
{
	std::weak_ptr<CShuriken> mShuriken = mWorld.lock()->CreateGameObject<CShuriken>("Shuriken");
	std::shared_ptr<CShuriken>	Shuriken = mShuriken.lock();
	auto	Mesh = mMeshComponent.lock();
	float RandomY = (float)(rand() % 10 - 5);
	if (IsHornTailWorld)
		RandomY -= 20.f;
	Shuriken->SetWorldPos(mShurikenX, mShurikenY + RandomY);
	Shuriken->SetMoveDir(mShurikenDir);
	if (mDarkDebuff)
	{
		Shuriken->SetDamage(0);
	}
	else
	{
		Shuriken->SetDamage(float(rand() % mDamageRange + mThrowDamage));
	}
}

void CPlayer::AttackPress()
{

	if (mIsDead || mSturnDebuff)
		return;
	auto	Anim = mAnimation2DComponent.lock();
	auto	Movement = mMovement.lock();
	auto	Mesh = mMeshComponent.lock();
	auto State = mStateComponent.lock();
	
	if (mIsAttack)
		return;
	if (mNearCount >= 1)
	{
		mIsAttack = true;
		mIsNearAttack = true;
		Anim->ChangeAnimation("PlayerNearAttack");
		return;
	}
	if (State)
	{
		if (State->GetMP() < 28)
			return;
		State->AddMP(-28.f);
		float mMP = (float)State->GetMP();
		float mMPMax = (float)State->GetMPMax();
		mMPWidgetFunc(mMP, mMPMax);
	}

	mIsAttack = true;
	mShurikenLastHorizon = mLastHorizonKey;
	mShurikenY = GetWorldPos().y;
	mShurikenX = GetWorldPos().x  + mLastHorizonKey * 30;
	if (mIsDown)
		MoveDownRelease();
	if (Anim)
	{	
		int AnimIndex = 0;
		AnimIndex = rand() % 3;

		Anim->ChangeAnimation(mAttackAnimList[AnimIndex]);
		auto ShadowAnim = mShadowAnimation.lock();
		if (ShadowAnim)
			ShadowAnim->ChangeAnimation(mShadow->GetAttackAnimName(AnimIndex));
	}
	mShurikenCount = 2;
	if (mShadow->GetEnable())
		mShurikenCount = 5;

	mShurikenDelayTime = 0.1f;
	std::weak_ptr<CShuriken> mShuriken = mWorld.lock()->CreateGameObject<CShuriken>("Shuriken");
	std::shared_ptr<CShuriken> Shuriken = mShuriken.lock();
	if (mDarkDebuff)
	{
		Shuriken->SetDamage(0);
	}
	else
	{
		Shuriken->SetDamage(float(rand() % mDamageRange + mThrowDamage));
	}
		
	float RandomY = (float)(rand() % 10 - 5);
	if (IsHornTailWorld)
		RandomY -= 20.f;
	Shuriken->SetWorldPos(mShurikenX, mShurikenY + RandomY);
	Shuriken->SetNearTarget("Monster", float(mLastHorizonKey));
	mShurikenDir = Shuriken->GetMoveDir();
	if (!mShurikenDir.Distance(FVector3::Zero))
		mShurikenDir = FVector3(float(mShurikenLastHorizon), 0, 0);
	Shuriken->SetMoveDir(mShurikenDir);
}

void CPlayer::AttackHold()
{
}

void CPlayer::AttackRelease()
{

}

void CPlayer::AttackEndNotify()
{
	mIsAttack = false;
	mAutoIdle = true;
}

void CPlayer::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	if (mIsDead)
		return;
	// 무적이 아닐때 맞고 여기서 데미지가 0일때를 체크해야함.
	auto Owner = Dest->GetOwner().lock();
	auto State = mStateComponent.lock();

	auto Body = mBody.lock();
	if (!mIsInvincible)
	{
		TakeDamage(-1);
		mInvincibleTime = 1.0f;
		mIsInvincible = true;
		Body->ClearCollisionList();
	}
	if (Dest->GetCollisionProfile()->Name == "Gravity")
	{
		// 위에서 아래로 떨어지는 중일 때만 바닥으로 인정
		if (mJumpVelocity <= 0.f)
		{
			mIsOnGround = true;
			mJumpVelocity = 0.f;

			// 위치 보정 (Snap to Floor)
			float FloorTop = Dest->GetWorldPos().y + dynamic_cast<CColliderBox2D*>(Dest)->GetInfo().HalfSize.y;
			FVector3 Pos = GetWorldPos();
			Pos.y = FloorTop + GetWorldScale().y * 0.5f;
			SetWorldPos(Pos);
		}
	}
}

void CPlayer::RayCollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	auto	Mesh = mMeshComponent.lock();
	auto Owner = Dest->GetOwner().lock();
	auto Movement = mMovement.lock();

	if (Dest->GetCollisionProfile()->Name == "Portal")
		return;

	if (Dest->GetCollisionProfile()->Name == "Gravity")
	{
		if (mJumpVelocity > 0.f)
			return;
		mIsOnGround = true;
		mJumpVelocity = 0.f;
		mJumpCount = 0;
		mIsJumping = false;
		mFlashJumpVelocity = 0.f;

		CColliderBox2D* Box = dynamic_cast<CColliderBox2D*>(Dest);
		FVector3 CurrentPos = GetWorldPos();
		CurrentPos.y = Dest->GetWorldPos().y + Box->GetInfo().HalfSize.y + GetWorldScale().y * 0.5f;
		SetWorldPos(CurrentPos);
		mRay->SetCollisionName(Dest->GetName());
	}
}

void CPlayer::RayCollisionEnd(CCollider* Dest)
{
	if (!mIsJumping)
	{
		mIsOnGround = false;
	}
}

void CPlayer::WallRayCollisionBegin(const FVector3& HitPoint, CCollider* Dest)
{
	auto	Mesh = mMeshComponent.lock();
	auto Owner = Dest->GetOwner().lock();
	auto Movement = mMovement.lock();

	if (Dest->GetCollisionProfile()->Name == "Portal")
		return;

	if (Dest->GetCollisionProfile()->Name == "Gravity" && Dest->GetName() == "Bottom")
	{
		if (mJumpVelocity > 0.f || mJumpVelocity < 0.f)
			return;

		CColliderBox2D* Box = dynamic_cast<CColliderBox2D*>(Dest);
		FVector3 Vel = GetVelocity();

		FVector3 CurrentPos = GetWorldPos();
		float PlayerHalfWidth = GetWorldScale().x * 0.5f;
		float WallHalfWidth = Box->GetInfo().HalfSize.x;
		float WallPosX = Dest->GetWorldPos().x;
		SetVelocity(FVector3(0, GetVelocity().y, 0));
		if (GetWorldPos().x < WallPosX)
		{
			CurrentPos.x = WallPosX - WallHalfWidth - PlayerHalfWidth;
		}
		else 
		{
			CurrentPos.x = WallPosX + WallHalfWidth + PlayerHalfWidth;
		}
		SetWorldPos(CurrentPos);
		mJumpVelocity = 0.f;
		mFlashJumpVelocity = 0.f;
		mWallRay->SetCollisionName(Dest->GetName());

	}

}

void CPlayer::WallRayCollisionEnd(CCollider* Dest)
{

}

void CPlayer::OpenInventory()
{
	auto Inventory = mInventory.lock();
	if (Inventory)
	{
		if (Inventory->GetEnable())
			Inventory->SetEnable(false);
		else
			Inventory->SetEnable(true);
	}
}

void CPlayer::OpenEquipment()
{
	auto Equip = mEquipment.lock();
	if (Equip)
	{
		if(Equip->GetEnable())
			Equip->SetEnable(false);
		else
			Equip->SetEnable(true);
	}
}

void CPlayer::OpenStatement()
{
	auto State = mStatement.lock();
	if (State)
	{
		if (State->GetEnable())
			State->SetEnable(false);
		else
			State->SetEnable(true);
	}
}

void CPlayer::UpdateSturnDebuff(float DeltaTime)
{
	if (mSturnDebuff)
	{
		mSturnDebuffTime -= DeltaTime;
		mSturnEffect->Resume();
		mSturnEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 20.f);
		if (mSturnDebuffTime <= 0)
		{
			mSturnDebuffTime = 2.f;
			mSturnDebuff = false;

			mSturnEffect->Pause();
		}
	}
}

void CPlayer::UpdateDarkDebuff(float DeltaTime)
{
	if (mDarkDebuff)
	{
		mDarkDebuffTime -= DeltaTime;
		mDarkEffect->Resume();
		mDarkEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 40.f);
		if (mDarkDebuffTime <= 0)
		{
			mDarkDebuffTime = 3.f;
			mDarkDebuff = false;
			mDarkEffect->Pause();
		}
	}
}

void CPlayer::UpdateStateWidget(float DeltaTime)
{

	auto State = mStateComponent.lock();

	if (State)
	{
		float RealHP = State->GetHP();
		float HPMax = State->GetHPMax();
		if (mCurrentShowHP != RealHP)
		{
			mCurrentShowHP += (RealHP - mCurrentShowHP) * DeltaTime * 5.0f;

			if (fabsf(RealHP - mCurrentShowHP) < 0.5f)
				mCurrentShowHP = RealHP;

			auto iter = mHPWidgetFunc.begin();
			for (; iter != mHPWidgetFunc.end(); ++iter)
			{
				(*iter)(mCurrentShowHP, HPMax);
			}
		}

		float RealMP = State->GetMP();
		float MPMax = State->GetMPMax();
		if (mCurrentShowMP != RealMP)
		{
			mCurrentShowMP += (RealMP - mCurrentShowMP) * DeltaTime * 5.0f;
			if (fabsf(RealMP - mCurrentShowMP) < 0.5f)
				mCurrentShowMP = RealMP;

			if (mMPWidgetFunc)
				mMPWidgetFunc(mCurrentShowMP, MPMax);
		}
	}
}

void CPlayer::UpdateVelocity(float DeltaTime)
{

	if (mIsOnGround)
	{
		mJumpVelocity = 0.f;
		mJumpCount = 0;
		mIsJumping = false;
	}
	else
	{
		mJumpVelocity -= 1700 * DeltaTime;

		if (mFlashJumpVelocity > 0.f)
		{
			mFlashJumpVelocity -= 1500.f * DeltaTime;
			if (mFlashJumpVelocity < 0.f)
				mFlashJumpVelocity = 0.f;
		}
		else if (mFlashJumpVelocity < 0.f)
		{
			mFlashJumpVelocity += 1500.f * DeltaTime;
			if (mFlashJumpVelocity > 0.f)
				mFlashJumpVelocity = 0.f;
		}
		if (mJumpVelocity <= 0.f) {
			auto Body = mRay->GetBox2D().lock();
			if (Body)
				Body->SetEnable(true);
			CheckBreakBottom();

		}
	}

	AddWorldPos(FVector3(mFlashJumpVelocity * DeltaTime, mJumpVelocity * DeltaTime, 0.f));

}

void CPlayer::CheckNearMonster()
{
	auto AttackBody = mAttackColider.lock();

	if (AttackBody)
	{
		auto Map = AttackBody->GetCollisionObjectMap();
		mNearCount = (int)Map.size();
		AttackBody->ClearCollisionList();
	}
}

void CPlayer::UpdateInvincible(float DeltaTime)
{
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

bool CPlayer::CheckDead()
{
	auto World = mWorld.lock();
	if (mIsDead)
	{
		auto Body = mBody.lock();
		auto AttackBody = mAttackColider.lock();
		auto	Anim = mAnimation2DComponent.lock();

		Anim->ChangeAnimation("PlayerDead");
		Body->SetEnable(false);
		AttackBody->SetEnable(false);
		if (mShadow)
			mShadow->Destroy();

		std::weak_ptr<CEndingWidget>	EndingWidget =
			World->GetUIManager().lock()->CreateWidget<CEndingWidget>("EndingWidget");
		return false;
	}
	return true;
}

void CPlayer::UpdateKnockBack(float DeltaTime)
{
	if (mKnockbackSpeed > 0.f)
	{
		AddWorldPos(mKnockbackDir * mKnockbackSpeed * DeltaTime);
		mKnockbackSpeed -= 1500.f * DeltaTime;
		if (mKnockbackSpeed <= 0)
			mIsKnockBack = false;
	}
}

void CPlayer::UpdateAvengerWidget(float DeltaTime)
{
	if (!mAvengerUse)
	{
		if (mAvengerWidgetFunc)
		{
			mAvengerWidgetFunc(mAvengerCooltime, 2.0f);
		}
		mAvengerCooltime -= DeltaTime;
		if (mAvengerCooltime <= 0)
		{
			mAvengerUse = true;
			mAvengerCooltime = 2.f;
		}
	}

	
}

void CPlayer::UpdateWillWidget(float DeltaTime)
{
	if (!mWillSkillUse)
	{
		if (mWillSkillWidgetFunc)
		{
			mWillSkillWidgetFunc(mWillSkillCoolTime, 15.0f);
		}
		mWillSkillCoolTime -= DeltaTime;
		if (mWillSkillCoolTime <= 0)
		{
			mWillSkillUse = true;
			mWillSkillCoolTime = 15.f;
		}
	}
}

void CPlayer::UpdateCreateShuriken(float DeltaTime)
{
	if (mShurikenCount > 0)
	{
		mShurikenDelayTime -= DeltaTime;

		if (mShurikenDelayTime <= 0.f)
		{
			CreateShuriken();
			mShurikenCount--;
			mShurikenDelayTime = mShurikenDelayMax;
		}
	}
}

void CPlayer::UpdateCreateAvenger(float DeltaTime)
{

	if (mShadowAvengerCount > 0)
	{
		mShadowAvengerDelay -= DeltaTime;
		if (mShadowAvengerDelay <= 0.f)
		{
			// 그림자 표창 생성
			std::weak_ptr<CAvenger> Avenger = mWorld.lock()->CreateGameObject<CAvenger>("Avenger");
			auto _Avenger = Avenger.lock();
			if (_Avenger)
			{
				_Avenger->SetOwner(this);
				_Avenger->SetMoveDir(FVector3((float)mShadowAvengerDir, 0, 0));
				_Avenger->SetWorldPos(mShurikenX + mShadowAvengerDir * 100, mShurikenY);
				if (mDarkDebuff)
				{
					_Avenger->SetDamage(0);
				}
				else
				{
					_Avenger->SetDamage(float(rand() % mDamageRange + mAvengerDamage));
				}
			}

			mShadowAvengerCount--;
			mShadowAvengerDelay = 0.5f;
		}
	}
}

void CPlayer::UpdateDirection()
{
	FVector3 Current;
	if (mUpKey)
		Current.y += 1.f;
	if (mDownKey)
		Current.y -= 1.f;
	if (mLeftKey)
		Current.x -= 1.f;
	if (mRightKey)
		Current.x += 1.f;

	if (!Current.IsZero())
	{
		Current.Normalize();
		mDir = Current;
	}

	if (Current.x < 0.f)
	{
		mLastHorizonKey = -1;
	}
	else if (Current.x > 0.f)
	{
		mLastHorizonKey = 1;
	}
}

void CPlayer::UpdateAnimationDirection()
{

	auto ShadowAnim = mShadowAnimation.lock();
	auto Anim = mAnimation2DComponent.lock();
	if (mLastHorizonKey < 0.f)
	{

		Anim->SetSymmetry("PlayerWalk", false);
		Anim->SetSymmetry("PlayerIdle", false);
		Anim->SetSymmetry("PlayerAttack", false);
		Anim->SetSymmetry("PlayerAttack1", false);
		Anim->SetSymmetry("PlayerAttack2", false);
		Anim->SetSymmetry("PlayerAttack3", false);
		Anim->SetSymmetry("PlayerJump", false);
		Anim->SetSymmetry("PlayerDown", false);
		Anim->SetSymmetry("PlayerNearAttack", false);
		Anim->SetSymmetry("PlayerDead", false);
		if (ShadowAnim)
		{
			ShadowAnim->SetSymmetry("ShadowWalk", false);
			ShadowAnim->SetSymmetry("ShadowIdle", false);
			ShadowAnim->SetSymmetry("ShadowAttack", false);
			ShadowAnim->SetSymmetry("ShadowAttack1", false);
			ShadowAnim->SetSymmetry("ShadowAttack2", false);
			ShadowAnim->SetSymmetry("ShadowAttack3", false);
			ShadowAnim->SetSymmetry("ShadowJump", false);
			ShadowAnim->SetSymmetry("ShadowDown", false);
		}



	}
	else if (mLastHorizonKey > 0.f)
	{

		Anim->SetSymmetry("PlayerWalk", true);
		Anim->SetSymmetry("PlayerIdle", true);
		Anim->SetSymmetry("PlayerAttack", true);
		Anim->SetSymmetry("PlayerAttack1", true);
		Anim->SetSymmetry("PlayerAttack2", true);
		Anim->SetSymmetry("PlayerAttack3", true);
		Anim->SetSymmetry("PlayerJump", true);
		Anim->SetSymmetry("PlayerDown", true);
		Anim->SetSymmetry("PlayerNearAttack", true);
		Anim->SetSymmetry("PlayerDead", true);
		if (ShadowAnim)
		{
			ShadowAnim->SetSymmetry("ShadowWalk", true);
			ShadowAnim->SetSymmetry("ShadowIdle", true);
			ShadowAnim->SetSymmetry("ShadowAttack", true);
			ShadowAnim->SetSymmetry("ShadowAttack1", true);
			ShadowAnim->SetSymmetry("ShadowAttack2", true);
			ShadowAnim->SetSymmetry("ShadowAttack3", true);
			ShadowAnim->SetSymmetry("ShadowJump", true);
			ShadowAnim->SetSymmetry("ShadowDown", true);
		}

	}
}

void CPlayer::UpdateAttackBodyPosition()
{
	auto AttackBody = mAttackColider.lock();
	if (AttackBody)
	{
		if (IsHornTailWorld)
		{
			AttackBody->SetWorldPos(GetWorldPos().x + mLastHorizonKey * 50, GetWorldPos().y - 20);
		}
		else
		{
			AttackBody->SetWorldPos(GetWorldPos().x + mLastHorizonKey * 50, GetWorldPos().y);
		}
	}

	
}

void CPlayer::UpdateWallRay()
{
	if (mWallRay)
	{
		if (mIsDown)
			mWallRay->SetEnable(false);
		else
			mWallRay->SetEnable(true);

		if (IsHornTailWorld)
		{
			mWallRay->SetWorldPos(GetWorldPos().x + mLastHorizonKey * 35, GetWorldPos().y - 20.f);
			mWallRay->ClearCollision();
		}
		else
		{
			mWallRay->SetWorldPos(GetWorldPos().x + mLastHorizonKey * 35, GetWorldPos().y);
			mWallRay->ClearCollision();
		}
	}
}

void CPlayer::UpdateMoveAnimation()
{
	auto ShadowAnim = mShadowAnimation.lock();
	auto Anim = mAnimation2DComponent.lock();
	if (!mIsAttack && !mIsDown)
	{
		if (mSturnDebuff)
		{
			MoveLeftRelease();
			MoveRightRelease();
			if (mIsDown)
				MoveDownRelease();
			Anim->ChangeAnimation("PlayerIdle");
			if (ShadowAnim)
			{
				ShadowAnim->ChangeAnimation("ShadowIdle");
			}
		}
		if (!mIsOnGround)
		{
			Anim->ChangeAnimation("PlayerJump");
			if (ShadowAnim)
			{
				ShadowAnim->ChangeAnimation("ShadowJump");
			}
		}
		else if (mLeftKey || mRightKey)
		{
			Anim->ChangeAnimation("PlayerWalk");
			if (ShadowAnim)
			{
				ShadowAnim->ChangeAnimation("ShadowWalk");
			}
		}
		else
		{
			Anim->ChangeAnimation("PlayerIdle");
			if (ShadowAnim)
			{
				ShadowAnim->ChangeAnimation("ShadowIdle");
			}
		}
	}
}

void CPlayer::UpdateShadowPosition()
{

	if (mShadow->GetEnable())
	{
		if (IsHornTailWorld)
		{
			mShadow->SetWorldPos(GetWorldPos().x + mLastHorizonKey * -30.f, GetWorldPos().y - 20.f);
		}
		else
		{
			mShadow->SetWorldPos(GetWorldPos().x + mLastHorizonKey * -30.f, GetWorldPos().y);
		}
	}
}

void CPlayer::CheckShadowEnable()
{
	
	if (mShadow->GetEnable())
		mShadowAnimation = mShadow->GetShadowAnim();
}



float CPlayer::TakeDamage(float Damage, bool KnockBack)
{
	if (mIsDead)
		return 0;
	if (mIsInvincible)
		return 0;
	auto State = mStateComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	if (State)
	{
		mIsInvincible = true;
		mInvincibleTime = 1.0f;
		State->AddHP(-Damage);
		float mHP = (float)State->GetHP();
		float mHPMax = (float)State->GetHPMax();
		auto	iter = mHPWidgetFunc.begin();
		auto	iterEnd = mHPWidgetFunc.end();
		if (KnockBack)
		{
			mKnockbackSpeed = 300.f;
			mKnockbackDir = FVector3((float)mLastHorizonKey * -1.f, 0.f, 0.f);
			mJumpVelocity = 250.f;
			mIsOnGround = false;
			mIsJumping = true;
			mIsKnockBack = true;
		}
		for (; iter != iterEnd; ++iter)
		{
			(*iter)(mHP, mHPMax);
		}

		if (State->IsDead())
		{
			auto World = mWorld.lock();
			auto Grave = World->CreateGameObject<CGrave>("Grave").lock();
			auto Mesh = mMeshComponent.lock();
			if (Mesh)
			{
				if (mLastHorizonKey < 0)
				{	
					Grave->SetSymmetry(false);
				}
				else
				{
					Grave->SetSymmetry(true);
				}

				Grave->SetWorldPos(Mesh->GetWorldPos().x - mLastHorizonKey*10, Mesh->GetWorldPos().y);
				Mesh->AddWorldPos(0.f, 30.f);
			}
			IsHornTailWorld = false;
			SetZakumEquip();
			World->SetPlayerIsDead(true);	
			mIsDead = true;

			return 0;
		}
	}
	return Damage;
}



void CPlayer::SetHornTailWorld(bool HornTailWorld)
{
	IsHornTailWorld = HornTailWorld;
	if (IsHornTailWorld)
	{
		mMeshComponent.lock()->SetWorldScale(110.f, 110.f);
		mBody.lock()->AddRelativePos(0, -20.f);
		mRay->AddRelativePos(0, -20.f);
		
		mShadow->AddRelativePos(0, -20.f);
		mAttackColider.lock()->AddRelativePos(0, -20.f);
		mStateComponent.lock()->SetHPMax(6358.f);
		mStateComponent.lock()->SetMPMax(873.f);
		mThrowDamage = 7248.f;
		mAvengerDamage = 13434.f;
	}

}

void CPlayer::AddFireEye()
{
	std::shared_ptr<CItem> Item = std::make_shared<CItem>();

	FDropItemData Data;
	Data.Name = "FireEye";
	Data.IconPath = L"Slot/FireEye.png";
	Data.Type = EItemType::Etc;
	Item->SetData(Data);
	CInventoryManager::GetInst()->AddItem(Item);
}

void CPlayer::SetHP(float HP,bool KnockBack)
{
	if (mIsDead)
		return;
	if (mIsInvincible)
		return;
	auto State = mStateComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	if (State)
	{
		mIsInvincible = true;
		mInvincibleTime = 1.0f;
		State->SetHP(HP);
		float mHP = (float)State->GetHP();
		float mHPMax = (float)State->GetHPMax();
		auto	iter = mHPWidgetFunc.begin();
		auto	iterEnd = mHPWidgetFunc.end();
		if (KnockBack)
		{
			mKnockbackSpeed = 300.f;
			mKnockbackDir = FVector3((float)mLastHorizonKey * -1.f, 0.f, 0.f);
			mJumpVelocity = 250.f;
			mIsOnGround = false;
			mIsJumping = true;
			mIsKnockBack = true;
		}
		for (; iter != iterEnd; ++iter)
		{
			(*iter)(mHP, mHPMax);
		}
	}
}