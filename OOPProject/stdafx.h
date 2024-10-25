// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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
