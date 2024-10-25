#include "stdafx.h"
#include "Player.h"


void Player::Init()
{
	// 택스쳐 불러오는데 문제 생김 map에 저장이상은 없어보이는데 뭐가 문제인지 파악 필요
	mpTexture = ResourceManager::Call().GetTexture(L"Player");
	SetTag("Player");
	RECT r = { 0,0,32,48 };
	SetSize(r);
}

void Player::Update(float dt)
{
	
	// keyboard 인풋 넣을것
}

void Player::Draw()
{
	mPos.y -= 16;
	GameObject::Draw();
}

Player::Player()
{

}

Player::Player(SpriteBatch * pBatch)
{
	
	mpBatch = pBatch;
}

Player::Player(SpriteBatch * pBatch, ID3D11ShaderResourceView * pTexture)
{
	mpBatch = pBatch;
	mpTexture = pTexture;
}


Player::~Player()
{
}
