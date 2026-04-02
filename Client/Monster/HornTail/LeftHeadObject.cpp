#include "LeftHeadObject.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
CLeftHeadObject::CLeftHeadObject()
{
	SetClassType<CLeftHeadObject>();
}

CLeftHeadObject::CLeftHeadObject(const CLeftHeadObject& ref) :
	CGameObject(ref)
{
}

CLeftHeadObject::CLeftHeadObject(CLeftHeadObject&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CLeftHeadObject::~CLeftHeadObject()
{
}

bool CLeftHeadObject::Init()
{
	CGameObject::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("LeftHeadObjectMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("LeftHeadObjectAnimation2D");
	mBody = CreateComponent<CColliderBox2D>("LeftHeadObjectBody");
	auto	Mesh = mMeshComponent.lock();
	auto	Anim = mAnimation2DComponent.lock();
	auto	Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(200.f, 200.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailLeftHead_Object");
		Anim->SetPlayRate("HornTailLeftHead_Object",0.5f);
		Anim->SetFinishNotify<CLeftHeadObject>("HornTailLeftHead_Object",
			this, &CLeftHeadObject::FinishAnimation);
		Anim->AddNotify<CLeftHeadObject>("HornTailLeftHead_Object",
			"HornTailLeftHead_Object", 29, this, &CLeftHeadObject::EnableBody);

		Anim->AddNotify<CLeftHeadObject>("HornTailLeftHead_Object",
			"HornTailLeftHead_Object", 31, this, &CLeftHeadObject::DisableBody);
	}
	if (Body)
	{
		Body->SetBoxSize(50.f, 50.f);
		Body->SetDebugDraw(false);
		Body->SetEnable(false);
		Body->SetCollisionProfile("MonsterAttack");
		Body->SetCollisionBeginFunction<CLeftHeadObject>(this, &CLeftHeadObject::OnHit);
	}




	return true;
}
void CLeftHeadObject::EnableBody()
{
	auto	Body = mBody.lock();
	auto	Mesh = mMeshComponent.lock();

	if (Body)
	{
		Body->SetEnable(true);
		Body->SetWorldPos(Mesh->GetWorldPos().x , Mesh->GetWorldPos().y - 60);
	}
}

void CLeftHeadObject::DisableBody()
{
	auto	Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(false);
	}
}




void CLeftHeadObject::Update(float DeltaTime)
{
	auto	Mesh = mMeshComponent.lock();
	CGameObject::Update(DeltaTime);

}


CLeftHeadObject* CLeftHeadObject::Clone()
{
	return new CLeftHeadObject(*this);
}
void CLeftHeadObject::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	FVector3 DestPos = Dest->GetWorldPos();
	auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.50f);
		}
	}

}


void CLeftHeadObject::FinishAnimation()
{
	Destroy();
}
