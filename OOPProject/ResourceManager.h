#pragma once
class ResourceManager final
{
private:
	// 모든 texture를 key value 값의 pair로 저장 하는 map
	map<wstring,ComPtr<ID3D11ShaderResourceView>> mTextures;
public:
	// singleton을 위한 static 함수
	static ResourceManager& Call()
	{
		static ResourceManager instance;
		return instance;
	}
private:
	// wictexture를 하는데 있어 필요한 device정보를 저장해 두기 위한 class
	ID3D11Device* mpDevice;

public :
	void Init(ID3D11Device* pDevice) { mpDevice = pDevice; }

public:
	// 생성 및 불러오기
	ID3D11ShaderResourceView * GetTexture(wstring fileName);

// 생성자를 사용할 일이 없기에 private
private:
	ResourceManager();
public:
	~ResourceManager();
};

