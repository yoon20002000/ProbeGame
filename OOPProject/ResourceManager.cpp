#include "stdafx.h"
#include "ResourceManager.h"
#include <sstream>
ID3D11ShaderResourceView * ResourceManager::GetTexture(wstring fileName)
{
	// device ���� null check
	if (mpDevice == NULL)
	{
		DXTRACE_ERR_MSGBOX(L"mpDevice is Null. Call Init() first",0);
		return NULL;
	}

	// ��� �� Ȯ���� ����
	int temp = fileName.find_first_of(L".");
	wstring str ;
	if (temp == -1)
	{
		str = fileName;
	}
	else
	{
		str = fileName.substr(7, temp - 7);
	}
	
	// �ӽ� resource view 
	ID3D11ShaderResourceView *pResource = NULL;
	// ���� map�� �� class�� �־� ��
	auto result = mTextures.insert(pair<wstring, ComPtr<ID3D11ShaderResourceView>>(str, nullptr));

	// ��ȯ �Ǵ� �� first -> iter / second -> insert complete or false bool
	if (result.second == true)
	{
		// �ش� resourceview �ʱ�ȭ �� �ش� value class�� �ҷ��� texture ����
		result.first->second = ComPtr<ID3D11ShaderResourceView>();
		// ���� ������ ����ó���� �ʿ� �� ������ ����
		CreateWICTextureFromFile(mpDevice, fileName.c_str(), NULL, result.first->second.ReleaseAndGetAddressOf());
		// �� �� �ش� pointer�� ���� �Ͽ� ����
		pResource = result.first->second.Get();
	}
	// �̹� ���� ��� �ش� point�� ����
	else { pResource = result.first->second.Get(); }

	return pResource;
}

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{
}
