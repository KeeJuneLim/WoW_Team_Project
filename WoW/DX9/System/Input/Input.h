#pragma once

#define MAX_INPUT_MOUSE 8
#define MAX_INPUT_KEY 255

class Input
{
public:
	static Input*	Get();
	static void		Delete();
private:
	static Input*	instance;
	Input();
	~Input();

public:
	void Init(HWND handle);
	void Update();

	//Mouse
private:
	HWND hWnd;
	
	static byte buttonMap[MAX_INPUT_MOUSE];
	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];

	D3DXVECTOR3 currPosition;
	D3DXVECTOR3 oldPosition;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	enum INPUT_STATUS
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

	static D3DXVECTOR3 position;
	static D3DXVECTOR3 deltaPosition;

public:
	
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	static bool ButtonDown(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;}
	static bool ButtonUp(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP;}
	static bool ButtonPress(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;}
	static bool ButtonDB(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK;}

	static const D3DXVECTOR3& GetMousePosition() { return position; }
	static const D3DXVECTOR3& GetMouseDeltaPosition() { return deltaPosition; }

	enum INPUT_TYPE
	{
		LBUTTON, RBUTTON, MBUTTON
	};

private:
	void UpdateMouseState();
	
	
	//Key
private:
	static byte keyMap[MAX_INPUT_KEY];
	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

public:
	static bool KeyDown(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	static bool KeyUp(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	static bool KeyPress(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

private:
	void UpdateKeyState();

};

