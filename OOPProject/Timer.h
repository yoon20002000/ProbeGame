#pragma once
class Timer
{
private:
	// ���� �ϵ������ ���ļ��� ��������� ���� ���� ����
	double		mdSecondsPerCount;
	// ���� �ð��� ���� �ð��� ���� ������ ����
	double		mdDeltaTime;
	// �ð� �帧 ������ ���� ����
	float		mfScale;

	//�� �� ������ �ð��� ����
	long long	mllBaseTime;
	long long	mllPausedTime;
	long long	mllStopTime;
	long long	mllPrevTime;
	long long	mllCurrTime;

	// �ð� �帧 ���� üũ ����
	bool		mbStopped;

public:
	// �� �ð� üũ
	float	TotalTime() const;
	// �� delta time return
	float	DeltaTime() const;
	// �ð� ����
	void	Start();
	// �ð� �����
	void	Resumte();
	// �ð� ����
	void	Stop();
	// �ð� �帧
	void	Update();
	// �ð� �帧�ӵ�
	void	SetScale(float scale) { mfScale = scale; }


public:
	Timer();
	~Timer();
};

