#pragma once

#include "HornTailPart.h"


class CWing :
	public CHornTailPart
{
	friend class CWorld;
	friend class CObject;
protected:
	CWing();
	CWing(const CWing& ref);
	CWing(CWing&& ref) noexcept;
public:
	virtual ~CWing();

protected:
	virtual CWing* Clone();


private:

	void SkillStart();
	void SkillEnd();
	void StartDie();
	void EndDie();

public:
	virtual void ChangeAttack();
	void ChangeDie();
	virtual bool Init();
};