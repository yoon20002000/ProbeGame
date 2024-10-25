#pragma once
#include "GameObject.h"
class Ball :
	public GameObject
{
public :
	virtual void Init();
	virtual void Update(float dt);
public:
	Ball();
	Ball(SpriteBatch* pBatch);
	Ball(SpriteBatch* pBatch, ID3D11ShaderResourceView* pTexture);
	virtual ~Ball();
};

