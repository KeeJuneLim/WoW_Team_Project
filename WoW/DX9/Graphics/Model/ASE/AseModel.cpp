#include "stdafx.h"
#include "AseModel.h"
#include "AseModelParts.h"
#include "AseLoader.h"

AseModel::AseModel()
{
	rootParts = NULL;
}


AseModel::~AseModel()
{
	
}

void AseModel::Load(LPCTSTR filepath, LPCTSTR filename)
{
	AseLoader pAseLoader;
	rootParts = pAseLoader.Load(filepath, filename, &aseScene);
	AddChild(rootParts);
	rootParts->SetModel(this);
}

void AseModel::Init()
{
	rootParts->Init();

	firstTick = aseScene.numFirstFrame * aseScene.TicksPerFrame;
	lastTick = aseScene.numLastFrame * aseScene.TicksPerFrame;
	tickSpeed = aseScene.FrameSpeed * aseScene.TicksPerFrame;

	currentTick = firstTick;
}


void AseModel::Update()
{	
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

	currentTick += tickSpeed * Time::Get()->GetDeltaTime();
	if (currentTick > lastTick) currentTick = firstTick;
	
	if (rootParts)
		rootParts->Update();
}

void AseModel::Render()
{
	
}

void AseModel::Render(IShader * pShader)
{
	rootParts->Render(pShader);
}

void AseModel::Draw()
{
}
