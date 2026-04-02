#include "Poison.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
CPoison::CPoison()
{
	SetClassType<CPoison>();
}

CPoison::CPoison(const CPoison& ref) :
	CGameObject(ref)
{
}

CPoison::CPoison(CPoison&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CPoison::~CPoison()
{
}

bool CPoison::Init()
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
		Mesh->SetWorldScale(600.f, 400.f);
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetMaterialOpacity(0, 0.5f);
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("Poison");
		Anim->SetPlayRate("Poison", 0.5f);
		Anim->SetLoop("Poison", true);
		
	}
	if (Body)
	{
		Body->SetBoxSize(400.f, 300.f);
		Body->SetDebugDraw(false);
		Body->SetEnable(true);
		Body->SetCollisionProfile("MonsterAttack");
		Body->SetCollisionBeginFunction<CPoison>(this, &CPoison::OnHit);
	}




	return true;
}

void CPoison::Update(float DeltaTime)
{
	auto	Mesh = mMeshComponent.lock();
	CGameObject::Update(DeltaTime);
	mPoisonTime -= DeltaTime;
	mPoisonTickTime -= DeltaTime;

	if (mPoisonTime <= 0)
	{
		Destroy();
	}
	if (mPoisonTickTime <= 0)
	{
		auto Body = mBody.lock();
		Body->ClearCollisionList();
		mPoisonTickTime = 0.01f;
	}

}


CPoison* CPoison::Clone()
{
	return new CPoison(*this);
}

void CPoison::OnHit(const FVector3& HitPoint, CCollider* Dest)
{
	FVector3 DestPos = Dest->GetWorldPos();
	auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.1f,false);
		}
	}

}
