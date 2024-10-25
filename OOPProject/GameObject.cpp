#include "stdafx.h"
#include "GameObject.h"


void GameObject::Init()
{
	// 현재는 애니메이션이 없어서 굳이 이닛 필요없음
}

void GameObject::Update(float dt)
{
}

void GameObject::Draw()
{
	mpBatch->Draw(mpTexture, mPos, &mSize);
}

GameObject::GameObject() : mPos(0,0)
{
	// texture 기본 size
	mSize = { 0,0,32,32 };
}

GameObject::GameObject(SpriteBatch * pBatch)
{
	GameObject();
	mpBatch = pBatch;
}

GameObject::GameObject(SpriteBatch* pBatch,ID3D11ShaderResourceView * pTexture)
{
	GameObject();
	mpBatch = pBatch;
	mpTexture = pTexture;
}


GameObject::~GameObject()
{
}
