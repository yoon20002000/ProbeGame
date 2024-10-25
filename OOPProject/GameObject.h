#pragma once
// gameobject가 기본적으로 가지고 있는
class GameObject
{
protected :
	XMFLOAT2 mPos;
	RECT mSize;
	string mTag;
protected :
	// texture 저장할 resource view
	ID3D11ShaderResourceView*	mpTexture;
	// 그리기 위해 필요한 sprite 
	SpriteBatch*				mpBatch;

public :
	// image 위치 값
	void SetPosition(XMFLOAT2 pos) { mPos = pos; }
	XMFLOAT2 GetPosition() { return mPos; }
	float GetPositionX() { return mPos.x; }
	float GetPositionY() { return mPos.y; }
	
	// size get,set
	void SetSize(RECT size) { mSize = size; }
	RECT GetSize() { return mSize; }

	// object tag get,set
	void SetTag(string tagName) { mTag = tagName; }
	string GetTag() { return mTag; }
public :
	// 초기화
	virtual void Init();
	// update loop 현재는 update 필요 없음
	virtual void Update(float dt);
	// 매 frame render
	virtual void Draw();
public:
	GameObject();
	GameObject(SpriteBatch* pBatch);
	GameObject(SpriteBatch* pBatch ,ID3D11ShaderResourceView* pTexture);
	virtual ~GameObject();
};

