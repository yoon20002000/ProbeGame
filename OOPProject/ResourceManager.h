#pragma once
class ResourceManager final
{
private:
	// ��� texture�� key value ���� pair�� ���� �ϴ� map
	map<wstring,ComPtr<ID3D11ShaderResourceView>> mTextures;
public:
	// singleton�� ���� static �Լ�
	static ResourceManager& Call()
	{
		static ResourceManager instance;
		return instance;
	}
private:
	// wictexture�� �ϴµ� �־� �ʿ��� device������ ������ �α� ���� class
	ID3D11Device* mpDevice;

public :
	void Init(ID3D11Device* pDevice) { mpDevice = pDevice; }

public:
	// ���� �� �ҷ�����
	ID3D11ShaderResourceView * GetTexture(wstring fileName);

// �����ڸ� ����� ���� ���⿡ private
private:
	ResourceManager();
public:
	~ResourceManager();
};

