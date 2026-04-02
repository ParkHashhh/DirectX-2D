#include "TileMapComponent.h"
#include "../World/World.h"
#include "../World/WorldAssetManager.h"
#include "../World/CameraManager.h"
#include "../World/WorldManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Shader/Shader.h"
#include "../Asset/Shader/ShaderManager.h"
#include "../Asset/Shader/CBufferTransform.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Object/GameObject.h"
#include "TileMapRender.h"
#include "../Device.h"
#include "../Asset/Shader/CBufferTileMap.h"

CTileMapComponent::CTileMapComponent()
{
	SetClassType<CTileMapComponent>();
}

CTileMapComponent::CTileMapComponent(
	const CTileMapComponent& ref) :
	CObjectComponent(ref)
{
}

CTileMapComponent::CTileMapComponent(
	CTileMapComponent&& ref) noexcept :
	CObjectComponent(std::move(ref))
{
}

CTileMapComponent::~CTileMapComponent()
{
}

void CTileMapComponent::SetTileOutLineRender(bool Render)
{
	mTileOutLineRender = Render;

	if (mTileOutLineRender)
	{
		// OutLine 출력용 Shader와 Mesh를 얻어온다.
	}

	else
	{
		mOutLineMesh.reset();
		mOutLineShader.reset();
	}
}

bool CTileMapComponent::SetTileTexture(
	ETileTextureType::Type Type,
	const std::weak_ptr<CTexture>& Texture)
{
	auto	Owner = mOwner.lock();

	std::weak_ptr<CTileMapRender>	Renderer =
		Owner->FindComponent<CTileMapRender>();

	if (Renderer.expired())
		return false;

	return Renderer.lock()->SetTexture(Type, Texture);
}

bool CTileMapComponent::SetTileTexture(
	ETileTextureType::Type Type,
	const std::string& Name)
{
	auto	Owner = mOwner.lock();

	std::weak_ptr<CTileMapRender>	Renderer =
		Owner->FindComponent<CTileMapRender>();

	if (Renderer.expired())
		return false;

	return Renderer.lock()->SetTexture(Type, Name);
}

bool CTileMapComponent::SetTileTexture(
	ETileTextureType::Type Type,
	const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	auto	Owner = mOwner.lock();

	std::weak_ptr<CTileMapRender>	Renderer =
		Owner->FindComponent<CTileMapRender>();

	if (Renderer.expired())
		return false;

	return Renderer.lock()->SetTexture(Type, Name, FileName,
		PathName);
}

void CTileMapComponent::AddTileFrame(const FVector2& Start,
	const FVector2& End)
{
	FTileFrame	Frame;
	Frame.Start = Start;
	Frame.End = End;

	mTileFrame.push_back(Frame);
}

void CTileMapComponent::AddTileFrame(float StartX, float StartY,
	float EndX, float EndY)
{
	FTileFrame	Frame;
	Frame.Start = FVector2(StartX, StartY);
	Frame.End = FVector2(EndX, EndY);

	mTileFrame.push_back(Frame);
}

void CTileMapComponent::SetTileFrameAll(int FrameIndex)
{
	auto	iter = mTileList.begin();
	auto	iterEnd = mTileList.end();

	FTileFrame	Frame = mTileFrame[FrameIndex];

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetFrame(Frame.Start, Frame.End);
	}
}

bool CTileMapComponent::Init()
{
	CObjectComponent::Init();

	auto	Owner = mOwner.lock();

	std::shared_ptr<CTileMapRender>	Renderer =
		Owner->FindComponent<CTileMapRender>().lock();

	if (Renderer)
	{
		Renderer->SetTileMapComponent(std::dynamic_pointer_cast<CTileMapComponent>(mSelf.lock()));
	}


	mTransform.reset(new CCBufferTransform);

	mTransform->Init();

	mCBufferTileMap.reset(new CCBufferTileMap);

	mCBufferTileMap->Init();

	auto	World = mWorld.lock();

	if (World)
	{
		auto	AssetMgr = World->GetWorldAssetManager().lock();

		mTileMesh = AssetMgr->FindMesh("RectTex");
	}

	else
	{
		auto	MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();

		mTileMesh = MeshMgr->FindMesh("Mesh_RectTex");
	}

	auto	ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

	//mTileShader = ShaderMgr->FindShader("TileMap");
	mTileShader = ShaderMgr->FindShader("TileMapInstancing");

	return true;
}

bool CTileMapComponent::Init(const char* FileName)
{
	return true;
}

void CTileMapComponent::Update(float DeltaTime)
{
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	FVector3	Center;

	auto	World = mWorld.lock();

	if (!World)
	{
		World = CWorldManager::GetInst()->GetWorld().lock();
	}

	Center = World->GetCameraManager().lock()->GetMainCameraWorldPos();

	// 타일맵 시작점으로부터 상대적인 위치를 구한다.
	Center -= mOwner.lock()->GetWorldPos();

	FResolution	RS = CDevice::GetInst()->GetResolution();

	switch (mShape)
	{
	case Rect:
		mViewStartX = (int)((Center.x - RS.Width * 0.5f) /
			mTileSize.x);
		mViewStartY = (int)((Center.y - RS.Height * 0.5f) /
			mTileSize.y);

		mViewEndX = (int)((Center.x + RS.Width * 0.5f) /
			mTileSize.x);
		mViewEndY = (int)((Center.y + RS.Height * 0.5f) /
			mTileSize.y);
		break;
	case Isometric:
		break;
	}

	mViewStartX = Clamp<int>(mViewStartX, 0, mCountX - 1);
	mViewEndX = Clamp<int>(mViewEndX, 0, mCountX - 1);

	mViewStartY = Clamp<int>(mViewStartY, 0, mCountY - 1);
	mViewEndY = Clamp<int>(mViewEndY, 0, mCountY - 1);

	int	DataCountX = mViewEndX - mViewStartX + 1;
	int	DataCountY = mViewEndY - mViewStartY + 1;

	mInstancingCount = DataCountX * DataCountY;

	if (mTileIstData.size() < mInstancingCount)
	{
		mTileIstData.clear();
		mTileIstData.resize(mInstancingCount);
	}

	auto	Owner = mOwner.lock();
	auto	CameraMgr = World->GetCameraManager().lock();

	mInstancingCount = 0;

	// 인스턴싱 정보를 만든다.
	for (int i = mViewStartY; i <= mViewEndY; ++i)
	{
		for (int j = mViewStartX; j <= mViewEndX; ++j)
		{
			int	Index = i * mCountX + j;

			if (!mTileList[Index]->GetRender())
				continue;

			FMatrix	ScaleMat, TranslateMat, WorldMat;

			ScaleMat.Scaling(mTileSize);

			FVector2	Pos = mTileList[Index]->GetPos();

			Pos.x += Owner->GetWorldPos().x;
			Pos.y += Owner->GetWorldPos().y;

			TranslateMat.Translation(Pos);

			//FMatrix	RotMat;

			//RotMat.RotationZ(230.f);
			//RotMat.Rotation(FVector3(0.f, 0.f, 238.f));

			//WorldMat = ScaleMat * RotMat * TranslateMat;
			WorldMat = ScaleMat * TranslateMat;

			FMatrix	ViewMat = CameraMgr->GetViewMatrix();
			FMatrix	ProjMat = CameraMgr->GetProjMatrix();

			FMatrix	WVPMat = WorldMat * ViewMat * ProjMat;

			mTileIstData[mInstancingCount].WVP0 = WVPMat[0];
			mTileIstData[mInstancingCount].WVP1 = WVPMat[1];
			mTileIstData[mInstancingCount].WVP2 = WVPMat[2];
			mTileIstData[mInstancingCount].WVP3 = WVPMat[3];

			mTileIstData[mInstancingCount].LTUV = 
				mTileList[Index]->GetFrameStart() / mTileTextureSize;
			mTileIstData[mInstancingCount].RBUV = 
				mTileList[Index]->GetFrameEnd() / mTileTextureSize;

			++mInstancingCount;
		}
	}

	SetInstancingData(&mTileIstData[0],
		mInstancingCount);
}

void CTileMapComponent::PostRender()
{
	CObjectComponent::PostRender();
}

void CTileMapComponent::Destroy()
{
	CObjectComponent::Destroy();
}

CTileMapComponent* CTileMapComponent::Clone()	const
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::RenderTile()
{
	auto	Owner = mOwner.lock();
	auto	World = mWorld.lock();

	if (!World)
		World = CWorldManager::GetInst()->GetWorld().lock();

	auto	CameraMgr = World->GetCameraManager().lock();

	auto	Shader = mTileShader.lock();
	auto	Mesh = mTileMesh.lock();

	Shader->SetShader();

	Mesh->RenderInstancing(mInstancingBuffer,
		mInstancingCount);

	/*for (int i = mViewStartY; i <= mViewEndY; ++i)
	{
		for (int j = mViewStartX; j <= mViewEndX; ++j)
		{
			int	Index = i * mCountX + j;

			if (!mTileList[Index]->GetRender())
				continue;

			FMatrix	ScaleMat, TranslateMat, WorldMat;

			ScaleMat.Scaling(mTileSize);

			FVector2	Pos = mTileList[Index]->GetPos();

			Pos.x += Owner->GetWorldPos().x;
			Pos.y += Owner->GetWorldPos().y;

			TranslateMat.Translation(Pos);

			WorldMat = ScaleMat * TranslateMat;

			mTransform->SetWorldMatrix(WorldMat);

			mTransform->SetViewMatrix(CameraMgr->GetViewMatrix());
			mTransform->SetProjMatrix(CameraMgr->GetProjMatrix());

			mTransform->UpdateBuffer();

			FVector2	LTUV = mTileList[Index]->GetFrameStart() / mTileTextureSize;
			FVector2	RBUV = mTileList[Index]->GetFrameEnd() / mTileTextureSize;

			mCBufferTileMap->SetUV(LTUV, RBUV);

			mCBufferTileMap->UpdateBuffer();

			Shader->SetShader();

			Mesh->Render();
		}
	}*/
}

void CTileMapComponent::RenderTileOutLine()
{
}

void CTileMapComponent::CreateTile(ETileShape Shape,
	int CountX, int CountY, const FVector2& TileSize,
	int TileTextureFrame)
{
	mShape = Shape;
	mCountX = CountX;
	mCountY = CountY;
	mTileSize = TileSize;

	switch (mShape)
	{
	case Rect:
		mMapSize = mTileSize *
			FVector2((float)mCountX, (float)mCountY);
		break;
	case Isometric:
		break;
	}

	auto	Owner = mOwner.lock();

	std::shared_ptr<CTileMapRender>	Renderer =
		Owner->FindComponent<CTileMapRender>().lock();

	if (Renderer)
	{
		Renderer->SetWorldScale(mMapSize);
	}

	mTileList.clear();

	mTileList.resize(mCountX * mCountY);

	for (int i = 0; i < mCountY; ++i)
	{
		for (int j = 0; j < mCountX; ++j)
		{
			int	Index = i * mCountX + j;

			mTileList[Index].reset(new CTile);

			switch (mShape)
			{
			case Rect:
				mTileList[Index]->SetPos(j * mTileSize.x,
					i * mTileSize.y);
				break;
			case Isometric:
				break;
			}

			mTileList[Index]->SetSize(mTileSize);
			mTileList[Index]->SetCenter(mTileList[Index]->GetPos() + mTileSize * 0.5f);
			mTileList[Index]->SetTextureFrame(TileTextureFrame);
		}
	}

	// 화면에 보일 수 있는 최대 타일 개수를 정한다.
	FResolution	RS = CDevice::GetInst()->GetResolution();

	int	ViewCountX = RS.Width / mTileSize.x + 3;
	int	ViewCountY = RS.Height / mTileSize.y + 3;

	CreateInstancingBuffer(
		sizeof(FTileMapInstancingBuffer),
		ViewCountX * ViewCountY);

	mTileIstData.resize(ViewCountX * ViewCountY);

	/*auto Mesh = mTileMesh.lock();

	if (Mesh)
	{
		Mesh->CreateInstancingBuffer(
			sizeof(FTileMapInstancingBuffer),
			ViewCountX * ViewCountY);

		mTileIstData.resize(ViewCountX * ViewCountY);
	}*/
}

bool CTileMapComponent::CreateInstancingBuffer(int Size,
	int Count)
{
	SAFE_RELEASE(mInstancingBuffer.Buffer);

	mInstancingBuffer.Size = Size;
	mInstancingBuffer.Count = Count;

	// 버퍼를 생성하기 위한 구조체
	D3D11_BUFFER_DESC	BufferDesc = {};

	// 버퍼의 전체 메모리 크기
	BufferDesc.ByteWidth = Size * Count;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, nullptr, &mInstancingBuffer.Buffer)))
		return false;

	return true;
}

bool CTileMapComponent::SetInstancingData(void* Data,
	int Count)
{
	if (!mInstancingBuffer.Buffer)
		return false;

	if (mInstancingBuffer.Count < Count)
	{
		if (!CreateInstancingBuffer(mInstancingBuffer.Size,
			Count * 2))
			return false;
	}

	ID3D11DeviceContext* Context =
		CDevice::GetInst()->GetContext();

	D3D11_MAPPED_SUBRESOURCE	MS = {};

	Context->Map(mInstancingBuffer.Buffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &MS);

	memcpy(MS.pData, Data, mInstancingBuffer.Size * Count);

	Context->Unmap(mInstancingBuffer.Buffer, 0);

	mInstancingCount = Count;

	return true;
}
