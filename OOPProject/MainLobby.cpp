#include "stdafx.h"
#include "MainLobby.h"


MainLobby::MainLobby() : mSelect(false)
{
}


MainLobby::~MainLobby()
{
}

void MainLobby::InitD3D(HWND hWnd)
{
	Frame::InitD3D(hWnd);
	CreateWICTextureFromFile(mspDevice.Get(), L"Images\\Lobby.jpg", NULL, mspTexture.ReleaseAndGetAddressOf());
}

void MainLobby::Update(float dt)
{
	Frame::Update(dt);
}

void MainLobby::Render()
{
	Frame::Render();

	RECT r = { 0,0,mScreenWidth,mScreenHeight };
	XMFLOAT2 pos = { 0,0 };

	mspSpriteBatch->Begin();
	mspSpriteBatch->Draw(mspTexture.Get(),pos);
	mspSpriteBatch->End();

}

int MainLobby::LobbyGameLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) { break; }
		}
		else
		{
			if (!mbPaused)
			{
				Frame::KeyboardTracker.Update(mspKeyboard->GetState());
				Frame::MouseTracker.Update(mspMouse->GetState());
				Update(mTimer.DeltaTime());
				BeginRender();
				Render();
				EndRender();
			}
			else
			{
				Sleep(100);
			}
		}

	}

	ClearD3D();

	return msg.wParam;
}


