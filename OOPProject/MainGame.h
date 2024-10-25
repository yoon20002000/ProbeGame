#pragma once
#include "Frame.h"
class MainGame :
	public Frame
{

private:
	// define 해둔 enum을 통해 좌표에 해당 값들을 입력시킴
	vector<vector<unsigned char>>		mvStage;
	// 게임 stage의 너비 높이 값 저장
	int									mStageWidth;
	int									mStageHeight;
	// 공의 갯수 체크 기본 공의 갯수와 공|골 갯수가 같은지 체크 하기 위해 사용
	int									mBallNum;
	// stage이름 저장하기 위한 변수
	string								mStageName;
	// 현재 stage의 index 
	int									mStageIndex;
	// 게임이 끝났는지 체크하기 위한 end 변수
	bool								mEndCheck;
	// background 전용으로 사용할 resource view 클래스
	ID3D11ShaderResourceView*			mpBackgroundTexture;
public:
	MainGame();
	virtual ~MainGame();
public:
	// 상속된 것 이외에 추가적으로 필요한 작업들 추가
	virtual void InitD3D(HWND hWnd);
	virtual void Update(float dt);
	virtual void Render();
private:
	// 모든 texture loading
	void CreateTexture();

private:
	// 파일 입출력을 통해 게임 stage 저장
	bool LoadStage(string fileName);
	// 현재 stage 저장 Save 파일로 저장하며 txt파일로 추출됨
	void SaveStage();
	// 매 frame 실행됨 방향키의 input값을 받아와 해당 값을 통해 character 조종
	void UpdateStage(char input);
	// 게임이 끝났는지 체크하기 위한 함수
	void CheckEndGame();
	// 게임의 ui들을 한꺼번에 처리 할 함수
	void UIWindow(bool result);
};

