#include "ThreadQueue.h"

CThreadQueue::CThreadQueue()
{
	InitializeCriticalSection(&mCrt);
}

CThreadQueue::~CThreadQueue()
{
	DeleteCriticalSection(&mCrt);
}

void CThreadQueue::push(int Header, int Size, unsigned char* Data)
{
	if (full())
		return;

	CSync	sync(&mCrt);

	mPush = (mPush + 1) % 200;

	mData[mPush].Header = Header;
	mData[mPush].Size = Size;
	memcpy(mData[mPush].Data, Data, Size);

	++mSize;
}

void CThreadQueue::pop(int& Header, int& Size, unsigned char* Data)
{
	if (empty())
		return;

	CSync	sync(&mCrt);

	mPop = (mPop + 1) % 200;

	Header = mData[mPop].Header;
	Size = mData[mPop].Size;
	memcpy(Data, mData[mPop].Data, Size);

	--mSize;
}

int CThreadQueue::size()
{
	CSync	sync(&mCrt);

	return mSize;
}

bool CThreadQueue::full()
{
	CSync	sync(&mCrt);

	return mSize == 200;
}

bool CThreadQueue::empty()
{
	CSync	sync(&mCrt);

	return mSize == 0;
}
