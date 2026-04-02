#pragma once

#include "../EngineInfo.h"

enum class ETileType
{
	Normal,
	UnableToMove
};

class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	CTile(const CTile& ref);
	CTile(CTile&& ref)	noexcept;

public:
	~CTile();

private:
	ETileType	mType = ETileType::Normal;
	FVector2	mPos;
	FVector2	mSize;
	FVector2	mCenter;
	int			mTextureFrame = -1;
	FVector2	mFrameStart;
	FVector2	mFrameEnd;
	bool		mRender = false;

public:
	const FVector2& GetFrameStart()	const
	{
		return mFrameStart;
	}

	const FVector2& GetFrameEnd()	const
	{
		return mFrameEnd;
	}

	bool GetRender()	const
	{
		return mRender;
	}

	ETileType GetType()	const
	{
		return mType;
	}

	int GetTextureFrame()	const
	{
		return mTextureFrame;
	}

	const FVector2& GetPos()	const
	{
		return mPos;
	}

	const FVector2& GetSize()	const
	{
		return mSize;
	}

	const FVector2& GetCenter()	const
	{
		return mCenter;
	}

public:
	void SetTileType(ETileType Type)
	{
		mType = Type;
	}

	void SetTextureFrame(int Frame)
	{
		mTextureFrame = Frame;
	}

	void SetPos(const FVector2& Pos)
	{
		mPos = Pos;
	}

	void SetPos(float x, float y)
	{
		mPos.x = x;
		mPos.y = y;
	}

	void SetSize(const FVector2& Size)
	{
		mSize = Size;
	}

	void SetSize(float x, float y)
	{
		mSize.x = x;
		mSize.y = y;
	}

	void SetCenter(const FVector2& Center)
	{
		mCenter = Center;
	}

	void SetCenter(float x, float y)
	{
		mCenter.x = x;
		mCenter.y = y;
	}

	void SetFrame(const FVector2& Start, const FVector2& End)
	{
		mFrameStart = Start;
		mFrameEnd = End;

		mRender = true;
	}

	void SetFrame(float StartX, float StartY,
		float EndX, float EndY)
	{
		mFrameStart.x = StartX;
		mFrameStart.y = StartY;

		mFrameEnd.x = EndX;
		mFrameEnd.y = EndY;

		mRender = true;
	}

public:
	void Save(FILE* File);
	void Load(FILE* File);
};

