#include "stdafx.h"
#include "Frame.h"

// 기본적인 initwindows에서 callback 함수가 필요한대 frame에서 만든 맴버 함수이기 때문에
// 이를 넘겨 주기 위해서는 전역 변수로 만든 뒤 해당 windowproc을 넘겨주는 방식을 사용하기 위해
// 만든 포인터 변수, static을 이용해서도 가능 함
Frame* gpDispatch = 0;

DirectX::Keyboard::KeyboardStateTracker Frame::KeyboardTracker;
DirectX::Mouse::ButtonStateTracker Frame::MouseTracker;

LRESULT CALLBACK GlobalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return gpDispatch->WindowProc(hWnd, message, wParam, lParam);
}

Frame::Frame() : mScreenWidth(WINDOW_SIZEX), mScreenHeight(WINDOW_SIZEY), mbMinimized(false), mbMaximized(false), mbResizing(false), mHwnd(NULL)
{
	gpDispatch = this;
}


Frame::~Frame()
{
}

void Frame::InitD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = mScreenWidth;
	scd.BufferDesc.Height = mScreenHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = true;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// swapchain 등록
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, mspSwapchain.GetAddressOf(), mspDevice.GetAddressOf(), NULL, mspDeviceCon.GetAddressOf());

	// image texture를 저장하기 위한 gpu device정보 전달 초기화
	ResourceManager::Call().Init(mspDevice.Get());

	mspKeyboard = make_unique<Keyboard>();
	mspMouse = make_unique<Mouse>();
	mspMouse->SetWindow(hWnd);

	mspSpriteBatch = make_unique<SpriteBatch>(mspDeviceCon.Get());

	OnResize();
}

// 게임의 주된 연산 부분들. 현재는 모든 object를 실시간으로 이동 시키지 않기때문에 사용 하지 않기로 함.
void Frame::Update(float dt)
{
	//ObjectrManager::Call().Update(dt);
}

void Frame::BeginRender()
{
	// view를 특정 색으로 초기화 한뒤 깊이 스텐실 버퍼 또한 초기화.
	mspDeviceCon->ClearRenderTargetView(mspTargetView.Get(), Colors::LawnGreen);
	mspDeviceCon->ClearDepthStencilView(mspDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
// render 사용 안함 objectmanager를 생성 및 택스쳐 불러오기용으로 사용하여 frame에서 할일이 줄어듬
void Frame::Render()
{
	//mspSpriteBatch->Begin();
	//ObjectrManager::Call().Draw();
	//mspSpriteBatch->End();
}

void Frame::EndRender()
{
	// double buffer 기법에서 스위칭 해주는 함수
	mspSwapchain->Present(0, 0);
}

void Frame::OnResize()
{
	// target view 초기화
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mspDeviceCon->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	mspTargetView.Reset();
	mspDepthStencilView.Reset();
	mspDeviceCon->Flush();
	// resize 버퍼에 항당 초기화
	mspSwapchain->ResizeBuffers(1, mScreenWidth, mScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ComPtr<ID3D11Texture2D> pBackBuffer;
	mspSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(pBackBuffer.ReleaseAndGetAddressOf()));

	// 만들어 둔 targetview를 현 target 뷰에 등록
	mspDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &mspTargetView);

	// depthstencil 초기화
	D3D11_TEXTURE2D_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
	dsd.Width = mScreenWidth;
	dsd.Height = mScreenHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 4;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// 최종 초기화 등록 
	mspDevice->CreateTexture2D(&dsd, 0, mspDepthStencilBuffer.ReleaseAndGetAddressOf());
	mspDevice->CreateDepthStencilView(mspDepthStencilBuffer.Get(), 0, mspDepthStencilView.ReleaseAndGetAddressOf());
	mspDeviceCon->OMSetRenderTargets(1, mspTargetView.GetAddressOf(), mspDepthStencilView.Get());

	// 최종적 viewport 재 정의
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(mScreenWidth);
	viewport.Height = static_cast<float>(mScreenHeight);

	mspDeviceCon->RSSetViewports(1, &viewport);
}

void Frame::ClearD3D()
{
	// 전체화면 이었을시 해당 기능 해제
	mspSwapchain->SetFullscreenState(false, NULL);
}
// windows 기본 초기화 함수
void Frame::InitWindow(HINSTANCE hInstance, LPCTSTR title, UINT wParam, LPARAM lParam)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"OOPProject";
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = GlobalWindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, static_cast<long>(mScreenWidth), static_cast<long>(mScreenHeight) };
	// 메뉴 카테고리 비포함 실제 크기를 정의하기위해 조정
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mTitleCaption = title;
	mHwnd = CreateWindowEx(NULL, L"OOPProject", title , WS_OVERLAPPEDWINDOW, WINDOW_START_POSX, WINDOW_START_POSY
		, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(mHwnd, SW_SHOW);

	InitD3D(mHwnd);
}

LRESULT Frame::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
		// wparam의 lowword 부분을 잘라와 현재 상태값이 비활성화 상태인지 체크
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mbPaused = true;
			mTimer.Stop();
		}
		else
		{
			mbPaused = false;
			mTimer.Resumte();
		}
		break;
		// resize가 일어났을때 발생하는 msg로 lparam의 low,highword를 잘라내 크기를 읽어냄
	case WM_SIZE:
		mScreenWidth = LOWORD(lParam);
		mScreenWidth = HIWORD(lParam);
		// resize로 인해 발생하는 msg들 각각 처리하는 부분
		if (mspDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mbPaused = true;
				mbMinimized = true;
				mbMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mbPaused = false;
				mbMinimized = false;
				mbMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mbMinimized)
				{
					mbPaused = false;
					mbMinimized = false;
					OnResize();
				}
				else if (mbMaximized)
				{
					mbMaximized = false;
					OnResize();
				}
				else if (mbResizing)
				{

				}
				else
				{
					OnResize();
				}
			}
		}
		return 0;
		break;

	case WM_ENTERSIZEMOVE:
		mbPaused = true;
		mbResizing = true;
		mTimer.Resumte();
		return 0;
		break;
	case WM_EXITSIZEMOVE:
		mbPaused = false;
		mbResizing = false;
		mTimer.Resumte();
		OnResize();
		return 0;
		break;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
		break;

		// 최대 최소크기를 지정 이하로는 줄일 수 없음
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
		return 0;
		break;

		// 다른 창에 들어왔다가 다시 실행돼었을때 각 keyboard와 mouse값 초기화
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		Frame::MouseTracker.Reset();
		Frame::KeyboardTracker.Reset();
		break;

		// 각 입력이 들어왔을때 작동하는 msg
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

		// window 종료 msg
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}
	// 이외 이벤트가 없을때 발생하는 msg
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int Frame::GameLoop()
{
	// 게임 시작시 timer start
	mTimer.Start();
	MSG msg;
	while (true)
	{
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// msg 번역
			TranslateMessage(&msg);
			// msg 처리
			DispatchMessage(&msg);
			// 게임 종료 msg시 종료
			if (msg.message == WM_QUIT) { break; }
		}
		else
		{
			// timer 지속적 시간 흐름 처리
			mTimer.Update();
			// 게임이 정지돼 있지 않을 경우만 모든 구문 실행
			if (!mbPaused)
			{
				Frame::KeyboardTracker.Update(mspKeyboard->GetState());
				Frame::MouseTracker.Update(mspMouse->GetState());
				Update(mTimer.DeltaTime());
				BeginRender();
				Render();
				EndRender();
			}
			else // frame 속도를 약간 조정하기 위한 sleep
			{
				Sleep(100);
			}
		}

	}
	
	ClearD3D();

	return msg.wParam;
}
