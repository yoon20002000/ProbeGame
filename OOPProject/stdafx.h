// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� ����
// Windows header
#include <windows.h>
#include <Windowsx.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <list>

// Directx TK
#include <d3d11.h>
#pragma comment (lib,"d3d11.lib")

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <WICTextureLoader.h>
#include "dxerr.h"

// namesapce
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Defines.h"
#include "Timer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "ObjectrManager.h"
#include "Frame.h"
#include "Player.h"
#include "Wall.h"
#include "Ball.h"
#include "Goal.h"
