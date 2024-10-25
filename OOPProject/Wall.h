#pragma once
#include "GameObject.h"
class Wall :
	public GameObject
{
public:
	virtual void Init();
	virtual void Update(float dt);
public:
	Wall();
	Wall(SpriteBatch* pBatch);
	Wall(SpriteBatch* pBatch, ID3D11ShaderResourceView* pTexture);
	virtual ~Wall();
};

