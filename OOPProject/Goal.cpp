#include "stdafx.h"
#include "Goal.h"


void Goal::Init()
{
	mpTexture = ResourceManager::Call().GetTexture(L"Goal");
	SetTag("Goal");
	RECT r = { 0,0,32,32 };
	SetSize(r);
}

void Goal::Update(float dt)
{
}

Goal::Goal()
{
}

Goal::Goal(SpriteBatch * pBatch)
{
	mpBatch = pBatch;
}

Goal::Goal(SpriteBatch * pBatch, ID3D11ShaderResourceView * pTexture)
{
	mpBatch = pBatch;
	mpTexture = pTexture;
}


Goal::~Goal()
{
}
