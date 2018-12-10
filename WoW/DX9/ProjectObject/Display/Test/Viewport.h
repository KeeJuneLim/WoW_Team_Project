#pragma once
#include "stdafx.h"

class Viewport : public IDisplayObject
{
	ID3DXMesh*	mMesh;
	D3DVIEWPORT9 LViewport, RViewport;

	~Viewport()
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SAFE_RELEASE(mMesh);
		SAFE_DELETE(material);
	}

	void Init()
	{
		material = new Material();
		shaderType = ST_Lighting;
		Rendering::Get()->AddRenderingObject(this);

		scale = D3DXVECTOR3(3, 3, 3);

		wstring filepath = ASSET_PATH + L"Models/x/zealot/zealot.x";
		D3DXLoadMeshFromX(filepath.c_str(), D3DXMESH_MANAGED, 
			DX::GetDevice(), NULL, NULL, NULL, NULL, &mMesh);

		assert(mMesh && "Loading x file is failed");

		filepath = ASSET_PATH + L"Models/x/zealot/Zealot_Diffuse.bmp";
		material->SetDiffuseMapTex(Assets::GetTexture(filepath.c_str()));

		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		D3DSURFACE_DESC bufferDesc;
		DX::GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		pBackBuffer->GetDesc(&bufferDesc);
		
		//Viewport setting
		LViewport.X = 0;
		LViewport.Y = 0;
		LViewport.Width = bufferDesc.Width / 2.0f;
		LViewport.Height = bufferDesc.Height;

		RViewport.X = LViewport.Width;
		RViewport.Y = 0;
		RViewport.Width = LViewport.Width;
		RViewport.Height = LViewport.Height;

		pBackBuffer->Release();

	}

	void Update()
	{
		UpdateLocalMatrix();
		UpdateWorldMatrix();
	}

	void Render()
	{
		Shaders::Get()->GetShader(shaderType)->Render(this);
	}

	void Draw() 
	{
		IShader* shader = Shaders::Get()->GetShader(shaderType);
		shader->SetMatrix(MatrixType::MT_World, &worldMatrix);
		shader->SetTexture(TextureType::TT_DiffuseMap, material->GetDiffuseMapTex());
		shader->SetMaterial(material);

		D3DXMATRIX viewMatrix;
		
		//¿ÞÂÊ ºäÆ÷Æ® ·»´õ¸µ
		DX::GetDevice()->SetViewport(&LViewport);
		DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			Color::BLUE, 1.0f, 0);
		
		viewMatrix = *Camera::Get()->GetViewMatrix();
		shader->SetMatrix(MatrixType::MT_World, &viewMatrix);
		shader->Commit();

		mMesh->DrawSubset(0);


		//¿À¸¥ÂÊ ºäÆ÷Æ® ·»´õ¸µ
		DX::GetDevice()->SetViewport(&RViewport);
		DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			Color::RED, 1.0f, 0);

		D3DXMATRIX matR;
		D3DXMatrixRotationY(&matR, D3DX_PI);
		viewMatrix = matR * viewMatrix;

		shader->SetMatrix(MatrixType::MT_World, &viewMatrix);
		shader->Commit();
		
		mMesh->DrawSubset(0);
	}
};