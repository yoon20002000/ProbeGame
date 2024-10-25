#include "stdafx.h"
#include "Wall.h"


void Wall::Init()
{
	mpTexture = ResourceManager::Call().GetTexture(L"Wall");
	SetTag("Wall");
	RECT r = { 0,0,32,32 };
	SetSize(r);
}

void Wall::Update(float dt)
{
}

Wall::Wall()
{
}

Wall::Wall(SpriteBatch * pBatch)
{
	mpBatch = pBatch;
}

Wall::Wall(SpriteBatch * pBatch, ID3D11ShaderResourceView * pTexture)
{
	mpBatch = pBatch;
	mpTexture = pTexture;
}


Wall::~Wall()
{
}
