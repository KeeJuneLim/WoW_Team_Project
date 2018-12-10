#pragma once
#include "stdafx.h"

class ExtrackFromBuffer : public IDisplayObject
{
	vector<D3DXVECTOR3> vertices;
	vector<WORD> indices;

public:
	void Init()
	{
		shaderType = ShaderType::ST_None;
		Rendering::Get()->AddRenderingObject(this);

		ID3DXMesh* mesh;
		D3DXCreateSphere(DX::GetDevice(), 4, 10, 10, &mesh, NULL);
		
		IDirect3DVertexBuffer9* vb;
		mesh->GetVertexBuffer(&vb);
		DWORD stride = mesh->GetNumBytesPerVertex();
		DWORD numVertices = mesh->GetNumVertices();
		BYTE* data = NULL;
		vb->Lock(0, 0, (LPVOID*)&data, 0);
		vector<D3DXVECTOR3> positions;
		vertices.resize(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(data + i * stride);
			vertices[i] = *pos;
		}
		vb->Unlock();
		vb->Release();

		IDirect3DIndexBuffer9* ib;
		mesh->GetIndexBuffer(&ib);
		D3DINDEXBUFFER_DESC ibDesc;
		ib->GetDesc(&ibDesc);
		stride = ibDesc.Format == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);
		DWORD numIndices = ibDesc.Size / stride;
		indices.resize(numIndices);
		ib->Lock(0, 0, (LPVOID*)&data, 0);
		memcpy(&indices[0], data, ibDesc.Size);
		ib->Unlock();
		ib->Release();

		mesh->Release();
	}

	void Update()
	{

	}

	void Render()
	{
		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetTexture(0, NULL);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
		DX::GetDevice()->SetFVF(D3DFVF_XYZ);
		DX::GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0,
			vertices.size(), indices.size() / 3, &indices[0], D3DFMT_INDEX16,
			&vertices[0], sizeof(D3DXVECTOR3));
		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	void Draw()
	{

	}
};