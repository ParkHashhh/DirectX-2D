#pragma once

#include "HornTailPart.h"


class CTail :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CTail();
	CTail(const CTail& ref);
	CTail(CTail&& ref) noexcept;
public:
	virtual ~CTail();


protected:
	virtual CTail* Clone();
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