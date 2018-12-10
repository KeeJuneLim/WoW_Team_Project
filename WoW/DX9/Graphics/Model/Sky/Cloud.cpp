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

	// �ϴ� ��鿡�� �� ������ ũ�⸦ �����մϴ�.
	float quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// �ʺ� �������� �ϴ� ����� �������� ����մϴ�.
	float radius = skyPlaneWidth / 2.0f;

	// ���� �� ���� ����� ����մϴ�.
	float constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// �ؽ�ó ��ǥ ���� ���� ����մϴ�.
	float textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// �ϴ� ����� �ݺ��ϰ� ������ ���� ���� ������� ��ǥ�� ����ϴ�.
	for (int j = 0; j <= skyPlaneResolution; j++)
	{
		for (int i = 0; i <= skyPlaneResolution; i++)
		{
			// ���� ��ǥ�� ����մϴ�.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// �ؽ�ó ��ǥ�� ����մϴ�.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// �� ��ǥ�� �߰��ϱ� ���� �ϴ� ��� �迭�� �ε����� ����մϴ�.
			int index = j * skyPlaneDim + i;

			// �ϴ� ��� �迭�� ��ǥ�� �߰��մϴ�.
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
	// �ϴ� ��� �޽��� ���� ���� ����մϴ�.
	vertexCount = skyPlaneDim * skyPlaneDim * 6;

	// �ε��� ���� ������ ���� ���� �����մϴ�.
	indexCount = vertexCount;

	vtx.resize(vertexCount);
	indices.resize(indexCount);

	// �ε����� ���� �迭�� �ʱ�ȭ �մϴ�.
	int index = 0;

	// �ϴ� ��� �迭 �����ͷ� �������� �ε��� �迭�� �ε��մϴ�.
	for (int j = 0; j<skyPlaneResolution; j++)
	{
		for (int i = 0; i<skyPlaneResolution; i++)
		{
			index1 = j * skyPlaneDim + i;
			index2 = j * skyPlaneDim + (i + 1);
			index3 = (j + 1) * skyPlaneDim + i;
			index4 = (j + 1) * skyPlaneDim + (i + 1);

			// �ﰢ�� 1 - ���� ��
			vtx[index] = vertices[index1];
			indices[index] = index;
			index++;

			// �ﰢ�� 1 - ������ ��
			vtx[index] = vertices[index2];
			indices[index] = index;
			index++;

			// �ﰢ�� 1 - ���� �Ʒ�
			vtx[index] = vertices[index3];
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ���� �Ʒ�
			vtx[index] = vertices[index3];
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ������ ��
			vtx[index] = vertices[index2];
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ������ �Ʒ�
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
