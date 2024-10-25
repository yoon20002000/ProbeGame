#pragma once
#include "GameObject.h"
class Goal :
	public GameObject
{
public:
	virtual void Init();
	virtual void Update(float dt);
public:
	Goal();
	Goal(SpriteBatch* pBatch);
	Goal(SpriteBatch* pBatch, ID3D11ShaderResourceView* pTexture);
	virtual~Goal();
};

