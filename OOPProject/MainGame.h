#pragma once
#include "Frame.h"
class MainGame :
	public Frame
{

private:
	// define �ص� enum�� ���� ��ǥ�� �ش� ������ �Է½�Ŵ
	vector<vector<unsigned char>>		mvStage;
	// ���� stage�� �ʺ� ���� �� ����
	int									mStageWidth;
	int									mStageHeight;
	// ���� ���� üũ �⺻ ���� ������ ��|�� ������ ������ üũ �ϱ� ���� ���
	int									mBallNum;
	// stage�̸� �����ϱ� ���� ����
	string								mStageName;
	// ���� stage�� index 
	int									mStageIndex;
	// ������ �������� üũ�ϱ� ���� end ����
	bool								mEndCheck;
	// background �������� ����� resource view Ŭ����
	ID3D11ShaderResourceView*			mpBackgroundTexture;
public:
	MainGame();
	virtual ~MainGame();
public:
	// ��ӵ� �� �̿ܿ� �߰������� �ʿ��� �۾��� �߰�
	virtual void InitD3D(HWND hWnd);
	virtual void Update(float dt);
	virtual void Render();
private:
	// ��� texture loading
	void CreateTexture();

private:
	// ���� ������� ���� ���� stage ����
	bool LoadStage(string fileName);
	// ���� stage ���� Save ���Ϸ� �����ϸ� txt���Ϸ� �����
	void SaveStage();
	// �� frame ����� ����Ű�� input���� �޾ƿ� �ش� ���� ���� character ����
	void UpdateStage(char input);
	// ������ �������� üũ�ϱ� ���� �Լ�
	void CheckEndGame();
	// ������ ui���� �Ѳ����� ó�� �� �Լ�
	void UIWindow(bool result);
};

