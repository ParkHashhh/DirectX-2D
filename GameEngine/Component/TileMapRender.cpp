#include "TileMapRender.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Asset/Texture/Texture.h"
#include "../World/World.h"
#include "../World/WorldAssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Shader/Shader.h"
#include "../Asset/Shader/ShaderManager.h"
#include "../Asset/Shader/CBufferTransform.h"
#include "TileMapComponent.h"
#include "../World/WorldManager.h"
#include "../World/CameraManager.h"

CTileMapRender::CTileMapRender()
{
	SetClassType<CTileMapRender>();
	mRenderType = EComponentRender::Render;
}

CTileMapRender::CTileMapRender(const CTileMapRender& ref)	:
	CSceneComponent(ref)
{
	mRenderType = EComponentRender::Render;
}

CTileMapRender::CTileMapRender(CTileMapRender&& ref) noexcept :
	CSceneComponent(ref)
{
	mRenderType = EComponentRender::Render;
}

CTileMapRender::~CTileMapRender()
{
}

void CTileMapRender::SetTileMapComponent(
	const std::weak_ptr<class CTileMapComponent>& TileMap)
{
	mTileMap = TileMap;

	auto	TileMapCom = mTileMap.lock();

	if (TileMapCom)
	{
		SetWorldScale(TileMapCom->GetMapSize());
	}
}

void CTileMapRender::EnableAlphaBlend()
{
	mAlphaBlend = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

void CTileMapRender::EnableTileAlphaBlend()
{
	mTileAlphaBlend = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type,
	const std::weak_ptr<class CTexture>& Texture)
{
	mTexture[Type] = Texture;

	if (Type == ETileTextureType::Tile)
	{
		auto	Texture = mTexture[Type].lock();

		auto	TileMap = mTileMap.lock();

		if (TileMap)
		{
			TileMap->SetTileTextureSize((float)Texture->GetTexture()->Width, (float)Texture->GetTexture()->Height);
		}
	}

	return true;
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type,
	const std::string& Name)
{
	auto	World = mWorld.lock();

	if (World)
	{
		auto	AssetMgr = World->GetWorldAssetManager().lock();

		mTexture[Type] = AssetMgr->FindTexture(Name);
	}

	else
	{
		auto	TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

		mTexture[Type] = TexMgr->FindTexture("Texture_" + Name);
	}

	if (Type == ETileTextureType::Tile)
	{
		auto	Texture = mTexture[Type].lock();

		auto	TileMap = mTileMap.lock();

		if (TileMap)
		{
			TileMap->SetTileTextureSize((float)Texture->GetTexture()->Width, (float)Texture->GetTexture()->Height);
		}
	}

	return true;
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type,
	const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	auto	World = mWorld.lock();

	if (World)
	{
		auto	AssetMgr = World->GetWorldAssetManager().lock();

		if (!AssetMgr->LoadTexture(Name, FileName, PathName))
			return false;

		mTexture[Type] = AssetMgr->FindTexture(Name);
	}

	else
	{
		auto	TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

		if (!TexMgr->LoadTexture("Texture_" + Name, FileName, PathName))
			return false;

		mTexture[Type] = TexMgr->FindTexture("Texture_" + Name);
	}

	if (Type == ETileTextureType::Tile)
	{
		auto	Texture = mTexture[Type].lock();

		auto	TileMap = mTileMap.lock();

		if (TileMap)
		{
			TileMap->SetTileTextureSize((float)Texture->GetTexture()->Width, (float)Texture->GetTexture()->Height);
		}
	}

	return true;
}

void CTileMapRender::SetBackMesh(
	const std::weak_ptr<class CMesh>& Mesh)
{
	mBackMesh = Mesh;
}

void CTileMapRender::SetBackMesh(const std::string& Name)
{
	auto	World = mWorld.lock();

	if (World)
	{
		auto	AssetMgr = World->GetWorldAssetManager().lock();

		mBackMesh = AssetMgr->FindMesh(Name);
	}

	else
	{
		auto	MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();

		mBackMesh = MeshMgr->FindMesh("Mesh_" + Name);
	}
}

void CTileMapRender::SetBackShader(
	const std::weak_ptr<class CShader>& Shader)
{
	mBackShader = Shader;
}

void CTileMapRender::SetBackShader(const std::string& Name)
{
	auto	ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

	mBackShader = ShaderMgr->FindShader(Name);
}

void CTileMapRender::SetTileFrameSize(float x, float y)
{
	auto	TileMap = mTileMap.lock();

	if (TileMap)
	{
	}
}

void CTileMapRender::AddTileFrame(const FVector2& Start,
	const FVector2& End)
{
	FTileFrame	Frame;
	Frame.Start = Start;
	Frame.End = End;

	mTileFrame.push_back(Frame);

	auto	TileMap = mTileMap.lock();

	if (TileMap)
	{
		TileMap->AddTileFrame(Start, End);
	}
}

void CTileMapRender::AddTileFrame(float StartX, float StartY,
	float EndX, float EndY)
{
	FTileFrame	Frame;
	Frame.Start = FVector2(StartX, StartY);
	Frame.End = FVector2(EndX, EndY);

	mTileFrame.push_back(Frame);

	auto	TileMap = mTileMap.lock();

	if (TileMap)
	{
		TileMap->AddTileFrame(Frame.Start, Frame.End);
	}
}

bool CTileMapRender::Init()
{
	SetRenderLayer("Map");

	CSceneComponent::Init();

	if(mTileMap.expired())
	{
		auto	Owner = mOwner.lock();

		mTileMap = Owner->FindComponent<CTileMapComponent>();
	}

	auto	TileMap = mTileMap.lock();

	if (TileMap)
	{
		SetWorldScale(TileMap->GetMapSize());
	}

	mTransformCBuffer.reset(new CCBufferTransform);

	mTransformCBuffer->Init();

	SetBackShader("DefaultTex");
	SetBackMesh("RectTex");

	return true;
}

void CTileMapRender::Render()
{
	CSceneComponent::Render();

	if (!mTexture[ETileTextureType::Back].expired())
	{
		FMatrix	ScaleMat, TranslateMat, WorldMat;

		ScaleMat.Scaling(mWorldScale);
		TranslateMat.Translation(mWorldPos);

		WorldMat = ScaleMat * TranslateMat;

		mTransformCBuffer->SetWorldMatrix(WorldMat);

		auto	World = mWorld.lock();

		if (!World)
		{
			World = CWorldManager::GetInst()->GetWorld().lock();
		}

		auto	CameraMgr = World->GetCameraManager().lock();

		mTransformCBuffer->SetViewMatrix(CameraMgr->GetViewMatrix());
		mTransformCBuffer->SetProjMatrix(CameraMgr->GetProjMatrix());

		std::shared_ptr<CShader>    Shader = mBackShader.lock();
		std::shared_ptr<CMesh>    Mesh = mBackMesh.lock();

		FVector3	PivotSize = mPivot * Mesh->GetMeshSize();

		mTransformCBuffer->SetPivotSize(PivotSize);

		mTransformCBuffer->UpdateBuffer();

		auto	Texture = mTexture[ETileTextureType::Back].lock();

		Texture->SetShader(0, EShaderBufferType::Pixel, 0);

		auto	State = mAlphaBlend.lock();

		if (State)
			State->SetState();

		Shader->SetShader();

		Mesh->Render();

		if (State)
			State->ResetState();
	}

	// 타일 출력
	if (!mTileMap.expired())
	{
		auto	TileMap = mTileMap.lock();

		if (!mTexture[ETileTextureType::Tile].expired())
		{
			auto	Texture = mTexture[ETileTextureType::Tile].lock();

			Texture->SetShader(1, EShaderBufferType::Pixel,
				0);

			auto	State = mTileAlphaBlend.lock();

			if (State)
				State->SetState();

			TileMap->RenderTile();

			if (State)
				State->ResetState();
		}

		TileMap->RenderTileOutLine();
	}
}

CTileMapRender* CTileMapRender::Clone()	const
{
	return new CTileMapRender(*this);
}
