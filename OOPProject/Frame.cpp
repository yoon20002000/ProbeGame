#include "stdafx.h"
#include "Frame.h"

// �⺻���� initwindows���� callback �Լ��� �ʿ��Ѵ� frame���� ���� �ɹ� �Լ��̱� ������
// �̸� �Ѱ� �ֱ� ���ؼ��� ���� ������ ���� �� �ش� windowproc�� �Ѱ��ִ� ����� ����ϱ� ����
// ���� ������ ����, static�� �̿��ؼ��� ���� ��
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

	// swapchain ���
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, mspSwapchain.GetAddressOf(), mspDevice.GetAddressOf(), NULL, mspDeviceCon.GetAddressOf());

	// image texture�� �����ϱ� ���� gpu device���� ���� �ʱ�ȭ
	ResourceManager::Call().Init(mspDevice.Get());

	mspKeyboard = make_unique<Keyboard>();
	mspMouse = make_unique<Mouse>();
	mspMouse->SetWindow(hWnd);

	mspSpriteBatch = make_unique<SpriteBatch>(mspDeviceCon.Get());

	OnResize();
}

// ������ �ֵ� ���� �κе�. ����� ��� object�� �ǽð����� �̵� ��Ű�� �ʱ⶧���� ��� ���� �ʱ�� ��.
void Frame::Update(float dt)
{
	//ObjectrManager::Call().Update(dt);
}

void Frame::BeginRender()
{
	// view�� Ư�� ������ �ʱ�ȭ �ѵ� ���� ���ٽ� ���� ���� �ʱ�ȭ.
	mspDeviceCon->ClearRenderTargetView(mspTargetView.Get(), Colors::LawnGreen);
	mspDeviceCon->ClearDepthStencilView(mspDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
// render ��� ���� objectmanager�� ���� �� �ý��� �ҷ���������� ����Ͽ� frame���� ������ �پ��
void Frame::Render()
{
	//mspSpriteBatch->Begin();
	//ObjectrManager::Call().Draw();
	//mspSpriteBatch->End();
}

void Frame::EndRender()
{
	// double buffer ������� ����Ī ���ִ� �Լ�
	mspSwapchain->Present(0, 0);
}

void Frame::OnResize()
{
	// target view �ʱ�ȭ
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mspDeviceCon->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	mspTargetView.Reset();
	mspDepthStencilView.Reset();
	mspDeviceCon->Flush();
	// resize ���ۿ� �״� �ʱ�ȭ
	mspSwapchain->ResizeBuffers(1, mScreenWidth, mScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ComPtr<ID3D11Texture2D> pBackBuffer;
	mspSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(pBackBuffer.ReleaseAndGetAddressOf()));

	// ����� �� targetview�� �� target �信 ���
	mspDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &mspTargetView);

	// depthstencil �ʱ�ȭ
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

	// ���� �ʱ�ȭ ��� 
	mspDevice->CreateTexture2D(&dsd, 0, mspDepthStencilBuffer.ReleaseAndGetAddressOf());
	mspDevice->CreateDepthStencilView(mspDepthStencilBuffer.Get(), 0, mspDepthStencilView.ReleaseAndGetAddressOf());
	mspDeviceCon->OMSetRenderTargets(1, mspTargetView.GetAddressOf(), mspDepthStencilView.Get());

	// ������ viewport �� ����
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
	// ��üȭ�� �̾����� �ش� ��� ����
	mspSwapchain->SetFullscreenState(false, NULL);
}
// windows �⺻ �ʱ�ȭ �Լ�
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
	// �޴� ī�װ� ������ ���� ũ�⸦ �����ϱ����� ����
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
		// wparam�� lowword �κ��� �߶�� ���� ���°��� ��Ȱ��ȭ �������� üũ
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
		// resize�� �Ͼ���� �߻��ϴ� msg�� lparam�� low,highword�� �߶� ũ�⸦ �о
	case WM_SIZE:
		mScreenWidth = LOWORD(lParam);
		mScreenWidth = HIWORD(lParam);
		// resize�� ���� �߻��ϴ� msg�� ���� ó���ϴ� �κ�
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

		// �ִ� �ּ�ũ�⸦ ���� ���Ϸδ� ���� �� ����
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
		return 0;
		break;

		// �ٸ� â�� ���Դٰ� �ٽ� ����ž����� �� keyboard�� mouse�� �ʱ�ȭ
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		Frame::MouseTracker.Reset();
		Frame::KeyboardTracker.Reset();
		break;

		// �� �Է��� �������� �۵��ϴ� msg
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

		// window ���� msg
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}
	// �̿� �̺�Ʈ�� ������ �߻��ϴ� msg
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int Frame::GameLoop()
{
	// ���� ���۽� timer start
	mTimer.Start();
	MSG msg;
	while (true)
	{
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// msg ����
			TranslateMessage(&msg);
			// msg ó��
			DispatchMessage(&msg);
			// ���� ���� msg�� ����
			if (msg.message == WM_QUIT) { break; }
		}
		else
		{
			// timer ������ �ð� �帧 ó��
			mTimer.Update();
			// ������ ������ ���� ���� ��츸 ��� ���� ����
			if (!mbPaused)
			{
				Frame::KeyboardTracker.Update(mspKeyboard->GetState());
				Frame::MouseTracker.Update(mspMouse->GetState());
				Update(mTimer.DeltaTime());
				BeginRender();
				Render();
				EndRender();
			}
			else // frame �ӵ��� �ణ �����ϱ� ���� sleep
			{
				Sleep(100);
			}
		}

	}
	
	ClearD3D();

	return msg.wParam;
}
