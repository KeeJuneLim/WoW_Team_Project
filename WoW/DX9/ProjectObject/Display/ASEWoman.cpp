#include "stdafx.h"
#include "ASEWoman.h"
#include "Graphics/Model/ASE/AseModel.h"

ASEWoman::ASEWoman()
{
	hiding = new Hiding;

	name = L"ASEWoman";
	tag = TAG_PLAYER;
	Objects::AddToTagList(tag, this);

	isMoving = false;
	baseRotY = D3DX_PI;

	scale = D3DXVECTOR3(3, 3, 3);
}

ASEWoman::~ASEWoman()
{
	Rendering::Get()->RemoveRenderingObject(this);
	Rendering::Get()->RemoveShadowCastObject(this);
	Objects::RemoveFromTagList(tag, this);
	SAFE_DELETE(material);
}

void ASEWoman::Init()
{
	
	wstring path = ASSET_PATH + _T("models/ase/woman/");
	wstring name = _T("woman_stand.ASE");
	standModel = new AseModel();
	standModel->Load(path.c_str(), name.c_str());
	standModel->Init();
	AddChild(standModel);

	name = _T("woman.ASE");
	runModel = new AseModel();
	runModel->Load(path.c_str(), name.c_str());
	runModel->Init();
	AddChild(runModel);

	currentModel = standModel;

	SetRootWithChildren(this);

	SetShaderTypeWithChildren(ST_Shadow);
	Rendering::Get()->AddRenderingObject(this);
	Rendering::Get()->AddShadowCastObject(this);
	
}

void ASEWoman::Update()
{
	ImGui::Begin("Shader");
	if (ImGui::Button("Lighting"))
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SetShaderTypeWithChildren(ShaderType::ST_Lighting);
		Rendering::Get()->AddRenderingObject(this);
		ImGui::SameLine(100, 10);
	}
	else if (ImGui::Button("Toon"))
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SetShaderTypeWithChildren(ShaderType::ST_Toon);
		Rendering::Get()->AddRenderingObject(this);
	}
	else if (ImGui::Button("Hiding"))
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SetShaderTypeWithChildren(ShaderType::ST_Hiding);
		Rendering::Get()->AddRenderingObject(this);

		hiding->Activate(!hiding->activatedHiding);
	}
	ImGui::End();

	if (hiding->onChange)
	{
		hiding->UpdateHidingTime(Time::Get()->GetDeltaTime());

		if (hiding->onChange == false)
		{
			if (hiding->activatedHiding == false)
			{
				Rendering::Get()->RemoveRenderingObject(this);
				SetShaderTypeWithChildren(ShaderType::ST_Lighting);
				Rendering::Get()->AddRenderingObject(this);
			}
		}
	}

	IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	IDisplayObject::ApplyVelocity();
	
	if (Objects::GetCurrentMap())
	{
		if (velocity.y <= 0)
		{
			float height;
			if (Objects::GetCurrentMap()->ComputeHeight(height, targetPosition))
			{
				if (targetPosition.y <= height)
				{
					targetPosition.y = height;
					velocity.y = 0;
				}
				position = targetPosition;
			}
		}
		else
		{
			position = targetPosition;
		}
	}
	else
	{
		position = targetPosition;
	}

	IDisplayObject::UpdateLocalMatrix();
	
	D3DXMATRIX baseRotYMat;
	D3DXMatrixRotationY(&baseRotYMat, baseRotY);
	localMatrix = baseRotYMat * localMatrix;

	IDisplayObject::UpdateWorldMatrix();
	
	if (D3DXVec3Length(&deltaTransform) > 0)
		isMoving = true;
	else
		isMoving = false;

	if (isMoving == true)
	{
		if (currentModel != runModel)
			currentModel = runModel;
	}
	else if (isMoving == false)
	{
		if (currentModel != standModel)
			currentModel = standModel;
	}

	currentModel->Update();
	IDisplayObject::UpdateUI();

	ImGui::Begin("Shader");
	ImGui::SliderFloat("bumpness", &hiding->bumpness, 0.01, 0.5f);
	ImGui::End();

	ShaderData::hiding.onChange = hiding->onChange;
	ShaderData::hiding.hidingFactor = hiding->hidingFactor;
	ShaderData::hiding.hidingBumpness = hiding->bumpness;
}

void ASEWoman::Render()
{
	
}

void ASEWoman::Render(IShader * pShader)
{
	currentModel->Render(pShader);
}

void ASEWoman::Draw()
{
}
