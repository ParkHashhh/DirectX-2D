#pragma once

#include "Object/GameObject.h"

class CArmSkill :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CArmSkill();
	CArmSkill(const CArmSkill& ref);
	CArmSkill(CArmSkill&& ref)	noexcept;

public:
	virtual ~CArmSkill();

private:

	std::weak_ptr<class CZakumArm> mOwner;

	std::weak_ptr<class CMeshComponent>	mMeshComponent;
	std::weak_ptr<class CAnimation2DComponent>	mAnimation2DComponent;
	std::weak_ptr<class CColliderBox2D>	mBody;

	bool mSymmetry = false;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetSymmetry(bool Symetry);
	void SetOwner(std::weak_ptr<class CZakumArm> Owner)
	{
		mOwner = Owner;
	}

protected:	
	virtual CArmSkill* Clone();
	void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	void Pause();
	void EnableBox();
	void DisableBox();
private:
	void FinishAnimation();
};

