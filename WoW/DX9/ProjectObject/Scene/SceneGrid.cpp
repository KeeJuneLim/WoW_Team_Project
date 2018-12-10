#include "stdafx.h"
#include "SceneGrid.h"
#include "ProjectObject/Display/RectObject.h"
#include "ProjectObject/Display/Cube.h"
#include "ProjectObject/Display/Grid.h"

SceneGrid::SceneGrid(WORD index)
	:IScene(index)
{

}

SceneGrid::~SceneGrid()
{
	SAFE_DELETE(rect);
	SAFE_DELETE(cube);
	SAFE_DELETE(grid);
	SAFE_DELETE(cubeChild);
}

void SceneGrid::Init()
{
	grid = new Grid; grid->Init();
	rect = new RectObject; rect->Init();
	cube = new Cube; cube->Init();
	cubeChild = new Cube; cubeChild->Init();
	cubeChild->SetParent(&cube);
	cube->GetChildren()->push_back(cubeChild);
	cubeChild->SetPosition(&D3DXVECTOR3(-2.5, 2.5, 0));

	Camera::Get()->SetTargetPosition(cube->GetPosition());
	
	D3DXVECTOR3 lightDir = D3DXVECTOR3(1, -1, 1);
	D3DXCOLOR lightColor = Color::WHITE;
	D3DLIGHT9 light = DXUtil::InitDirectional(&lightDir, &lightColor);
	DX::GetDevice()->SetLight(0, &light);
	DX::GetDevice()->LightEnable(0, true);
	
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	DX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	DX::GetDevice()->SetRenderState(D3DRS_SPECULARENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xff222222);
	
}

void SceneGrid::Update()
{
	SAFE_UPDATE(rect);
	SAFE_UPDATE(cube);
	SAFE_UPDATE(grid);
}

void SceneGrid::Render()
{
	SAFE_RENDER(rect);
	SAFE_RENDER(cube);
	SAFE_RENDER(grid);
}
