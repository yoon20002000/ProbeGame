// OOPProject.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "OOPProject.h"
#include "MainGame.h"

// windows main
int WINAPI wWinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPWSTR    lpCmdLine,
                   int       nCmdShow)
{
	// main game class
	MainGame mainGame;
	// main game Init
	mainGame.InitWindow(hInstance);

	// if any return come end game
	return mainGame.GameLoop();
}
