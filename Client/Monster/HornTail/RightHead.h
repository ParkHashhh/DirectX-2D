#pragma once

#include "HornTailPart.h"


class CRightHead :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CRightHead();
	CRightHead(const CRightHead& ref);
	CRightHead(CRightHead&& ref) noexcept;
public:
	virtual ~CRightHead();

private:
	int mMonsterCount = 3;

protected:
	virtual CRightHead* Clone();
private:
	void SkillStart();
	void SkillEnd();

	void StartDie();
	void EndDie();
public:

	void ChangeAttack();
	virtual void Damage(float Damage);

	virtual bool Init();
	virtual void OnHit(const FVector3& HitPoint, class CCollider* Dest);
};