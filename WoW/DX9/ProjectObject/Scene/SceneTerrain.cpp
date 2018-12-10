#include "stdafx.h"
#include "SceneTerrain.h"
#include "Graphics/Model/Sky/Cloud.h"
#include "Graphics/Model/Sky/SkyGradient.h"
#include "Graphics/Model/Terrain/Terrain.h"
#include "ProjectObject/Display/ASEWoman.h"
#include "ProjectObject/Display/Test/InstancingTest.h"
#include "ProjectObject\Display\Player.h"
#include "ProjectObject\Display\ShaderCube.h"


SceneTerrain::SceneTerrain(WORD index)
	:IScene(index)
{

}

SceneTerrain::~SceneTerrain()
{
	
}

void SceneTerrain::Init()
{
	IDisplayObject* pObj = NULL, *camTarget = NULL;
	pObj = new SkyGradient(); pObj->Init(); AddDisplayObject(pObj);
	pObj = new Cloud(); pObj->Init(); AddDisplayObject(pObj);
	//pObj = new ASEWoman(); pObj->Init(); AddDisplayObject(pObj);

	IDisplayObject* player = new Player(); player->Init(); AddDisplayObject(player);
	Objects::AddToTagList(TAG_PLAYER, player);
	camTarget = player;

	//pObj = new ShaderCube(D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1)); pObj->Init(); AddDisplayObject(pObj);

	Terrain* map = new Terrain(); AddDisplayObject(map);
	wstring filepath = ASSET_PATH + L"/textures/heightmap/grass.dds";
	map->GetMaterial()->SetTexture(TT_DiffuseMap, Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"/textures/heightmap/slope.dds";
	map->GetMaterial()->SetTexture(TT_DiffuseMap1, Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"/textures/heightmap/rock.dds";
	map->GetMaterial()->SetTexture(TT_DiffuseMap2, Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"/textures/girl.png";
	map->GetMaterial()->SetTexture(TT_DiffuseMap3, Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"/textures/ham.jpg";
	map->GetMaterial()->SetTexture(TT_DiffuseMap4, Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"/textures/heightmap/blend.png";
	map->GetMaterial()->SetTexture(TT_AlphaMap, Assets::GetTexture(filepath.c_str()));
	//map->SetHeightFactor(0.1f);
	//map->Load(ASSET_PATH + L"textures/heightmap/heightmap.raw");
	//map->LoadData(ASSET_PATH + L"Models/Map/00.terrain");
	map->UpdateDimension(65);
	map->Init();

	pObj = new InstancingTest(); pObj->Init(); AddDisplayObject(pObj);

	if (camTarget) Camera::Get()->SetTargetPosition(camTarget->GetPosition());

	
}

void SceneTerrain::Update()
{
	
}

void SceneTerrain::Render()
{
	
}
