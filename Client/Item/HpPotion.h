#pragma once
#include "Item.h"

class CHpPotion :
    public CItem
{
	friend class CWorld;
	friend class CObject;
protected:
	CHpPotion();
	CHpPotion(const CHpPotion& ref);
	CHpPotion(CHpPotion&& ref)	noexcept;
public:
	virtual ~CHpPotion();

	virtual bool Init();
	virtual void GetItem(std::weak_ptr<CPlayer> Player);
	virtual void Update(float DeltaTime);


};

