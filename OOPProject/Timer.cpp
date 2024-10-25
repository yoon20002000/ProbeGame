#include "stdafx.h"


float Timer::TotalTime() const
{
	if (mbStopped)
	{
		return static_cast<float>(((mllStopTime - mllPausedTime) - mllBaseTime) * mdSecondsPerCount);
	}
	else
	{
		return static_cast<float>(((mllCurrTime - mllPausedTime) - mllBaseTime)*mdSecondsPerCount);
	}
}

float Timer::DeltaTime() const
{
	return static_cast<float>(mdDeltaTime * mfScale);
}

void Timer::Start()
{
	long long currTime;
	//  ���� �ϵ������ �ð�
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mllBaseTime = currTime;
	mllPrevTime = currTime;
	mllStopTime = 0;
	mbStopped = false;

}

void Timer::Resumte()
{
	long long startTime;
	//  ���� �ϵ������ �ð�
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mbStopped)
	{
		mllPausedTime += startTime - mllStopTime;

		mllStopTime = mllCurrTime;
		mllStopTime = 0;
		mbStopped = false;

	}
	
}

void Timer::Stop()
{
	if (!mbStopped)
	{
		long long currTime;
		//  ���� �ϵ������ �ð�
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllStopTime = currTime;
		mbStopped = true;
	}
}

void Timer::Update()
{
	if (mbStopped)
	{
		mdDeltaTime = 0;
		return ;
	}
	else 
	{

		__int64 currTime;
		//  ���� �ϵ������ �ð�
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllCurrTime = currTime;
		mdDeltaTime = (mllCurrTime - mllPrevTime) * mdSecondsPerCount;
		mllPrevTime = mllCurrTime;

		if (mdDeltaTime < 0.0)
		{
			mdDeltaTime = 0.0;
		}

	}

}


Timer::Timer() : mdSecondsPerCount(0.0f),
				mdDeltaTime(-1.0f),
				mfScale(1.0f),
				mllBaseTime(0),
				mllPausedTime(0),
				mllStopTime(0),
				mllPrevTime(0),
				mllCurrTime(0)
{

	long long countsPerSec;
	// ���� �ϵ������ ����Ŭ(���ļ�, �ʴ�ݺ�Ƚ��)
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mdSecondsPerCount = 1.0 / (double)countsPerSec;
}


Timer::~Timer()
{
}
