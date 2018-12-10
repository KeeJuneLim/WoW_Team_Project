#include "stdafx.h"
#include "Time.h"

Time* Time::instance = NULL;

Time* Time::Get()
{
	if (instance == NULL)
		instance = new Time();

	return instance;
}

void Time::Init()
{
	Start();
}

void Time::Delete()
{
	SAFE_DELETE(instance);
}

Time::Time()
	: isTimerStopped(true), timeElapsed(0), frameCount(0), runningTime(0), framePerSecond(0),
	ticksPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0), fpsUpdateInterval(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;
}

Time::~Time()
{

}

void Time::Update()
{
	if (isTimerStopped) return;

	//1. 현재시간을 가져와 시간 간격 및 진행 시간을 계산한다.
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;


	//2. FPS Update
	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentTime / (float)ticksPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)ticksPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void Time::Start()
{
	assert(isTimerStopped);

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

void Time::Stop()
{
	assert(isTimerStopped == false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);
	runningTime += (float)(stopTime - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
	timeElapsed = 0;
}