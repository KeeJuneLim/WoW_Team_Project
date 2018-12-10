#pragma once
#include "stdafx.h"

class MipmapFilter : public IDisplayObject
{
	typedef VERTEX_PNT VertexType;
	vector<VertexType>	vertices;

public:
	MipmapFilter()
	{
		material = new Material();
		shaderType = ST_Lighting;
		Rendering::Get()->AddRenderingObject(this);
	}

	~MipmapFilter()
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SAFE_DELETE(material);
	}
	
	virtual void Init() override
	{
		for (int i = 0; i < g_vecQuadIndex.size(); i++)
		{
			vertices.push_back(VertexType(
				g_vecQuadVertex[g_vecQuadIndex[i]],
				D3DXVECTOR3(0, 1, 0),
				g_vecQuadUV[g_vecQuadIndex[i]]));
		}
		
		D3DXMATRIXA16 matS, matR;
		D3DXMatrixScaling(&matS, 80, 80, 0);
		D3DXMatrixRotationX(&matR, D3DX_PI / 2);
		for (auto &p : vertices)
			D3DXVec3TransformCoord(&p.p, &p.p, &(matS * matR));

		wstring path;
		path = ASSET_PATH + L"textures/mipmap/png512.png";
		LPDIRECT3DTEXTURE9 pTex = Assets::GetTexture(path.c_str());
		
		vector<LPDIRECT3DTEXTURE9> mipmapTex;
		path = ASSET_PATH + L"textures/mipmap/png256.png";
		mipmapTex.push_back(Assets::GetTexture(path.c_str()));
		path = ASSET_PATH + L"textures/mipmap/png128.png";
		mipmapTex.push_back(Assets::GetTexture(path.c_str()));
		path = ASSET_PATH + L"textures/mipmap/png64.png";
		mipmapTex.push_back(Assets::GetTexture(path.c_str()));
		path = ASSET_PATH + L"textures/mipmap/png32.png";
		mipmapTex.push_back(Assets::GetTexture(path.c_str()));
		path = ASSET_PATH + L"textures/mipmap/png16.png";
		mipmapTex.push_back(Assets::GetTexture(path.c_str()));
		
		
		LPDIRECT3DSURFACE9 src = NULL, dest = NULL;
		
		for (size_t i = 0; i < mipmapTex.size(); i++)
		{
			pTex->GetSurfaceLevel(i + 1, &dest);
			mipmapTex[i]->GetSurfaceLevel(0, &src);

			D3DXLoadSurfaceFromSurface(dest, NULL, NULL, src, NULL, NULL, D3DX_DEFAULT, 0);

			src->Release();
			dest->Release();
		}

		material->SetDiffuseMapTex(pTex);

		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		DX::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		DX::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	virtual void Update() override {}

	virtual void Render() override
	{
		Shaders::Get()->GetShader(shaderType)->Render(this);
	}

	virtual void Draw() override 
	{
		IShader* shader = Shaders::Get()->GetShader(shaderType);
		shader->SetMatrix(MatrixType::MT_World, &worldMatrix);
		shader->SetTexture(TextureType::TT_DiffuseMap, material->GetDiffuseMapTex());
		shader->SetMaterial(material);
		shader->Commit();

		DX::GetDevice()->SetFVF(VertexType::FVF);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			vertices.size() / 3, &vertices[0], sizeof(VertexType));
	}
};

