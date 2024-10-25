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

	// 인풋 받아서 게임 실행하도록

public:
	int LobbyGameLoop();

};

