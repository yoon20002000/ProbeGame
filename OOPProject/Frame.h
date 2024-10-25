#pragma once
// directx를 이용해 게임을 만드는 가장 기본적인 framework로 해당 클래스를 상속하여
// 다양한 게임을 만들 목적으로 만들었으며 현 기말 프로젝트에서 사용 중.
class Frame
{
protected:
	// gpu 장치가 가지고있는 swapchian을 가져와 가지고 있는 클래스				
	ComPtr<IDXGISwapChain>			mspSwapchain;
	// gpu 장치 저장 클래스 버퍼/텍스쳐 등의 리소스를 만들고, 리소스에 접근하는 뷰를 만듬.
	ComPtr<ID3D11Device>			mspDevice;
	// 파이프라인을 설정하거나 실제로 화면에 그림
	ComPtr<ID3D11DeviceContext>		mspDeviceCon;
	// frame마다 그림을 그려줄 뷰
	ComPtr<ID3D11RenderTargetView>	mspTargetView;
	// 깊이스텐실 버퍼
	ComPtr<ID3D11Texture2D>			mspDepthStencilBuffer;
	// 버퍼를 이용해 만든 뷰
	ComPtr<ID3D11DepthStencilView>	mspDepthStencilView;
	// direct에서 지원하는 keyboard input 관련 변수
	unique_ptr<Keyboard>			mspKeyboard;
	// direct에서 지원하는 mouse input 관련 변수
	unique_ptr<Mouse>				mspMouse;
	// image를 그리기 위해서 필요한 클래스
	unique_ptr<SpriteBatch>			mspSpriteBatch;

	// 화면 해상도 입력 변수
	UINT							mScreenWidth;
	UINT							mScreenHeight;

	// 현재 화면이 최대,최소,사이즈 변화를 체크하기 위한 부울 변수
	bool							mbMinimized;
	bool							mbMaximized;
	bool							mbResizing;

	// 현재 게임이 정지 돼 있는지 체크하기 위한 변수
	bool							mbPaused;
	// 현재 windows의 핸들
	HWND							mHwnd;
	// windows 상단 title
	wstring							mTitleCaption;
	// f게임의 시간을 관장하는 클래스
	Timer							mTimer;
public:
	// 현재 keyboard와 mouse 상태를 지속적으로 트래킹하는 클래스
	static DirectX::Keyboard::KeyboardStateTracker	KeyboardTracker;
	static DirectX::Mouse::ButtonStateTracker		MouseTracker;

public:
	// 기본적으로 directx를 위해 처리해 줘야하는 초기화
	virtual void InitD3D(HWND hWnd);
	// 매 frame마다 반복되는 함수
	virtual void Update(float dt);
	// frame마다 rendering 하기 전 화면 초기화
	virtual void BeginRender();
	// frame마다 rendering 끝날때 닫아줘야하는 명령어가 실행되는 함수
	virtual void EndRender();
	// frame마다 renderring 해 줄 코드를 적는 곳
	virtual void Render();
	// 사이즈가 바뀌었을때 출력 되는 함수
	virtual void OnResize();
	// 게임을 끄면서 소멸시켜줘야 하는 것들 한번에 처리
	virtual void ClearD3D();
public:
	// windows를 생성할때 필요한 초기화 함수
	void InitWindow(HINSTANCE hInstance, LPCTSTR title = L"20121306김학윤", UINT wParam = 640, LPARAM lParam = 480);
	// windows 매 프레임마다 발생하는 msg를 체크하기 위해 발생하는 콜백 함수
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// game의 루프문에 해당하는 부분
	int GameLoop();
public:
	Frame();
	~Frame();
};