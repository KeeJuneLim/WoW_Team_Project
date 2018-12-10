#include "stdafx.h"
#include "SceneOBJ.h"
#include "Graphics/Model/Sky/SkyGradient.h"
#include "Graphics/Model/OBJ/ObjMap.h"
#include "ProjectObject/Display/ASEWoman.h"
#include "ProjectObject\Display\Player.h"


SceneOBJ::SceneOBJ(WORD index)
	:IScene(index)
{

}

SceneOBJ::~SceneOBJ()
{
	
}

void SceneOBJ::Init()
{
	IDisplayObject* pObj = NULL, *camTarget = NULL;
	pObj = new SkyGradient(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new ASEWoman(); pObj->Init(); AddDisplayObject(pObj);

	pObj = new Player();
	pObj->Init();
	AddDisplayObject(pObj);
	camTarget = pObj;
	Objects::AddToTagList(TAG_PLAYER, pObj);

	D3DXMATRIX matS, matRY, matT, localMatrix;
	D3DXMatrixScaling(&matS, 0.06f, 0.06f, 0.06f);
	D3DXMatrixRotationY(&matRY, D3DX_PI / 2.0f);
	D3DXMatrixTranslation(&matT, 0, -75, 0);
	localMatrix = matS * matRY * matT;

	ObjMap* map = new ObjMap(); AddDisplayObject(map);
	map->Load(ASSET_PATH + L"Models/OBJ/cs_italy/", L"cs_italy.obj", &localMatrix);
	map->Init();

	if (camTarget) Camera::Get()->SetTargetPosition(camTarget->GetPosition());
}

void SceneOBJ::Update()
{
	
}

void SceneOBJ::Render()
{
	
}
