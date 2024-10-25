#include "stdafx.h"
#include "Ball.h"


void Ball::Init()
{
	mpTexture = ResourceManager::Call().GetTexture(L"Ball");
	SetTag("Ball");
	RECT r = { 0,0,32,32 };
	SetSize(r);
}

void Ball::Update(float dt)
{
}


Ball::Ball()
{
}

Ball::Ball(SpriteBatch * pBatch)
{
	mpBatch = pBatch;
}

Ball::Ball(SpriteBatch * pBatch, ID3D11ShaderResourceView * pTexture)
{
	mpBatch = pBatch;
	mpTexture = pTexture;
}


Ball::~Ball()
{
}
