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

	bool isTimerStopped;	//< Ÿ�̸� ����
	float timeElapsed;	//< ���� ���������κ��� ����ð�

	INT64 ticksPerSecond;	//< �ʴ� ƽī��Ʈ
	INT64 currentTime;	//< ���� �ð�
	INT64 lastTime;	//< �����ð�
	INT64 lastFPSUpdate;	//< ������ FPS ������Ʈ �ð�
	INT64 fpsUpdateInterval;	//< fps ������Ʈ ����

	UINT frameCount;	//< ������ ��
	float runningTime;	//< ���� �ð�
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