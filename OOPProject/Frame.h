#pragma once
// directx�� �̿��� ������ ����� ���� �⺻���� framework�� �ش� Ŭ������ ����Ͽ�
// �پ��� ������ ���� �������� ��������� �� �⸻ ������Ʈ���� ��� ��.
class Frame
{
protected:
	// gpu ��ġ�� �������ִ� swapchian�� ������ ������ �ִ� Ŭ����				
	ComPtr<IDXGISwapChain>			mspSwapchain;
	// gpu ��ġ ���� Ŭ���� ����/�ؽ��� ���� ���ҽ��� �����, ���ҽ��� �����ϴ� �並 ����.
	ComPtr<ID3D11Device>			mspDevice;
	// ������������ �����ϰų� ������ ȭ�鿡 �׸�
	ComPtr<ID3D11DeviceContext>		mspDeviceCon;
	// frame���� �׸��� �׷��� ��
	ComPtr<ID3D11RenderTargetView>	mspTargetView;
	// ���̽��ٽ� ����
	ComPtr<ID3D11Texture2D>			mspDepthStencilBuffer;
	// ���۸� �̿��� ���� ��
	ComPtr<ID3D11DepthStencilView>	mspDepthStencilView;
	// direct���� �����ϴ� keyboard input ���� ����
	unique_ptr<Keyboard>			mspKeyboard;
	// direct���� �����ϴ� mouse input ���� ����
	unique_ptr<Mouse>				mspMouse;
	// image�� �׸��� ���ؼ� �ʿ��� Ŭ����
	unique_ptr<SpriteBatch>			mspSpriteBatch;

	// ȭ�� �ػ� �Է� ����
	UINT							mScreenWidth;
	UINT							mScreenHeight;

	// ���� ȭ���� �ִ�,�ּ�,������ ��ȭ�� üũ�ϱ� ���� �ο� ����
	bool							mbMinimized;
	bool							mbMaximized;
	bool							mbResizing;

	// ���� ������ ���� �� �ִ��� üũ�ϱ� ���� ����
	bool							mbPaused;
	// ���� windows�� �ڵ�
	HWND							mHwnd;
	// windows ��� title
	wstring							mTitleCaption;
	// f������ �ð��� �����ϴ� Ŭ����
	Timer							mTimer;
public:
	// ���� keyboard�� mouse ���¸� ���������� Ʈ��ŷ�ϴ� Ŭ����
	static DirectX::Keyboard::KeyboardStateTracker	KeyboardTracker;
	static DirectX::Mouse::ButtonStateTracker		MouseTracker;

public:
	// �⺻������ directx�� ���� ó���� ����ϴ� �ʱ�ȭ
	virtual void InitD3D(HWND hWnd);
	// �� frame���� �ݺ��Ǵ� �Լ�
	virtual void Update(float dt);
	// frame���� rendering �ϱ� �� ȭ�� �ʱ�ȭ
	virtual void BeginRender();
	// frame���� rendering ������ �ݾ�����ϴ� ��ɾ ����Ǵ� �Լ�
	virtual void EndRender();
	// frame���� renderring �� �� �ڵ带 ���� ��
	virtual void Render();
	// ����� �ٲ������ ��� �Ǵ� �Լ�
	virtual void OnResize();
	// ������ ���鼭 �Ҹ������� �ϴ� �͵� �ѹ��� ó��
	virtual void ClearD3D();
public:
	// windows�� �����Ҷ� �ʿ��� �ʱ�ȭ �Լ�
	void InitWindow(HINSTANCE hInstance, LPCTSTR title = L"20121306������", UINT wParam = 640, LPARAM lParam = 480);
	// windows �� �����Ӹ��� �߻��ϴ� msg�� üũ�ϱ� ���� �߻��ϴ� �ݹ� �Լ�
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// game�� �������� �ش��ϴ� �κ�
	int GameLoop();
public:
	Frame();
	~Frame();
};