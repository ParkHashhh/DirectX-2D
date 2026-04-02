#include "DamageUI.h"
#include "UI/NumberWidget.h"
#include "UI/Image.h"
#include "World/WorldUIManager.h"
#include "Component/MeshComponent.h"
#include "World/World.h"

CDamageUI::CDamageUI()
{
}

CDamageUI::~CDamageUI()
{
	if (mObject)
	{
		mObject->Destroy();
	}
}


void CDamageUI::SetNumber(int Number)
{
	auto NumberWidget = mNumberWidget.lock();
	auto Mesh = mMeshComponent.lock();
	if (Number <= 0)
	{

		mMissImage->SetEnable(true);
		NumberWidget->SetEnable(false);
		Mesh->SetEnable(false);
	}
	else
	{
		mMissImage->SetEnable(false);
		NumberWidget->SetEnable(true);
		NumberWidget->SetNumber(Number);
	}
}
void CDamageUI::SetPos(FVector3 Pos)
{
	auto	Number = mNumberWidget.lock();
	if (Number)
		Number->SetPos(Pos.x ,Pos.y);
	if (mMissImage)
		mMissImage->SetPos(Pos.x, Pos.y);
}

void CDamageUI::SetEnable(bool Enable)
{
	auto	Number = mNumberWidget.lock();
	if (Number)
		Number->SetEnable(Enable);
}
void CDamageUI::SetHitPos(float X, float Y)
{
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldPos(X, Y);
	}
}

bool CDamageUI::Init()
{
	CWidgetContainer::Init();

	mNumberWidget = CreateWidget<CNumberWidget>("Damage" + std::to_string(rand()));
	auto	Number = mNumberWidget.lock();

	Number->SetSize(24.f,24.f);
	Number->SetNumberSize(48.f, 48.f);
	Number->SetTexture("DamageTexture", TEXT("Damage/Damage.png"));
	
	auto World = mWorld.lock();

	mObject = World->CreateGameObject<CGameObject>("Effect" + std::to_string(rand())).lock();
	mMeshComponent = mObject->CreateComponent<CMeshComponent>("DamageMesh" + std::to_string(rand()));
	auto AssetMng = mWorld.lock()->GetWorldAssetManager().lock();

	mMissImage = CreateWidget<CImage>("MissImage" + std::to_string(rand())).lock();
	mMissImage->SetSize(85, 40);
	mMissImage->AddPos(FVector2(-45, 0));
	mMissImage->SetTexture("MissImage", TEXT("Damage/Miss.png"));

	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetRelativeScale(20.f, 20.f);
		Mesh->SetRelativePos(-40, 80, 0);
		auto Tex = AssetMng->FindTexture("Hit");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, FVector4(1, 1, 1, 1));
	}


	return true;
}


void CDamageUI::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
	mLifeTime += DeltaTime;
	auto	Number = mNumberWidget.lock();
	auto	Mesh = mMeshComponent.lock();
	

	if (Number)
	{
		Number->AddPos(FVector2(0, -150 * DeltaTime));
	}
	if (Mesh)
	{
		Mesh->AddWorldPos(0, 150 * DeltaTime);
	}
	if (mMissImage)
	{
		mMissImage->AddPos(FVector2(0, -150 * DeltaTime));
	}
	if (mLifeTime >= mMaxLifeTime)
	{

		if (mObject)
		{
			mObject->Destroy();
			mObject = nullptr;
		}
		if (Number)
		{
			Number->Destroy();
		}
	}

}

void CDamageUI::Render()
{
	CWidgetContainer::Render();
}

