#include "stdafx.h"
#include "Application.h"

Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	DX::Get()->Init();
	Time::Get()->Init();
	Input::Get()->Init(DX::GetInfo().hWnd);
	Camera::Get()->Init(DX::GetInfo().hWnd);
	Frustum::Get()->Init();
	Shaders::Get()->Init();
	Rendering::Get()->Init();
	Scenes::Get()->Init();
	GUI::Get()->Init();

}

void Application::Destroy()
{
	GUI::Delete();
	Frustum::Delete();
	Camera::Delete();
	Input::Delete();
	Time::Delete();
	Scenes::Delete();
	Rendering::Delete();
	Shaders::Delete();
	//Debug::Delete();
	Objects::Delete();
	Assets::Delete();
	DX::Delete();
}

void Application::Update()
{
	GUI::Get()->Begin();	
	GUI::Get()->UpdateUI();
	
	//Debug::Get()->InitText();
	Time::Get()->Update();
	if (ImGui::IsAnyItemHovered() == false)
		Input::Get()->Update();
	Light::Update();
	Scenes::Get()->Update();
	Camera::Get()->Update();
	Frustum::Get()->Update();

	if (GUI::Get()->showSystem)
	{
		ImGui::Begin(GUI::SystemStr);
		ImGui::Text("FPS : %.0f", Time::Get()->GetFPS());
		ImGui::End();
	}
	
}

void Application::Render()
{
	DX::GetDevice()->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(55, 55, 55), 1.0f, 0);
	
	DX::GetDevice()->BeginScene();
	
	Rendering::Get()->Render();
	Scenes::Get()->Render();
	//Frustum::Get()->Render();
	//Debug::Get()->Print();

	GUI::Get()->End();

	DX::GetDevice()->EndScene();
	DX::GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GUI::Get()->WndProc(hWnd, message, wParam, lParam);
	
	Input::Get()->InputProc(message, wParam, lParam);
}

