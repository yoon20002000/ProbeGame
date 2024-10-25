#pragma once
#include "Frame.h"
class MainLobby :
	public Frame
{

public:
	MainLobby();
	~MainLobby();

private:
	bool								mSelect;
private:
	ComPtr<ID3D11ShaderResourceView>	mspTexture;

public:
	virtual void InitD3D(HWND hWnd);
	virtual void Update(float dt);
	virtual void Render();

	// ��ǲ �޾Ƽ� ���� �����ϵ���

public:
	int LobbyGameLoop();

};

