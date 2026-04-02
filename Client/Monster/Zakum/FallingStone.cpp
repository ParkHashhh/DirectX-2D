#include "FallingStone.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "ArmSkill.h"
CFallingStone::CFallingStone()
{
	SetClassType<CFallingStone>();
}

CFallingStone::CFallingStone(const CFallingStone& ref) :
	CGameObject(ref)
{
}

CFallingStone::CFallingStone(CFallingStone&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CFallingStone::~CFallingStone()
{
}

bool CFallingStone::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("FallingStoneMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("FallingStoneAnimation2D");
	mBody = CreateComponent<CColliderBox2D>("FallingStoneBody");
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto	Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(200.f, 400.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialBaseColor(0, FVector4(1.f, 1.f, 1.f, 1));
		Mesh->SetRenderLayer(0);
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("FallingStone");
		Anim->SetFinishNotify<CFallingStone>("FallingStone",
			this, &CFallingStone::FinishAnimation);
		Anim->AddNotify<CFallingStone>("FallingStone",
			"FallingStone",7, this, &CFallingStone::Pause);
	}
	if (Body)
	{
		Body->SetBoxSize(50.f, 150.f);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("MonsterAttack");
		Body->SetCollisionBeginFunction<CFallingStone>(this, &CFallingStone::OnHit);
	}




	return true;
}
void CFallingStone::Pause()
{
	auto	Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->SetPlayRate("FallingStone", 0.f);
	}
}



void CFallingStone::Update(float DeltaTime)
{
	auto	Mesh = mMeshComponent.lock();
	CGameObject::Update(DeltaTime);
	Mesh->AddWorldPos(0, -mSpeed * DeltaTime, 0);

}


CFallingStone* CFallingStone::Clone()
{
	return new CFallingStone(*this);
}
void CFallingStone::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	auto	Body = mBody.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto	Mesh = mMeshComponent.lock();
	FVector3 DestPos = Dest->GetWorldPos();
	auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		if (Owner)
		{
			mSpeed = 0;
			Owner->TakeDamage(Owner->GetHPMax() * 0.50f);
			if (Mesh)
			{
				Mesh->SetWorldPos(GetWorldPos());
				Mesh->SetPivot(0, -0.25f);
			}
			if (Body)
			{
				Body->SetEnable(false);
			}

			if (Anim)
			{
				Anim->SetPlayRate("FallingStone", 1.f);
			}
		}
	}
	else if (DestPos.x == mTargetPos.x && DestPos.y == mTargetPos.y)
	{
		mSpeed = 0;
		if (Mesh)
		{
			Mesh->SetWorldPos(GetWorldPos());
			Mesh->SetPivot(0, -0.25f);

		}
		if (Body)
		{
			Body->SetEnable(false);
		}

		if (Anim)
		{
			Anim->SetPlayRate("FallingStone", 1.f);
		}
	}

	
}


void CFallingStone::FinishAnimation()
{
	Destroy();
}
