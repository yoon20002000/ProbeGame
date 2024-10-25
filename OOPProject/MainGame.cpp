#include "stdafx.h"
#include "MainGame.h"
#include <fstream>
#include <regex>

bool MainGame::LoadStage(string fileName)
{
	// txt파일의 open 여부를 저장하여 예외처리 하기 위한 변수
	bool stageCheck = false;

	// 입력된 파일 이름을 통해 찾으며 txt를 붙여 줌.
	string tempName = fileName;
	tempName += ".txt";

	// 공의 갯수 재 초기화
	mBallNum = 0;
	// 게임의 끝 초기화
	mEndCheck = false;
	
	// stage@.txt open
	ifstream inputFile(tempName.c_str(), ifstream::binary);
	// open 상태 체크
	stageCheck = inputFile.is_open();

	// 파일이 있을 경우 실행
	if (stageCheck)
	{
		// 한 줄을 읽어 저장하기 위한 변수
		string strLine;

		// 첫줄은 너비
		getline(inputFile, strLine);
		mStageWidth = stoi(strLine);

		// 둘째줄은 높이
		getline(inputFile, strLine);
		mStageHeight = stoi(strLine);

		// 너비 높이 데이터를 기반으로 스테이지 크기 할당
		mvStage.assign(mStageHeight, vector<unsigned char>(mStageWidth));

		int x = 0, y = 0;

		// 파일이 끝날때 까지 검사
		while (!inputFile.eof())
		{
			getline(inputFile, strLine);
			x = 0;

			for (auto ch : strLine)
			{
				unsigned char t;
				switch (ch)
				{
				case '#':
					t = OBJ_WALL;
					break;
				case ' ':
					t = OBJ_SPACE;
					break;
				case 'o':
					t = OBJ_BALL;
					mBallNum++;
					break;
				case 'O':
					t = OBJ_BALL | OBJ_GOAL;
					mBallNum++;
					break;
				case '.':
					t = OBJ_GOAL;
					break;
				case 'p':
					t = OBJ_PLAYER;
					break;
				case 'P':
					t = OBJ_PLAYER || OBJ_GOAL;
					break;
				default:
					t = OBJ_UNKNOWN;
					break;
				}
				// 알맞은 데이터가 들어왔을때만 값을 넣음
				if (t != OBJ_UNKNOWN)
				{
					mvStage[y][x] = t;
					x++; // 다음 단어 탐색
				}
			}
			y++; // 다음 줄 탐색
		}
	}
	else
	{
		// 최초 게임 stage를 부를때 만약 stagedata가 없으면 예외 처리 해 줄 예정이었으나
		// 해당 부분에 구문을 초기화하면 게임이 끝났을때도 뜨기 때문에 다른 방법을 찾을 예정
		// MessageBox(NULL, L"Stage Data Null", L"Error", MB_OK);
	}

	return stageCheck;
}
// 현재 플레이 중인 stage 저장
void MainGame::SaveStage()
{
	string tempName = "stageData";
	tempName += to_string(mStageIndex);

	ofstream saveFile;
	saveFile.open("Save.txt", ios_base::out);
	saveFile << tempName.c_str();
	saveFile.close();

}

MainGame::MainGame() : mStageWidth(0), mStageHeight(0),mBallNum(0),mEndCheck(false),mStageName("stageData1"),mStageIndex(1)
{
	// save data 가 있는지 체크
	ifstream loadStage("Save.txt", ifstream::binary);

	// 있을 시 loading
	if (loadStage.is_open())
	{
		string strLine;

		getline(loadStage, strLine);

		mStageName = strLine;
		mStageIndex = stoi(strLine.substr(9, 9));
	}

}


MainGame::~MainGame()
{
}

void MainGame::InitD3D(HWND hWnd)
{
	Frame::InitD3D(hWnd);
	// stage load
	const bool result = LoadStage(mStageName);

	// false 일때만 ui 창
	UIWindow(result);

	// texture loading
	CreateTexture();

	// player 클래스 생성, 등록, 및 초기화
	Player* pPlayer = ObjectrManager::Call().Create<Player>(mspSpriteBatch.get());
	pPlayer->Init();

	// wall 클래스 생성, 등록, 및 초기화
	Wall* pWall = ObjectrManager::Call().Create<Wall>(mspSpriteBatch.get());
	pWall->Init();

	// ball 클래스 생성, 등록, 및 초기화
	Ball* pBlock = ObjectrManager::Call().Create<Ball>(mspSpriteBatch.get());
	pBlock->Init();

	// goal 클래스 생성, 등록, 및 초기화
	Goal* pGoal = ObjectrManager::Call().Create<Goal>(mspSpriteBatch.get());
	pGoal->Init();
}

void MainGame::Update(float dt)
{
	Frame::Update(dt);

	// stage가 끝났으면 출력 되는 ui, uiwindows 함수와 합병 필요 예상 파라미터를 하나 더 추가하거나
	// enum 으로 상태값을 정의하여 사용 하면 좋을것으로 판단
	if(mEndCheck)
	{
		const int result = MessageBox(NULL, L"Game Clear", L"Next Stage?", MB_YESNO);
		// 다음 stage를 위해 index++
		mStageIndex++;
		// auto save
		SaveStage();
		string tempName = "stageData";
		tempName += to_string(mStageIndex);
		mStageName = tempName;

		switch (result)
		{
		case IDYES:
		{
			const int check = LoadStage(mStageName);

			UIWindow(check);
			break;
		}
			
		case IDNO:
			PostQuitMessage(0);
			break;
		case IDCANCEL:
			break;
		}
	}

	char input = 0;

	// character 상,하,좌,우 이동
	if (KeyboardTracker.released.Up)
	{
		input = 'w';
	}
	else if (KeyboardTracker.released.Down)
	{
		input = 's';
	}

	if (KeyboardTracker.released.Left)
	{
		input = 'a';
	}
	else if (KeyboardTracker.released.Right)
	{
		input = 'd';
	}	

	// 게임 reset
	if (KeyboardTracker.released.R)
	{
		LoadStage(mStageName);
	}

	// 게임 save
	if (KeyboardTracker.released.T)
	{
		SaveStage();
		// save 했다고 ui로 알려주는 편이 좋다고 판단
	}

	if (input != 0)
	{
		// input이 들어왔을때만 updatestage를 해 줌으로서 연상량 감소
		UpdateStage(input);
	}

	CheckEndGame();

}

void MainGame::UpdateStage(char input)
{
	// input check
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a':
		dx = -1;
		break;
	case 'd':
		dx = 1;
		break;
	case 'w':
		dy = -1;
		break;
	case 's':
		dy = 1;
		break;
	}

	// player position check
	int x = 0;
	int y = 0;
	bool check = false;

	for (; y < mStageHeight; y++)
	{
		for (; x < mStageWidth; x++)
		{
			if ((mvStage[y][x] & ~OBJ_GOAL) == OBJ_PLAYER)
			{
				check = true;
				break;
			}
		}
		if (check) { break; }
		x = 0;
	}

	if (!check)
	{
		// error point
	}
	else if ((dx != 0 || dy != 0) && check)
	{
		// 캐릭터 위치
		auto player = mvStage[y][x];
		auto pplayer = &mvStage[y][x];

		// 앞의 물체 
		int tx = x + dx;
		int ty = y + dy;

		auto front = mvStage[ty][tx];
		auto pfront = &mvStage[ty][tx];

		// bit 연산을 이용해 물체 처리
		switch (front)
		{
			case OBJ_WALL: { break; }
			// 공일 경우 공의 앞까지 체크하여 밀수 있을 경우 밈
			case OBJ_BALL:
			case OBJ_BALL | OBJ_GOAL:
			{
				int px = dx + tx;
				int py = dy + ty;

				auto ffront = mvStage[py][px];
				auto pffront = &mvStage[py][px];

				if (ffront == OBJ_GOAL)
				{
					*pplayer = player & ~OBJ_PLAYER;
					*pfront = (front &~OBJ_BALL) | OBJ_PLAYER;
					*pffront = OBJ_GOAL | OBJ_BALL;
				}
				else if (ffront == OBJ_SPACE)
				{
					*pplayer = player & ~OBJ_PLAYER;
					*pfront = (front &~OBJ_BALL) | OBJ_PLAYER;
					*pffront = OBJ_SPACE | OBJ_BALL;
				}
				break;

			}		
			case OBJ_GOAL:
			{
				mvStage[y][x] = mvStage[y][x] & ~OBJ_PLAYER;
				mvStage[ty][tx] = front | OBJ_PLAYER;
				break;
			}
			
			case OBJ_SPACE:
			{
				mvStage[y][x] = mvStage[y][x] & ~OBJ_PLAYER;
				mvStage[ty][tx] = OBJ_PLAYER;
				break;
			}
		}
	}

}
void MainGame::CheckEndGame()
{

	int check = 0;

	// stage data 중 공|골 체크
	for (auto raw : mvStage)
	{
		for (auto element : raw)
		{
			if (static_cast<int>(element) == (OBJ_BALL | OBJ_GOAL)) { check++; }
		}
	}

	// 초기 공 갯수와 같을경우
	if (check == mBallNum) 
	{ 
		mEndCheck = true;
	}

}
// 현재는 맨 마지막 stage를 클리어 했을때 뜨는 ui
void MainGame::UIWindow(bool result)
{
	if (!result)
	{
		const int result = MessageBox(NULL, L"Yes : RePlay, No : EndGame", L"Thanks for playing", MB_YESNO);
		switch (result)
		{
		case IDYES:
			mStageIndex = 1;
			mStageName = "stageData";
			mStageName += to_string(mStageIndex);
			SaveStage();
			LoadStage(mStageName);
			break;
		case IDNO:
			mStageIndex--;
			SaveStage();
			PostQuitMessage(0);
			break;
		}
	}
}
// 주 화면
void MainGame::Render()
{
	XMFLOAT2 pos = { 0,0 };

	mspSpriteBatch->Begin();

	mspSpriteBatch->Draw(mpBackgroundTexture, XMFLOAT2(0, 0));

	// 각 object 마다 object의 위치값을 정해 준 뒤 해당 위치에 그림을 그려줌
	// 모든 object의 texture sizse 는 32,32 pixel(player height제외)로 고정시켰기 때문에
	// +32를 통해 다음 칸을 지정
	// 모든 칸 마다 오브젝트를 부여하여 objectmanager에 등록하려 하였으나.
	// 데이터 값의 변화는 캐릭터만 있기 때문에 굳이 모든 오브젝트를 메모리에 담고 있을 필요가 없다고 판단하여 사용하지않고
	// 기본 틀만 받아와 필요한 위치에 이미지를 출력 하게 만듬
	for (auto row : mvStage)
	{
		for (auto element : row)
		{
			switch (element)
			{
			case OBJ_PLAYER:
			{
				Player* pPlayer = (Player*)(ObjectrManager::Call().GetOject("Player"));
				pPlayer->SetPosition(pos);
				pPlayer->Draw();
				break;
			}
			case OBJ_PLAYER | OBJ_GOAL:
			{

				Goal* pGoal = (Goal*)(ObjectrManager::Call().GetOject("Goal"));
				pGoal->SetPosition(pos);
				pGoal->Draw();

				Player* pPlayer = (Player*)(ObjectrManager::Call().GetOject("Player"));
				pPlayer->SetPosition(pos);
				pPlayer->Draw();
				break;
			}
			case OBJ_WALL:
			{
				Wall* pWall = (Wall*)(ObjectrManager::Call().GetOject("Wall"));
				pWall->SetPosition(pos);
				pWall->Draw();
				break;
			}

			case OBJ_BALL:
			{
				Ball* pBall = (Ball*)(ObjectrManager::Call().GetOject("Ball"));
				pBall->SetPosition(pos);
				pBall->Draw();
				break;
			}

			case OBJ_GOAL:
			{
				Goal* pGoal = (Goal*)(ObjectrManager::Call().GetOject("Goal"));
				pGoal->SetPosition(pos);
				pGoal->Draw();
				break;
			}
			case OBJ_GOAL | OBJ_BALL:
			{

				Goal* pGoal = (Goal*)(ObjectrManager::Call().GetOject("Goal"));
				pGoal->SetPosition(pos);
				pGoal->Draw();

				Ball* pBall = (Ball*)(ObjectrManager::Call().GetOject("Ball"));
				pBall->SetPosition(pos);
				pBall->Draw();
				break;

			}

			}
			pos.x += 32;
		}
		pos.x = 0;
		pos.y += 32;
	}
	mspSpriteBatch->End();

}

// texure load
void MainGame::CreateTexture()
{
	ResourceManager::Call().GetTexture(L"Images\\Ball.png");
	ResourceManager::Call().GetTexture(L"Images\\Goal.png");
	ResourceManager::Call().GetTexture(L"Images\\Player.png");
	ResourceManager::Call().GetTexture(L"Images\\Wall.png");
	ResourceManager::Call().GetTexture(L"Images\\Background.jpg");

	mpBackgroundTexture = ResourceManager::Call().GetTexture(L"Background");
}
