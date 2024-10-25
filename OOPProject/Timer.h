#pragma once
class Timer
{
private:
	// 현재 하드웨어의 주파수를 기반으로한 단위 저장 변수
	double		mdSecondsPerCount;
	// 이전 시간과 현재 시간의 차를 저장할 변수
	double		mdDeltaTime;
	// 시간 흐름 조절을 위한 변수
	float		mfScale;

	//각 각 상태의 시간을 저장
	long long	mllBaseTime;
	long long	mllPausedTime;
	long long	mllStopTime;
	long long	mllPrevTime;
	long long	mllCurrTime;

	// 시간 흐름 정지 체크 변수
	bool		mbStopped;

public:
	// 총 시간 체크
	float	TotalTime() const;
	// 실 delta time return
	float	DeltaTime() const;
	// 시간 시작
	void	Start();
	// 시간 재시작
	void	Resumte();
	// 시간 정지
	void	Stop();
	// 시간 흐름
	void	Update();
	// 시간 흐름속도
	void	SetScale(float scale) { mfScale = scale; }


public:
	Timer();
	~Timer();
};

