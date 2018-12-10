#include "stdafx.h"
#include "TerrainEditor.h"
#include "Terrain.h"

TerrainEditor::TerrainEditor()
{
	Init();
}

TerrainEditor::TerrainEditor(Terrain * pTerrain)
{
	this->pTerrain = pTerrain;
	Init();
}

void TerrainEditor::Init()
{
	OnOpenDataFunc = bind(&TerrainEditor::OnOpenData, this, placeholders::_1);
	OnSaveDataFunc = bind(&TerrainEditor::OnSaveData, this, placeholders::_1);
	OnOpenDiffuseMapFunc = bind(&TerrainEditor::OnOpenDiffuseMap, this, placeholders::_1);
	OnCreateBlendingMapFunc = bind(&TerrainEditor::OnCreateBlendingMap, this, placeholders::_1);
	OnOpenBlendingMapFunc = bind(&TerrainEditor::OnOpenBlendingMap, this, placeholders::_1);
	OnSaveBlendingMapFunc = bind(&TerrainEditor::OnSaveBlendingMap, this, placeholders::_1);
}

void TerrainEditor::OnSaveData(wstring filename)
{
	pTerrain->SaveData(filename);
}

void TerrainEditor::OnOpenData(wstring filename)
{
	pTerrain->OpenData(filename);
}

void TerrainEditor::OnOpenDiffuseMap(wstring filename)
{
	pTerrain->GetMaterial()->
		SetTexture(TT_DiffuseMap + currentDiffuseMapIndex, Assets::GetTexture((filename).c_str()));
}

void TerrainEditor::OnCreateBlendingMap(wstring filename)
{
	
	UINT texDim = blendingTexDim;
	LPDIRECT3DTEXTURE9 tex = NULL;
	
	D3DXCreateTexture(DX::GetDevice(), texDim, texDim, 1, D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex);
	
	D3DLOCKED_RECT lockedRect;
	tex->LockRect(0, &lockedRect, 0, 0);

	DWORD* data = (DWORD*)lockedRect.pBits;
	//ZeroMemory(data,texDim * texDim * sizeof(DWORD));
	DWORD pixelCount = lockedRect.Pitch / sizeof(DWORD) * texDim;
	for (UINT i = 0; i < pixelCount; i++)
	{
		*data = 0xff000000;
		data++;
	}

	tex->UnlockRect(0);

	D3DXSaveTextureToFile(
		(filename).c_str(),
		D3DXIMAGE_FILEFORMAT::D3DXIFF_PNG,
		tex, 0);

	SAFE_RELEASE(tex);

	OnOpenBlendingMap(filename);
}

void TerrainEditor::OnOpenBlendingMap(wstring filename)
{
	D3DXIMAGE_INFO imgInfo;
	blendingTex = Assets::GetTexture(filename.c_str(), &imgInfo);
	blendingTexDim = imgInfo.Width;

	Objects::GetCurrentMap()->GetMaterial()->SetTexture(TT_AlphaMap, blendingTex);
}

void TerrainEditor::OnSaveBlendingMap(wstring filename)
{
	D3DXSaveTextureToFile(
		(filename).c_str(),
		D3DXIMAGE_FILEFORMAT::D3DXIFF_PNG,
		blendingTex, 0);
}


void TerrainEditor::UpdateUI()
{
	ImGuiWindowFlags flag = 0;
	flag |= ImGuiWindowFlags_NoResize;
	flag |= ImGuiWindowFlags_NoMove;
	ImGui::SetNextWindowSize(ImVec2(250, DX::GetInfo().screenHeight - 20));
	ImGui::SetNextWindowPos(ImVec2(DX::GetInfo().screenWidth - 250, 20));
	
	int menuBtnWidth = 40;
	int menuBtnHeight = 25;
	if (ImGui::Begin("TerrainEditor"), flag)
	{
		if (ImGui::Button("Data", ImVec2(menuBtnWidth, menuBtnHeight)))
		{
			menuIndex = EditorMenu::Data;
			type = BT_None;
			shape = BS_None;
		}
		ImGui::SameLine();
		if (ImGui::Button("Edit", ImVec2(menuBtnWidth, menuBtnHeight)))
		{
			menuIndex = EditorMenu::Edit;
			type = BT_Sum;
			shape = BS_Rect;
		}
		ImGui::Separator();
		ImGui::Text("Dimension : %d", pTerrain->dimension);
		ImGui::Separator();
		if (menuIndex == EditorMenu::Data)
		{
			ImGui::Text("indexCount = %d", pTerrain->indexCount);
			ImGui::SliderFloat("lodRatio", &pTerrain->lodRatio, 0.01f, 0.4f);

			if (ImGui::Button("New", ImVec2(50, 30)))
			{
				ImGui::OpenPopup("NewTerrain");
			}
			ImGui::SameLine();
			if (ImGui::Button("Save", ImVec2(50, 30)))
			{
				Path::SaveFileDialog(L"", NULL, ASSET_PATH, OnSaveDataFunc, DX::GetInfo().hWnd);
			}
			ImGui::SameLine();
			if (ImGui::Button("Open", ImVec2(50, 30)))
			{
				Path::OpenFileDialog(L"", NULL, ASSET_PATH, OnOpenDataFunc, DX::GetInfo().hWnd);
			}

			if (ImGui::BeginPopup("NewTerrain"))
			{
				ImGui::DragInt("Dimension", (int*)&terrainDim);

				if (ImGui::Button("Create", ImVec2(50, 30)))
				{
					ImGui::CloseCurrentPopup();

					pTerrain->UpdateDimension(terrainDim);
				}

				ImGui::EndPopup();
			}
		}
		else if (menuIndex == EditorMenu::Edit)
		{
			ImGui::Text("Type : %s", typeStr[type].c_str());
			ImGui::Text("Shape : %s", shapeStr[shape].c_str());
			ImGui::Text("Center : %.2f %.2f %.2f", center.x, center.y, center.z);
			ImGui::Separator();
			ImGui::SliderInt("Type", (int*)&type, 0, 4);
			ImGui::SliderInt("Shape", (int*)&shape, 0, 2);
			ImGui::SliderFloat("Range", (float*)&range, 0.1f, 10.0f);
			ImGui::SliderFloat("Intensity", (float*)&intensity, 1.0f, 25.0f);
			if (type == BT_Converge || type == BT_Mean)
				ImGui::Checkbox("Fit", &fit);
			if (type == BT_Converge)
				ImGui::SliderFloat("Converge", (float*)&converge, 0.0f, 30.0f);
			if (type == BT_Blending)
			{
				ImGui::BulletText("DiffuseMap");
				ImGui::SliderInt("Current Index", &currentDiffuseMapIndex, 0, 4);
				if (ImGui::Button("OpenD"))
				{
					Path::OpenFileDialog(L"", Path::ImageFilter, ASSET_PATH, OnOpenDiffuseMapFunc, DX::GetInfo().hWnd);
				}

				ImGui::BulletText("BlendingMap");

				if (ImGui::Button("Create"))
				{
					ImGui::OpenPopup("createBlendingTex");
				}
				if (ImGui::BeginPopup("createBlendingTex"))
				{
					ImGui::SliderInt("size", (int*)&blendingTexDim, 32, 257);

					if (ImGui::Button("Create"))
					{
						ImGui::CloseCurrentPopup();
						Path::SaveFileDialog(L"", Path::ImageFilter, ASSET_PATH, OnCreateBlendingMapFunc, DX::GetInfo().hWnd);
					}

					ImGui::EndPopup();
				}


				ImGui::SameLine();
				if (ImGui::Button("OpenB"))
				{
					Path::OpenFileDialog(L"", Path::ImageFilter, ASSET_PATH, OnOpenBlendingMapFunc, DX::GetInfo().hWnd);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					Path::OpenFileDialog(L"", Path::ImageFilter, ASSET_PATH, OnSaveBlendingMapFunc, DX::GetInfo().hWnd);
				}
				ImGui::SliderInt("Current Channel", &currentBlendingChannel, 0, 3);
				
				if (blendingTex) ImGui::Image(blendingTex, ImVec2(75, 75));
			}
		}
		

		ImGui::End();
	}
	
}

void TerrainEditor::ComputeEditedIndices(BYTE* vertices, UINT stride, vector<UINT> &editedIndices, UINT dimension)
{
	editedIndices.clear();

	RECT rect;
	rect.left = center.x - range;
	rect.top = center.z + range;
	rect.right = center.x + range;
	rect.bottom = center.z - range;

	if (rect.left < 0)rect.left = 0;
	if (rect.top >= dimension - 1)rect.top = dimension - 1;
	if (rect.right >= dimension - 1)rect.right = dimension - 1;
	if (rect.bottom < 0)rect.bottom = 0;

	if (shape == BS_Rect)
	{
		for (UINT z = rect.bottom; z <= rect.top; z++)
		{
			for (UINT x = rect.left; x <= rect.right; x++)
			{
				UINT index = (dimension)* z + x;
				editedIndices.push_back(index);
			}
		}
	}
	else if (shape == BS_Circle)
	{
		for (UINT z = rect.bottom; z <= rect.top; z++)
		{
			for (UINT x = rect.left; x <= rect.right; x++)
			{
				UINT index = (dimension)* z + x;
				D3DXVECTOR3* pos = (D3DXVECTOR3*)(vertices + index * stride);
				D3DXVECTOR2 targetPos(pos->x, pos->z);
				D3DXVECTOR2 centerPos(center.x, center.z);

				if (D3DXVec2LengthSq(&(targetPos - centerPos)) <
					range * range)
				{
					editedIndices.push_back(index);
				}
			}
		}
	}
}

void TerrainEditor::EditHeight(BYTE* vertices, UINT stride, vector<UINT> &editedIndices, UINT dimension)
{
	ComputeEditedIndices(vertices, stride, editedIndices, dimension);

	float editValue = intensity;
	if (Input::KeyPress(VK_SHIFT))
		editValue = -editValue;

	if (type == TerrainEditor::BT_Sum)
	{
		for (UINT i = 0; i < editedIndices.size(); i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(vertices + editedIndices[i] * stride);

			D3DXVECTOR3 _center = D3DXVECTOR3(center.x, 0, center.z);
			D3DXVECTOR3 _curr = D3DXVECTOR3(pos->x, 0, pos->z);
			float dist = D3DXVec3Length(&(_center - _curr));

			pos->y += editValue / (3 + dist * 0.3f) * Time::Get()->GetDeltaTime();
		}
	}
	else if (type == TerrainEditor::BT_Mean)
	{
		int count = 0;
		float meanHeight = 0;
		for (UINT i = 0; i < editedIndices.size(); i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(vertices + editedIndices[i] * stride);
			meanHeight += pos->y;
			count++;
		}

		meanHeight /= count;

		for (UINT i = 0; i < editedIndices.size(); i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(vertices + editedIndices[i] * stride);
			
			if (fit)
			{
				pos->y = meanHeight;
			}
			else
			{
				if (pos->y > meanHeight)
				{
					pos->y -= editValue * Time::Get()->GetDeltaTime();
				}
				else if (pos->y < meanHeight)
				{
					pos->y += editValue * Time::Get()->GetDeltaTime();
				}
			}
			
		}
	}
	else if (type == TerrainEditor::BT_Converge)
	{
		for (UINT i = 0; i < editedIndices.size(); i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(vertices + editedIndices[i] * stride);

			if (fit)
			{
				pos->y = converge;
			}
			else
			{
				if (pos->y > converge)
				{
					pos->y -= editValue * Time::Get()->GetDeltaTime();
				}
				else if (pos->y < converge)
				{
					pos->y += editValue * Time::Get()->GetDeltaTime();
				}
			}
		}
	}

}

void TerrainEditor::EditBlendingTex(UINT dimension)
{
	vector<UINT> editedIndices;

	float mappingScale = blendingTexDim / (float)dimension;
	D3DXVECTOR3 center = this->center * mappingScale;
	float range = this->range * mappingScale;

	RECT rect;
	rect.left = center.x - range;
	rect.right = center.x + range;
	rect.bottom = center.z - range;
	rect.top = center.z + range;

	if (rect.left < 0) rect.left = 0;
	if (rect.right >= blendingTexDim - 1) rect.right = blendingTexDim - 1;
	if (rect.bottom < 0) rect.bottom = 0;
	if (rect.top >= blendingTexDim - 1) rect.top = blendingTexDim - 1;

	if (shape == BS_Rect)
	{
		for (UINT z = rect.bottom; z <= rect.top; z++)
		{
			for (UINT x = rect.left; x <= rect.right; x++)
			{
				UINT index = z * (blendingTexDim) + x;
				editedIndices.push_back(index);
			}
		}
	}
	else if (shape == BS_Circle)
	{
		for (UINT z = rect.bottom; z <= rect.top; z++)
		{
			for (UINT x = rect.left; x <= rect.right; x++)
			{
				UINT index = z * (blendingTexDim) + x;
				D3DXVECTOR2 targetPos(x, z);
				D3DXVECTOR2 centerPos(center.x, center.z);

				if (D3DXVec2LengthSq(&(targetPos - centerPos)) <
					range * range)
				{
					editedIndices.push_back(index);
				}
			}
		}
	}

	float editValue = intensity;
	if (Input::KeyPress(VK_SHIFT))
		editValue = -editValue;

	D3DLOCKED_RECT lockedRect;
	blendingTex->LockRect(0, &lockedRect, 0, 0);

	DWORD* data = (DWORD*)lockedRect.pBits;

	for (UINT i = 0; i < editedIndices.size(); i++)
	{
		UINT index = editedIndices[i];

		D3DXCOLOR color = data[index];

		if (currentBlendingChannel == 0)
			color.r += editValue * Time::Get()->GetDeltaTime();
		else if (currentBlendingChannel == 1)
			color.g += editValue * Time::Get()->GetDeltaTime();
		else if (currentBlendingChannel == 2)
			color.b += editValue * Time::Get()->GetDeltaTime();
		else if (currentBlendingChannel == 3)
			color.a += editValue * Time::Get()->GetDeltaTime();

		data[index] = color;
	}


	blendingTex->UnlockRect(0);
}

