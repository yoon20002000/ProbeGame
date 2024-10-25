#include "stdafx.h"
#include "ResourceManager.h"
#include <sstream>
ID3D11ShaderResourceView * ResourceManager::GetTexture(wstring fileName)
{
	// device 정보 null check
	if (mpDevice == NULL)
	{
		DXTRACE_ERR_MSGBOX(L"mpDevice is Null. Call Init() first",0);
		return NULL;
	}

	// 경로 및 확장자 삭제
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
	
	// 임시 resource view 
	ID3D11ShaderResourceView *pResource = NULL;
	// 먼저 map에 빈 class를 넣어 둠
	auto result = mTextures.insert(pair<wstring, ComPtr<ID3D11ShaderResourceView>>(str, nullptr));

	// 반환 되는 값 first -> iter / second -> insert complete or false bool
	if (result.second == true)
	{
		// 해당 resourceview 초기화 및 해당 value class에 불러온 texture 저장
		result.first->second = ComPtr<ID3D11ShaderResourceView>();
		// 만약 없을때 예외처리가 필요 할 것으로 생각
		CreateWICTextureFromFile(mpDevice, fileName.c_str(), NULL, result.first->second.ReleaseAndGetAddressOf());
		// 그 후 해당 pointer를 전달 하여 저장
		pResource = result.first->second.Get();
	}
	// 이미 있을 경우 해당 point를 전달
	else { pResource = result.first->second.Get(); }

	return pResource;
}

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{
}
