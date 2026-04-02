#pragma once

#include "HornTailPart.h"


class CMiddleHead :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CMiddleHead();
	CMiddleHead(const CMiddleHead& ref);
	CMiddleHead(CMiddleHead&& ref) noexcept;
public:
	virtual ~CMiddleHead();

private:
	std::weak_ptr<class CColliderBox2D>	mAttackBody;

protected:
	virtual CMiddleHead* Clone();
private:

	void StartDie();
	void EndDie();
	void SkillStart();
	void SkillEnd();
	void EnableAttackBody();
	void DisableAttackBody();
public:
	void ChangeAttack();
	virtual bool Init();
	virtual void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	virtual void OnHitAttack(const FVector3& HitPoint, class CCollider* Dest);
	virtual void Damage(float Damage);

};