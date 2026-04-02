#pragma once
#include "Monster.h"

class CHornTail :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CHornTail();
	CHornTail(const CHornTail& ref);
	CHornTail(CHornTail&& ref)	noexcept;
public:
	virtual ~CHornTail();


private:
	std::shared_ptr<class CLeftHead> mLeftHead;
	std::shared_ptr<class CRightHead> mRightHead;
	std::shared_ptr<class CMiddleHead> mMiddleHead;
	std::shared_ptr<class CLeftHand> mLeftHand;
	std::shared_ptr<class CRightHand> mRightHand;
	std::shared_ptr<class CTail> mTail;
	std::shared_ptr<class CLeg> mLeg;

protected:
	virtual CHornTail* Clone();

public:
	virtual bool Init();
};

