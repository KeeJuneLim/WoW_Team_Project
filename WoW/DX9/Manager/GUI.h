#pragma once

//ImGui 라이브러리를 관리

class GUI
{
public:
	static GUI*	Get();
	static void	Delete();
private:
	static GUI*	instance;
	GUI();
	~GUI();

public:
	bool showLight = true;
	bool showSystem = true;
	bool showCamera = true;

	void Init();
	void Begin();
	void End();
	HRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void UpdateUI();

	static const char* Menu_WindowStr;
	static const char* LightStr;
	static const char* SystemStr;
	static const char* CameraStr;
};

