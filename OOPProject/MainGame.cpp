#include "stdafx.h"
#include "MainGame.h"
#include <fstream>
#include <regex>

bool MainGame::LoadStage(string fileName)
{
	// txt������ open ���θ� �����Ͽ� ����ó�� �ϱ� ���� ����
	bool stageCheck = false;

	// �Էµ� ���� �̸��� ���� ã���� txt�� �ٿ� ��.
	string tempName = fileName;
	tempName += ".txt";

	// ���� ���� �� �ʱ�ȭ
	mBallNum = 0;
	// ������ �� �ʱ�ȭ
	mEndCheck = false;
	
	// stage@.txt open
	ifstream inputFile(tempName.c_str(), ifstream::binary);
	// open ���� üũ
	stageCheck = inputFile.is_open();

	// ������ ���� ��� ����
	if (stageCheck)
	{
		// �� ���� �о� �����ϱ� ���� ����
		string strLine;

		// ù���� �ʺ�
		getline(inputFile, strLine);
		mStageWidth = stoi(strLine);

		// ��°���� ����
		getline(inputFile, strLine);
		mStageHeight = stoi(strLine);

		// �ʺ� ���� �����͸� ������� �������� ũ�� �Ҵ�
		mvStage.assign(mStageHeight, vector<unsigned char>(mStageWidth));

		int x = 0, y = 0;

		// ������ ������ ���� �˻�
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
				// �˸��� �����Ͱ� ���������� ���� ����
				if (t != OBJ_UNKNOWN)
				{
					mvStage[y][x] = t;
					x++; // ���� �ܾ� Ž��
				}
			}
			y++; // ���� �� Ž��
		}
	}
	else
	{
		// ���� ���� stage�� �θ��� ���� stagedata�� ������ ���� ó�� �� �� �����̾�����
		// �ش� �κп� ������ �ʱ�ȭ�ϸ� ������ ���������� �߱� ������ �ٸ� ����� ã�� ����
		// MessageBox(NULL, L"Stage Data Null", L"Error", MB_OK);
	}

	return stageCheck;
}
// ���� �÷��� ���� stage ����
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
	// save data �� �ִ��� üũ
	ifstream loadStage("Save.txt", ifstream::binary);

	// ���� �� loading
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

	// false �϶��� ui â
	UIWindow(result);

	// texture loading
	CreateTexture();

	// player Ŭ���� ����, ���, �� �ʱ�ȭ
	Player* pPlayer = ObjectrManager::Call().Create<Player>(mspSpriteBatch.get());
	pPlayer->Init();

	// wall Ŭ���� ����, ���, �� �ʱ�ȭ
	Wall* pWall = ObjectrManager::Call().Create<Wall>(mspSpriteBatch.get());
	pWall->Init();

	// ball Ŭ���� ����, ���, �� �ʱ�ȭ
	Ball* pBlock = ObjectrManager::Call().Create<Ball>(mspSpriteBatch.get());
	pBlock->Init();

	// goal Ŭ���� ����, ���, �� �ʱ�ȭ
	Goal* pGoal = ObjectrManager::Call().Create<Goal>(mspSpriteBatch.get());
	pGoal->Init();
}

void MainGame::Update(float dt)
{
	Frame::Update(dt);

	// stage�� �������� ��� �Ǵ� ui, uiwindows �Լ��� �պ� �ʿ� ���� �Ķ���͸� �ϳ� �� �߰��ϰų�
	// enum ���� ���°��� �����Ͽ� ��� �ϸ� ���������� �Ǵ�
	if(mEndCheck)
	{
		const int result = MessageBox(NULL, L"Game Clear", L"Next Stage?", MB_YESNO);
		// ���� stage�� ���� index++
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

	// character ��,��,��,�� �̵�
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

	// ���� reset
	if (KeyboardTracker.released.R)
	{
		LoadStage(mStageName);
	}

	// ���� save
	if (KeyboardTracker.released.T)
	{
		SaveStage();
		// save �ߴٰ� ui�� �˷��ִ� ���� ���ٰ� �Ǵ�
	}

	if (input != 0)
	{
		// input�� ���������� updatestage�� �� �����μ� ���� ����
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
		// ĳ���� ��ġ
		auto player = mvStage[y][x];
		auto pplayer = &mvStage[y][x];

		// ���� ��ü 
		int tx = x + dx;
		int ty = y + dy;

		auto front = mvStage[ty][tx];
		auto pfront = &mvStage[ty][tx];

		// bit ������ �̿��� ��ü ó��
		switch (front)
		{
			case OBJ_WALL: { break; }
			// ���� ��� ���� �ձ��� üũ�Ͽ� �м� ���� ��� ��
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

	// stage data �� ��|�� üũ
	for (auto raw : mvStage)
	{
		for (auto element : raw)
		{
			if (static_cast<int>(element) == (OBJ_BALL | OBJ_GOAL)) { check++; }
		}
	}

	// �ʱ� �� ������ �������
	if (check == mBallNum) 
	{ 
		mEndCheck = true;
	}

}
// ����� �� ������ stage�� Ŭ���� ������ �ߴ� ui
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
// �� ȭ��
void MainGame::Render()
{
	XMFLOAT2 pos = { 0,0 };

	mspSpriteBatch->Begin();

	mspSpriteBatch->Draw(mpBackgroundTexture, XMFLOAT2(0, 0));

	// �� object ���� object�� ��ġ���� ���� �� �� �ش� ��ġ�� �׸��� �׷���
	// ��� object�� texture sizse �� 32,32 pixel(player height����)�� �������ױ� ������
	// +32�� ���� ���� ĭ�� ����
	// ��� ĭ ���� ������Ʈ�� �ο��Ͽ� objectmanager�� ����Ϸ� �Ͽ�����.
	// ������ ���� ��ȭ�� ĳ���͸� �ֱ� ������ ���� ��� ������Ʈ�� �޸𸮿� ��� ���� �ʿ䰡 ���ٰ� �Ǵ��Ͽ� ��������ʰ�
	// �⺻ Ʋ�� �޾ƿ� �ʿ��� ��ġ�� �̹����� ��� �ϰ� ����
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
