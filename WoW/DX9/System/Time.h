#pragma once

class Time
{
public:
	static Time*	Get();
	static void		Delete();
private:
	static Time*	instance;
	Time();
	~Time();

	bool isTimerStopped;	//< 타이머 중지
	float timeElapsed;	//< 이전 프레임으로부터 경과시간

	INT64 ticksPerSecond;	//< 초당 틱카운트
	INT64 currentTime;	//< 현재 시간
	INT64 lastTime;	//< 이전시간
	INT64 lastFPSUpdate;	//< 마지막 FPS 업데이트 시간
	INT64 fpsUpdateInterval;	//< fps 업데이트 간격

	UINT frameCount;	//< 프레임 수
	float runningTime;	//< 진행 시간
	float framePerSecond;	//< FPS

public:
	
	void Init();
	void Update();

	void Start();
	void Stop();

	float GetFPS() const { return framePerSecond; }
	float GetDeltaTime() { return timeElapsed; }
	float GetRunningTime() const { return runningTime; }
	bool IsTimerStopped() { return isTimerStopped; }
	


};