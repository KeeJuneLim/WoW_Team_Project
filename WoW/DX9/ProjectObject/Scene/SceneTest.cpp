#include "stdafx.h"
#include "SceneTest.h"
#include "Graphics/Model/Sky/SkyGradient.h"
#include "Graphics/Model/Sky/Sky.h"
#include "Graphics/Model/OBJ/ObjMap.h"
#include "ProjectObject/Display/ShaderCube.h"
#include "ProjectObject/Display/Particle/SphereParticle.h"
#include "ProjectObject/Display/Test/MipmapFilter.h"
#include "ProjectObject/Display/Test/ExtractFromBuffer.h"
#include "ProjectObject/Display/Test/Viewport.h"
#include "ProjectObject/Display/Test/EnvMapping.h"
#include "ProjectObject/Display/Test/NormalMapping.h"
#include "ProjectObject/Display/Test/InstancingTest.h"
#include "ProjectObject/Display/Test/ShadowMapping.h"
#include "ProjectObject\Display\Player.h"


SceneTest::SceneTest(WORD index)
	:IScene(index)
{

}

SceneTest::~SceneTest()
{
	
}

void SceneTest::Init()
{
	IDisplayObject* pObj = NULL, *camTarget = NULL;
	//pObj = new ShaderCube(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new SkyGradient(); pObj->Init(); AddDisplayObject(pObj);
	/*
	pObj = new Sky(); pObj->Init(); AddDisplayObject(pObj);
	wstring filepath = ASSET_PATH + L"textures/sky/space.png";
	pObj->GetMaterial()->SetTexture(TT_DiffuseMap, Assets::GetTexture(filepath.c_str()));
	*/
	//pObj = new MipmapFilter(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new ExtrackFromBuffer(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new SphereParticle(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new Viewport(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new EnvMapping(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new NormalMapping(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new InstancingTest(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new ShadowMapping(); pObj->Init(); AddDisplayObject(pObj);
	//camTarget = pObj;
	/**/
	//푸쉬 테스트용
	float testf = 0.0f;

	float scale = 1.2f;
	float offset = 2.5f;


	D3DXMATRIX matS1, matR1, matT1, localMat1;
	D3DXMatrixScaling(&matS1, 0.12f, 0.12f, 0.12f);
	D3DXMatrixRotationY(&matR1, D3DX_PI * 1.0f);
	D3DXMatrixTranslation(&matT1, 255, 5, 125);
	localMat1 = matS1 * matR1 * matT1;

	ObjMap* map = new ObjMap();
	map->Load(ASSET_PATH + L"Models/MAP/", L"testmap1.obj", &localMat1);
	map->Init();
	AddDisplayObject(map);


	pObj = new Player();
	pObj->Init();
	AddDisplayObject(pObj);
	Objects::AddToTagList(TAG_PLAYER, pObj);
	camTarget = pObj;





	////D3DXMATRIX matS2, matR2, matT2, localMat2;
	//D3DXMatrixScaling(&matS1, 0.12f, 0.12f, 0.12f);
	//D3DXMatrixRotationY(&matR1, D3DX_PI);
	//D3DXMatrixTranslation(&matT1, 255, 5, 125);
	//localMat1 = matS1 * matR1 * matT1;

	//ObjMap* map2 = new ObjMap();
	//map2->Load(ASSET_PATH + L"Models/MAP/", L"testmap2.obj", &localMat1);
	//map2->Init();
	//AddDisplayObject(map2);



	
	
	if (camTarget) Camera::Get()->SetTargetPosition(camTarget->GetPosition());
	playerUI = new PlayerUI; playerUI->Init();
}

void SceneTest::Update()
{
	SAFE_UPDATE(playerUI);
}

void SceneTest::Render()
{
	SAFE_RENDER(playerUI);
}
