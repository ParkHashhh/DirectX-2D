#include "RayTracing.h"
#include "Component/ColliderLine2D.h"
#include "Component/ColliderBox2D.h"

CRayTracing::CRayTracing()
{
}

CRayTracing::CRayTracing(const CRayTracing& ref)
{
}

CRayTracing::CRayTracing(CRayTracing&& ref) noexcept
{
}

CRayTracing::~CRayTracing()
{
}


void CRayTracing::SetEnable(bool Enable)
{
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(Enable);
	}
}



bool CRayTracing::Init()
{
	mBody = CreateComponent<CColliderBox2D>("Ray");
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(true);
		Body->SetCollisionProfile("Ray");
		Body->SetDebugDraw(false);
		Body->SetBoxSize(10, 10);

	}

	return true;
}
void CRayTracing::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	
}

void CRayTracing::ClearCollision()
{
	auto Body = mBody.lock();
	if (Body)
	{
		Body->ClearCollisionList();
	}
}

void CRayTracing::SetBoxSize(float X,float Y)
{
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(X, Y);
	}
}
