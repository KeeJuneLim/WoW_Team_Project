#include "stdafx.h"
#include "Cloud.h"

Cloud::Cloud()
{
	
	material = new Material();

	shaderType = ST_Cloud;
	Rendering::Get()->AddRenderingObject(this);
}

Cloud::~Cloud()
{
	Rendering::Get()->RemoveRenderingObject(this);
}

void Cloud::Init()
{
	D3DVERTEXELEMENT9 vertexElem[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(VertexType::FVF, vertexElem);
	DX::GetDevice()->CreateVertexDeclaration(vertexElem, &vertexDecl);

	int skyPlaneResolution = 50;
	int skyPlaneDim = skyPlaneResolution + 1;
	float skyPlaneWidth = 12.0f;
	float skyPlaneTop = 1.0f;
	float skyPlaneBottom = 0.5f;
	int textureRepeat = 3;

	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	vertices.resize(skyPlaneDim * skyPlaneDim);

	// 하늘 평면에서 각 쿼드의 크기를 결정합니다.
	float quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// 너비를 기준으로 하늘 평면의 반지름을 계산합니다.
	float radius = skyPlaneWidth / 2.0f;

	// 증가 할 높이 상수를 계산합니다.
	float constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// 텍스처 좌표 증가 값을 계산합니다.
	float textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// 하늘 평면을 반복하고 제공된 증분 값을 기반으로 좌표를 만듭니다.
	for (int j = 0; j <= skyPlaneResolution; j++)
	{
		for (int i = 0; i <= skyPlaneResolution; i++)
		{
			// 정점 좌표를 계산합니다.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// 텍스처 좌표를 계산합니다.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// 이 좌표를 추가하기 위해 하늘 평면 배열에 인덱스를 계산합니다.
			int index = j * skyPlaneDim + i;

			// 하늘 평면 배열에 좌표를 추가합니다.
			vertices[index].p.x = positionX;
			vertices[index].p.y = positionY;
			vertices[index].p.z = positionZ;
			vertices[index].t.x = tu;
			vertices[index].t.y = tv;
		}
	}

	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index4 = 0;

	vector<VertexType> vtx;
	
	vertexStride = sizeof(VertexType);
	// 하늘 평면 메쉬의 정점 수를 계산합니다.
	vertexCount = skyPlaneDim * skyPlaneDim * 6;

	// 인덱스 수를 꼭지점 수와 같게 설정합니다.
	indexCount = vertexCount;

	vtx.resize(vertexCount);
	indices.resize(indexCount);

	// 인덱스를 정점 배열로 초기화 합니다.
	int index = 0;

	// 하늘 평면 배열 데이터로 꼭지점과 인덱스 배열을 로드합니다.
	for (int j = 0; j<skyPlaneResolution; j++)
	{
		for (int i = 0; i<skyPlaneResolution; i++)
		{
			index1 = j * skyPlaneDim + i;
			index2 = j * skyPlaneDim + (i + 1);
			index3 = (j + 1) * skyPlaneDim + i;
			index4 = (j + 1) * skyPlaneDim + (i + 1);

			// 삼각형 1 - 왼쪽 위
			vtx[index] = vertices[index1];
			indices[index] = index;
			index++;

			// 삼각형 1 - 오른쪽 위
			vtx[index] = vertices[index2];
			indices[index] = index;
			index++;

			// 삼각형 1 - 왼쪽 아래
			vtx[index] = vertices[index3];
			indices[index] = index;
			index++;

			// 삼각형 2 - 왼쪽 아래
			vtx[index] = vertices[index3];
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 위
			vtx[index] = vertices[index2];
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 아래
			vtx[index] = vertices[index4];
			indices[index] = index;
			index++;
		}
	}

	DXUtil::CreateVertexBuffer(vb, (LPVOID)&vtx[0], VertexType::FVF,
		vertexCount, vertexStride);
	DXUtil::CreateIndexBuffer(ib, (LPVOID)&indices[0], indexCount, sizeof(indices[0]));

	wstring filepath;
	filepath = ASSET_PATH + L"textures/sky/cloud.dds";
	material->SetDiffuseMapTex(Assets::GetTexture(filepath.c_str()));
	filepath = ASSET_PATH + L"textures/sky/perturb.dds";
	material->SetDiffuseMapTex(Assets::GetTexture(filepath.c_str()), 1);
}

void Cloud::Update()
{
	
}

void Cloud::Render()
{
	
	
}

void Cloud::Render(IShader * pShader)
{
	position = *Camera::Get()->GetPosition();

	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pShader->Render(this);

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void Cloud::Draw()
{
	DrawIndexedPrimitive();
}
