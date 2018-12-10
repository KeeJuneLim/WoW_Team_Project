#include "stdafx.h"
#include "BoundaryShader.h"

BoundaryShader::BoundaryShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Boundary.fx");
}


BoundaryShader::~BoundaryShader()
{
	
}

void BoundaryShader::PreRender()
{
	
}

void BoundaryShader::Render(IDisplayObject* pObj)
{
	
}

void BoundaryShader::PostRender()
{

}

void BoundaryShader::Render(Boundary * pObj)
{
	UINT numPasses = 0;
	effect->Begin(&numPasses, NULL);

	D3DXMATRIX wvpMatrix =
		pObj->worldMatrix *
		*Camera::Get()->GetViewMatrix() *
		*Camera::Get()->GetProjectionMatrix();

	SetMatrix(MT_WVP, &wvpMatrix);
	effect->SetInt("gIsPicked", pObj->isPicked);

	for (int i = 0; i < numPasses; i++)
	{
		effect->BeginPass(i);

		pObj->Draw();

		effect->EndPass();
	}

	effect->End();
}


