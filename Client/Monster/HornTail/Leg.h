#pragma once

#include "HornTailPart.h"


class CLeg :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CLeg();
	CLeg(const CLeg& ref);
	CLeg(CLeg&& ref) noexcept;
public:
	virtual ~CLeg();

protected:
	virtual CLeg* Clone();

private:
	std::weak_ptr<class CColliderBox2D>	mAttackBody;

private:
	void SkillStart();
	void SkillEnd();

	void EnableBody();
	void DisableBody();

	void StartDie();
	void EndDie();
public:

	virtual void Damage(float Damage);
	void ChangeAttack();
	virtual bool Init();
	virtual void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	virtual void OnHitAttack(const FVector3& HitPoint, class CCollider* Dest);
};