#include "Portal.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "../Player/Player.h"
#include "World/WorldManager.h"
#include "../World/LoadingWorld.h"
#include "../World/ZakumWorld.h"
#include "../World/MainWorld.h"
#include "../UI/FadeOutWidget.h"

CPortal::CPortal()
{
}

CPortal::CPortal(const CPortal& ref)
	: CGameObject(ref)
{
}

CPortal::CPortal(CPortal&& ref) noexcept
	: CGameObject(std::move(ref))
{
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	CGameObject::Init();
	mMeshComponent = CreateComponent<CMeshComponent>("Portal");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("PortalAnim");
	mBody = CreateComponent<CColliderBox2D>("PortalBody");
	auto Mesh = mMeshComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Body = mBody.lock();

	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetRelativeScale(80.f, 120.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation("Portal");
		Anim->SetLoop("Portal",true);
	}

	if (Body)
	{
		Body->SetCollisionProfile("Portal");
		Body->SetBoxSize(80.f, 30.f);
		Body->SetWorldPos(Mesh->GetWorldPos().x, Mesh->GetWorldPos().y - 40.f);
		Body->SetDebugDraw(false);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CPortal>(this, &CPortal::CollisionBegin);
		Body->SetCollisionEndFunction<CPortal>(this, &CPortal::CollisionEnd);
	}

	auto World = mWorld.lock();
	auto UIMng = World->GetUIManager().lock();
	mFadeOut = UIMng->CreateWidget<CFadeOutWidget>("FadeOutWidget" + std::to_string(rand()), 999);
	
	return true;
}

void CPortal::CollisionBegin(const FVector3& HitPoint,CCollider* Dest)
{
	auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
	if(Owner)
		Owner->SetTargetObject(this);
}
void CPortal::Interact()
{
	// 여기서 포탈 이동
	auto CurrentWorld = mWorld.lock();
	auto UIMng = CurrentWorld->GetUIManager().lock();
	auto FadeOut = 	mFadeOut.lock();
	if (FadeOut)
	{
		FadeOut->SetFadeOutSpeed(1.f);
		FadeOut->Start();
	}
	auto AssetMng = CurrentWorld->GetWorldAssetManager().lock();
	
	AssetMng->SoundPlay("Portal");
}

void CPortal::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	auto FadeOut = mFadeOut.lock();
	if (FadeOut->Finish())
	{
		auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock();
		World->Load(mLoadType);
		FadeOut->Destroy();
	}
}

void CPortal::CollisionEnd(CCollider* Dest)
{
	auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
	if(Owner)
		Owner->SetTargetObject(nullptr);
}

CPortal* CPortal::Clone()
{
	return new CPortal(*this);
}