
#include "stdafx.h"
#include "DX9.h"
#include "Application.h"
#include <time.h>
//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console") 


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((int)time(NULL));

	D3DInfo info;
	info.appName = _T("DirectX Game");
	info.hInstance = hInstance;
	info.isWindowMode = true;
	info.hWnd = NULL;

	if (info.isWindowMode)
	{
		info.screenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.7f;
		info.screenHeight = info.screenWidth * 9 / 16.0f;
	}
	else
	{
		info.screenWidth = GetSystemMetrics(SM_CXSCREEN);
		info.screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	info.useMultySample = false;

	DX::SetInfo(info);

	Application* app = new Application;
	app->Run();

	return 0;
}

