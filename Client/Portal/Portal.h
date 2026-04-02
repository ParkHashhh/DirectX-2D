#pragma once
#include "Object/GameObject.h"
#include "../ClientInfo.h"

class CPortal :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;
protected:
	CPortal();
	CPortal(const CPortal& ref);
	CPortal(CPortal&& ref)	noexcept;
public:
	virtual ~CPortal();

private:
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CFadeOutWidget>	mFadeOut;

	EWorldType	mLoadType = EWorldType::Start;


private:
	void CollisionBegin(const FVector3& HitPoint,
		class CCollider* Dest);
	void CollisionEnd(class CCollider* Dest);

protected:
	virtual CPortal* Clone();


public:
	void SetLoadType(EWorldType Type)
	{
		mLoadType = Type;
	}
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Interact();

};

