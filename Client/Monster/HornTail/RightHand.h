#pragma once

#include "HornTailPart.h"

class CRightHand :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CRightHand();
	CRightHand(const CRightHand& ref);
	CRightHand(CRightHand&& ref) noexcept;
public:
	virtual ~CRightHand();

protected:
	virtual CRightHand* Clone();
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