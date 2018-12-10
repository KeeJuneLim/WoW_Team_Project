#include "stdafx.h"
#include "InstancingTest.h"

InstancingTest::InstancingTest()
{
	Rendering::Get()->AddRenderingObject(this);
	//Rendering::Get()->AddShadowCastObject(this);
}

InstancingTest::~InstancingTest()
{
	Rendering::Get()->RemoveRenderingObject(this);
	//Rendering::Get()->RemoveShadowCastObject(this);

	SAFE_RELEASE(vbIns);
}

void InstancingTest::Init()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + _T("Instancing.fx"));
	
	D3DVERTEXELEMENT9 vtxEle[8] =
	{
		{ 0, 0,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
		{ 1, 0,     D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,	1 },
		{ 1, 16,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	2 },
		{ 1, 32,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	3 },
		{ 1, 48,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	4 },
		{ 1, 64,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	5 },
		D3DDECL_END()
	};
	DX::GetDevice()->CreateVertexDeclaration(vtxEle, &vertexDecl);
	
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 0, 0.9f, 0);
	vector<VertexType> vertices;
	for (int i = 0; i < g_vecQuadVertex.size(); i++)
	{
		vertices.push_back(VertexType(
			g_vecQuadVertex[i],
			g_vecQuadUV[i])
		);
		D3DXVec3TransformCoord(&vertices[i].p, &vertices[i].p, &mat);
	}
	vertexCount = vertices.size();
	indexCount = g_vecQuadIndex.size();

	DXUtil::CreateVertexBuffer(vb, (LPVOID)&vertices[0], VertexType::FVF,
		vertices.size(), sizeof(VertexType));
	DXUtil::CreateIndexBuffer(ib, (LPVOID)&g_vecQuadIndex[0], g_vecQuadIndex.size(), sizeof(g_vecQuadIndex[0]));

	DX::GetDevice()->CreateVertexBuffer(numInstance * sizeof(InstanceType), D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &vbIns, 0);

	tex = Assets::GetTexture((ASSET_PATH + L"textures/billboard/tree.png").c_str());
	//tex = Assets::GetTexture((ASSET_PATH + L"textures/billboard/grass.dds").c_str());
	effect->SetTexture("DiffuseMapTex", tex);

	datas.resize(numInstance);
	instances.resize(numInstance);
	float range = 65.0f;
	for (int i = 0; i<numInstance; i++)
	{
		datas[i].position.x = ((float)rand() / (float)(RAND_MAX)) * range;
		datas[i].position.z = ((float)rand() / (float)(RAND_MAX)) * range;
		Objects::GetCurrentMap()->ComputeHeight(datas[i].position.y, datas[i].position);

		float red = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
		float green = ((float)rand() / (float)(RAND_MAX)) * 1.0f;

		datas[i].color.x = red + 1.0f;
		datas[i].color.y = green + 0.5f;
		datas[i].color.z = 0.0f;

		instances[i].color = datas[i].color;
	}
}

void InstancingTest::Update()
{
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

	D3DXVECTOR3* camPos = Camera::Get()->GetPosition();
	D3DXVECTOR3 modelPos;
	D3DXMATRIX matRX, matRY, matT;

	static int windDirection = 1;
	static float windRotation = 0.0f;
	float rotSpeed = 0.1f;
	float angle;

	windRotation += windDirection * rotSpeed;
	if (windRotation > 10.0f)
		windDirection = -1;
	else if (windRotation < -10.0f)
		windDirection = 1;

	float windRad = windRotation * 0.0174532925f;
	for (int i = 0; i<numInstance; i++)
	{
		modelPos = datas[i].position;

		angle = atan2(modelPos.x - camPos->x, modelPos.z - camPos->z);

		D3DXMatrixRotationY(&matRY, angle);
		D3DXMatrixRotationX(&matRX, windRad);
		D3DXMatrixTranslation(&matT, modelPos.x, modelPos.y, modelPos.z);

		instances[i].matrix = matRX * matRY * matT;
	}

	InstanceType* instancesPtr;
	vbIns->Lock(0, 0, (LPVOID*)&instancesPtr, D3DLOCK_DISCARD);
	memcpy(instancesPtr, (LPVOID)&instances[0], (sizeof(InstanceType) * numInstance));
	vbIns->Unlock();
}

void InstancingTest::Render()
{
	
}

void InstancingTest::Render(IShader * pShader)
{
	effect->SetMatrix("gWorldMatrix", &worldMatrix);
	effect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
	effect->SetMatrix("gProjMatrix", Camera::Get()->GetProjectionMatrix());

	// Stream zero is our model, and its frequency is how we communicate the number of instances required,
	// which in this case is the total number of boxes
	DX::GetDevice()->SetStreamSource(0, vb, 0, sizeof(VertexType));
	DX::GetDevice()->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | numInstance);

	// Stream one is the instancing buffer, so this advances to the next value
	// after each box instance has been drawn, so the divider is 1.
	DX::GetDevice()->SetStreamSource(1, vbIns, 0, sizeof(InstanceType));
	DX::GetDevice()->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

	DX::GetDevice()->SetIndices(ib);
	DX::GetDevice()->SetVertexDeclaration(vertexDecl);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	effect->Begin(0, 0);
	effect->BeginPass(0);

	effect->CommitChanges();
	DX::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);

	effect->EndPass();
	effect->End();

	DX::GetDevice()->SetStreamSourceFreq(0, 1);
	DX::GetDevice()->SetStreamSourceFreq(1, 1);

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}


