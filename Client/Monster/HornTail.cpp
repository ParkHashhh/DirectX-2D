#include "HornTail.h"

CHornTail::CHornTail()
{
	SetClassType<CHornTail>();
}

CHornTail::CHornTail(const CHornTail& ref)
	: CMonster(ref)
{
}


CHornTail::CHornTail(CHornTail&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CHornTail::~CHornTail()
{

}

bool CHornTail::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

CHornTail* CHornTail::Clone()
{
	return new CHornTail(*this);
}