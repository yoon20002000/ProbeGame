#pragma once
// gameobject�� �⺻������ ������ �ִ�
class GameObject
{
protected :
	XMFLOAT2 mPos;
	RECT mSize;
	string mTag;
protected :
	// texture ������ resource view
	ID3D11ShaderResourceView*	mpTexture;
	// �׸��� ���� �ʿ��� sprite 
	SpriteBatch*				mpBatch;

public :
	// image ��ġ ��
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
	// �ʱ�ȭ
	virtual void Init();
	// update loop ����� update �ʿ� ����
	virtual void Update(float dt);
	// �� frame render
	virtual void Draw();
public:
	GameObject();
	GameObject(SpriteBatch* pBatch);
	GameObject(SpriteBatch* pBatch ,ID3D11ShaderResourceView* pTexture);
	virtual ~GameObject();
};

