#include "stdafx.h"
#include "SceneTest.h"
#include "Graphics/Model/Sky/SkyGradient.h"
#include "Graphics/Model/Sky/Sky.h"
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
	float scale = 1.2f;
	float offset = 2.5f;


	pObj = new Player();
	pObj->Init();
	AddDisplayObject(pObj);

	//player->Init();
	//pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"Idle.X");
	//pObj->Init(); AddDisplayObject(pObj);


	for (int i = 0; i < 1; i++)
	{
		//float radian = D3DX_PI * 2 * i / (10.0f + (int)(i / 10));
		//float radius = 3.0f + offset * (int)(i / 10);
		//pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"zealot.X");
		//pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"Attack.X");
	//	pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"Idle.X");
	//	pObj->Init(); AddDisplayObject(pObj);

	//	pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"Face.X");
	//	pObj->Init(); AddDisplayObject(pObj);

	//	pObj->SetPosition(&D3DXVECTOR3(0,25,0));
	//	pObj->SetRotation(&D3DXVECTOR3( D3DX_PI , -D3DX_PI / 2, -D3DX_PI / 2));
	////	pObj->SetScale(&D3DXVECTOR3(scale, scale, scale));
	//	//pObj->SetPosition(&D3DXVECTOR3(cosf(radian) * radius, 0, sinf(radian) * radius));
	//	//pObj->SetBaseRotY(D3DX_PI);

	//	LPD3DXFRAME rootFrame = dynamic_cast<SkinnedMeshHLSL*>(pObj)->m_pRootFrame;
	//	LPD3DXFRAME bone = dynamic_cast<SkinnedMeshHLSL*>(pObj)->FindBone(rootFrame);
	//	bone->CombinedTM;

		//pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"Dance.X");



		//pObj->Init(); AddDisplayObject(pObj);
		//pObj->SetScale(&D3DXVECTOR3(scale, scale, scale));
		////pObj->SetPosition(&D3DXVECTOR3(cosf(radian) * radius, 0, sinf(radian) * radius));
		//pObj->SetBaseRotY(D3DX_PI);

		//pObj = new SkinnedMeshHLSL((ASSET_PATH + L"Models/x/zealot/").c_str(), L"head.X");

	}


	
	
	//if (camTarget) Camera::Get()->SetTargetPosition(camTarget->GetPosition());

}

void SceneTest::Update()
{
}

void SceneTest::Render()
{
	
}
