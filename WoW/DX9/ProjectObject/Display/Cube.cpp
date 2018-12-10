#include "stdafx.h"
#include "Cube.h"

Cube::Cube()
{
	D3DXMatrixIdentity(&localMatrix);
	D3DXMatrixIdentity(&worldMatrix);
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	useBuffer = true;
	parent = NULL;
	
	//boundary = new BoundingSphere(D3DXVECTOR3(0, 0, 0), 2.2f);
	boundary = new BoundingBox(D3DXVECTOR3(-2, -2, -2), D3DXVECTOR3(2, 2, 2));
	boundary->InitGeometry();
}

Cube::~Cube()
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(vb);
	SAFE_DELETE(boundary);
}

void Cube::Init()
{
	vector<D3DXVECTOR2> indexedUVList;
	
	indexedUVList.push_back(g_vecCubeUV[0]);
	indexedUVList.push_back(g_vecCubeUV[1]);
	indexedUVList.push_back(g_vecCubeUV[2]);

	indexedUVList.push_back(g_vecCubeUV[0]);
	indexedUVList.push_back(g_vecCubeUV[2]);
	indexedUVList.push_back(g_vecCubeUV[3]);

	vertices.resize(g_vecCubeIndex.size());
	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vertices[i] = VertexType(
			g_vecCubeVertex[g_vecCubeIndex[i]],
			g_vecCubeVertex[g_vecCubeIndex[i]],
			indexedUVList[i % 6]);
	}

	DXUtil::CreateVertexBuffer(vb, (LPVOID)&vertices[0], VertexType::FVF,
		vertices.size(), sizeof(VertexType));
	
	wstring filepath = ASSET_PATH + L"textures/girl.png";
	D3DXCreateTextureFromFile(DX::GetDevice(), filepath.c_str(), &tex);
}

void Cube::Update()
{
	if (parent == NULL) UpdateInput();

	Ray ray = Ray::RayFromScreenToWorld(Input::GetMousePosition().x, Input::GetMousePosition().y);
	boundary->isPicked = boundary->IntersectRay(ray);

	float moveSpeed = 6.0f;
	float rotSpeed = 4.0f;
	
	rotation.y += deltaTransform.y * rotSpeed * Time::Get()->GetDeltaTime();

	D3DXVECTOR3 _forward;
	D3DXMATRIXA16 matT, matR, matS;
	D3DXMatrixRotationYawPitchRoll(&matR, rotation.y, rotation.x, rotation.z);
	D3DXVec3TransformNormal(&_forward,
		&D3DXVECTOR3(deltaTransform.x, 0, deltaTransform.z), &matR);
	D3DXVec3Normalize(&_forward, &_forward);
	
	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &matR);

	position = position + _forward * moveSpeed * Time::Get()->GetDeltaTime();

	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	localMatrix = matS * matR * matT;

	UpdateWorldMatrix();

	boundary->UpdateTransform(worldMatrix);

	for (auto p : children)
	{
		SAFE_UPDATE(p);
	}
}

void Cube::Render()
{
	if (parent == NULL)
	{
		ImGui::Begin("Cube");
		ImGui::SliderFloat3("Position", (float*)&position, -10, 10);
		ImGui::SliderFloat3("Rotation", (float*)&rotation, -D3DX_PI, D3DX_PI);
		ImGui::SliderFloat3("Scaling", (float*)&scale, 0.1f, 10);
		ImGui::Text("Forward %.2f %.2f %.2f", forward.x, forward.y, forward.z);
		ImGui::End();
	}
	
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	if ( boundary->isPicked)
		DX::GetDevice()->SetMaterial(&DXUtil::GREEN_MTRL);
	else
		DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);

	DX::GetDevice()->SetTexture(0, tex);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
	DX::GetDevice()->SetFVF(VertexType::FVF);
	
	if (useBuffer)
	{
		DX::GetDevice()->SetStreamSource(0, vb, 0, sizeof(VertexType));
		DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices.size() / 3);
	}
	else
	{
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
			vertices.size() / 3, &vertices[0], sizeof(VertexType));
	}
	
	SAFE_RENDER(boundary);
	
	for (auto p : children)
	{
		SAFE_RENDER(p);
	}
}

void Cube::UpdateInput()
{
	if (Input::KeyPress('W'))
		deltaTransform.z = 1;
	else if (Input::KeyPress('S'))
		deltaTransform.z = -1;
	else
		deltaTransform.z = 0;

	if (Input::KeyPress('E'))
		deltaTransform.x = 1;
	else if (Input::KeyPress('Q'))
		deltaTransform.x = -1;
	else
		deltaTransform.x = 0;

	if (Input::KeyPress('D'))
		deltaTransform.y = 1;
	else if (Input::KeyPress('A'))
		deltaTransform.y = -1;
	else
		deltaTransform.y = 0;

	//if (Input::Get()->ButtonPress(Input::RBUTTON))
	//{
	//	rotation.y = Camera::Get()->GetRotation()->y;
	//	//rotation = *Camera::Get()->GetRotation();
	//}

}

void Cube::UpdateWorldMatrix()
{
	if (parent)
	{
		worldMatrix = localMatrix * *parent->GetWorldMatrix();
	}
	else
	{
		worldMatrix = localMatrix;
	}
}
