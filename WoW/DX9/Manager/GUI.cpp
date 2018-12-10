#include "stdafx.h"
#include "GUI.h"

GUI* GUI::instance = NULL;
const char* GUI::Menu_WindowStr = "Menu_Window";
const char* GUI::LightStr = "Light";
const char* GUI::SystemStr = "System";
const char* GUI::CameraStr = "Camera";

GUI * GUI::Get()
{
	if (instance == NULL)
		instance = new GUI();

	return instance;
}

void GUI::Delete()
{
	SAFE_DELETE(instance);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

GUI::GUI()
{

}

GUI::~GUI()
{

}

void GUI::Init()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplWin32_Init(DX::GetInfo().hWnd);
	ImGui_ImplDX9_Init(DX::GetDevice());
	ImGui::StyleColorsDark();
	ImGui_ImplDX9_CreateDeviceObjects();
}

void GUI::Begin()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUI::End()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void GUI::UpdateUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(Menu_WindowStr))
		{
			if (ImGui::MenuItem("System", 0, &showSystem));
			if (ImGui::MenuItem("Camera", 0, &showCamera));
			if (ImGui::MenuItem("Light", 0, &showLight));
			
			
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

