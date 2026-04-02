#pragma once

#include "HornTailPart.h"

class CLeftHead :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CLeftHead();
	CLeftHead(const CLeftHead& ref);
	CLeftHead(CLeftHead&& ref) noexcept;
public:
	virtual ~CLeftHead();

private:
	std::vector <std::weak_ptr<CCollider>> mGravityList;
	std::vector<float> XPosList;

protected:
	virtual CLeftHead* Clone();
private:

	void SkillStart();
	void SkillEnd();
	void SetXPosList();
	void GetGravityList();
	void ShuffleXPosList();
	void StartDie();
	void EndDie();


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void OnHit(const FVector3& HitPoint, class CCollider* Dest);
	virtual void Damage(float Damage);
	void ChangeAttack();
};