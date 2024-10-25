#include "stdafx.h"
#include "Player.h"


void Player::Init()
{
	// �ý��� �ҷ����µ� ���� ���� map�� �����̻��� ����̴µ� ���� �������� �ľ� �ʿ�
	mpTexture = ResourceManager::Call().GetTexture(L"Player");
	SetTag("Player");
	RECT r = { 0,0,32,48 };
	SetSize(r);
}

void Player::Update(float dt)
{
	
	// keyboard ��ǲ ������
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
