#include "stdafx.h"
#include "Scenes.h"
#include "ProjectObject/Scene/SceneGrid.h"
#include "ProjectObject/Scene/SceneOBJ.h"
#include "ProjectObject/Scene/SceneTerrain.h"
#include "ProjectObject/Scene/SceneTest.h"
#include "ProjectObject/Display/MainUI.h"

Scenes* Scenes::instance = NULL;
IScene* Scenes::m_pCurrentScene = NULL;
MainUI* mainUI = NULL;

Scenes::Scenes()
{
}

Scenes::~Scenes()
{
	for (auto p : m_pSceneList)
	{
		if (p) p->OnDestructIScene();
		SAFE_DELETE(p);
	}
	SAFE_DELETE(mainUI);
}

void Scenes::Init()	
{
	m_pSceneList.resize(SCENE_COUNT);
	
	//SetCurrentScene(SCENE_GRID);
	//SetCurrentScene(SCENE_TERRAIN);
	//SetCurrentScene(SCENE_TEST);
	mainUI = new MainUI; mainUI->Init();
}


void Scenes::Update()
{
	SAFE_UPDATE(m_pCurrentScene);
	if (m_pCurrentScene) m_pCurrentScene->OnUpdateIScene();
	SAFE_UPDATE(mainUI);
}

void Scenes::Render()
{
	SAFE_RENDER(m_pCurrentScene);
	if (m_pCurrentScene) m_pCurrentScene->OnRenderIScene();
	SAFE_RENDER(mainUI);
}


void Scenes::SetCurrentScene(int index)
{
	if (index >= m_pSceneList.size()) return;

	IScene* pScene = m_pSceneList[index];
	if (pScene == NULL)
	{
		if ( index == SCENE_GRID) pScene = new SceneGrid(index);
		if ( index == SCENE_TERRAIN) pScene = new SceneTerrain(index);
		if ( index == SCENE_OBJ) pScene = new SceneOBJ(index);
		if ( index == SCENE_TEST) pScene = new SceneTest(index);
	}
	m_pSceneList[index] = pScene;
	pScene->Init();
	m_pCurrentScene = pScene;
}

void Scenes::DeleteCurrentScene()
{
	if (m_pCurrentScene == NULL) return;

	m_pSceneList[m_pCurrentScene->GetIndex()] = NULL;
	m_pCurrentScene->OnDestructIScene();
	SAFE_DELETE(m_pCurrentScene);
}
