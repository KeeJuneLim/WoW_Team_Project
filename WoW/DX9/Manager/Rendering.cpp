#include "stdafx.h"
#include "Rendering.h"
#include "Graphics/Interface/IDisplayObject.h"
#include "Graphics/Shader/IShader.h"

Rendering* Rendering::instance = NULL;

Rendering * Rendering::Get()
{
	if (instance == NULL)
		instance = new Rendering();

	return instance;
}

void Rendering::Delete()
{
	SAFE_DELETE(instance);
}

Rendering::Rendering()
{
	
}

Rendering::~Rendering()
{
	
}

void Rendering::Init()
{
	renderingList.resize(ShaderType::ST_COUNT);
}

void Rendering::Render()
{
	
	for (int i = 0; i < renderingList.size(); i++)
	{
		IShader* shader = Shaders::Get()->GetShader((ShaderType)i);

		if (shader) shader->PreRender();

		if (i == ST_ShadowMap)
		{
			for (auto obj : shadowCastList)
				obj->Render(shader);

			//ImGui::Image(ShaderData::shadow.shadowMap, ImVec2(100, 100));
		}
		else
		{
			for (auto obj : renderingList[i])
			{
				//Render() 는 임시용. Render(shader) 만 쓰도록 바꾸기.
				obj->Render();
				obj->Render(shader);
			}
				
		}

		if (shader) shader->PostRender();
		
	}
	
}


void Rendering::AddRenderingObject(IDisplayObject* pObj)
{
	if (pObj == NULL) return;
	
	ShaderType type = pObj->GetShaderType();
	renderingList[type].push_back(pObj);
}

void Rendering::RemoveRenderingObject(IDisplayObject* pObj)
{
	if (pObj == NULL) return;
	
	ShaderType type = pObj->GetShaderType();
	renderingList[type].remove(pObj);
}
