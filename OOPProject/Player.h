#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
public:
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw();
public:
	Player();
	Player(SpriteBatch* pBatch);
	Player(SpriteBatch* pBatch, ID3D11ShaderResourceView* pTexture);
	virtual ~Player();
};

