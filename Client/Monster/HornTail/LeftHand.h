#pragma once

#include "HornTailPart.h"


class CLeftHand :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CLeftHand();
	CLeftHand(const CLeftHand& ref);
	CLeftHand(CLeftHand&& ref) noexcept;
public:
	virtual ~CLeftHand();

protected:
	virtual CLeftHand* Clone();
private:
	void SkillStart();
	void SkillEnd();

	void StartDie();
	void EndDie();

public:
	virtual void Damage(float Damage);
	void ChangeAttack();
	virtual bool Init();
	virtual void OnHit(const FVector3& HitPoint, class CCollider* Dest);
};