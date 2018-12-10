#include "stdafx.h"
#include "ShaderCube.h"

ShaderCube::ShaderCube()
{
	name = L"ShaderCube";
	boundary = new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1.3f);
	//boundary = new BoundingBox(D3DXVECTOR3(-1.2, -1.2, -1.2), D3DXVECTOR3(1.2, 1.2, 1.2));
	boundary->InitGeometry();
	scale = D3DXVECTOR3(5, 5, 5);
	material = new Material();

	shaderType = ST_Shadow;
	Rendering::Get()->AddRenderingObject(this);
	Rendering::Get()->AddShadowCastObject(this);
}

ShaderCube::ShaderCube(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	name = L"ShaderCube";
	boundary = new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1.3f);
	//boundary = new BoundingBox(D3DXVECTOR3(-1.2, -1.2, -1.2), D3DXVECTOR3(1.2, 1.2, 1.2));
	boundary->InitGeometry();
	position = pos + dir;
	scale = D3DXVECTOR3(5, 5, 5);
	material = new Material();

	skillDir = dir;
	shaderType = ST_Shadow;

	Rendering::Get()->AddRenderingObject(this);
	Rendering::Get()->AddShadowCastObject(this);
}

ShaderCube::~ShaderCube()
{
	Rendering::Get()->RemoveRenderingObject(this);
	Rendering::Get()->RemoveShadowCastObject(this);
}

void ShaderCube::Init()
{
	D3DVERTEXELEMENT9 vertexElem[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(VertexType::FVF, vertexElem);

	DX::GetDevice()->CreateVertexDeclaration(vertexElem, &vertexDecl);

	vector<D3DXVECTOR2> indexedUVList;
	
	indexedUVList.push_back(g_vecCubeUV[0]);
	indexedUVList.push_back(g_vecCubeUV[1]);
	indexedUVList.push_back(g_vecCubeUV[2]);

	indexedUVList.push_back(g_vecCubeUV[0]);
	indexedUVList.push_back(g_vecCubeUV[2]);
	indexedUVList.push_back(g_vecCubeUV[3]);

	vertexCount = g_vecCubeIndex.size();
	vertexStride = sizeof(VertexType);
	vertices.resize(vertexCount);

	D3DXCOLOR color[] = {
		Color::BLACK, Color::BLUE, Color::GREEN, Color::RED, Color::WHITE, Color::YELLOW
	};
	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		//PC
		//vertices[i] = VertexType(g_vecCubeVertex[g_vecCubeIndex[i]], color[i % 6]);
		//PT
		//vertices[i] = VertexType(g_vecCubeVertex[g_vecCubeIndex[i]], indexedUVList[i % 6]);
		//PNT
		vertices[i] = VertexType(g_vecCubeVertex[g_vecCubeIndex[i]], g_vecCubeVertex[g_vecCubeIndex[i]], indexedUVList[i % 6]);
	}

	DXUtil::CreateVertexBuffer(vb, (LPVOID)&vertices[0], 0,
		vertexCount, vertexStride);
	
	wstring filepath = ASSET_PATH + L"textures/etc/girl.png";
	material->SetDiffuseMapTex(Assets::GetTexture(filepath.c_str()));
}

void ShaderCube::Update()
{
	//IDisplayObject::UpdateInput();
	//IDisplayObject::ApplyInput();
	////position = targetPosition;
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

	boundary->UpdateTransform(worldMatrix);

	for (auto p : children)
	{
		SAFE_UPDATE(p);
	}

	position += skillDir * 100 * Time::Get()->GetDeltaTime();
}

void ShaderCube::Render()
{
	
}

void ShaderCube::Render(IShader * pShader)
{
	if (Frustum::Get()->IsSphereInsideFrustum(&position, boundary->cullingRadius) == false) return;

	if (boundary->isPicked)
		material->Diffuse = D3DXCOLOR(0, 1, 0, 1);
	else
		material->Diffuse = D3DXCOLOR(1, 1, 1, 1);


	pShader->Render(this);

	SAFE_RENDER(boundary);

	for (auto p : children)
	{
		SAFE_RENDER(p);
	}

	IDisplayObject::UpdateUI();
}

void ShaderCube::Draw()
{
	/*IShader* shader = Shaders::Get()->GetShader(shaderType);
	shader->SetMatrix(MatrixType::MT_World, &worldMatrix);
	shader->SetTexture(TextureType::TT_DiffuseMap, material->GetDiffuseMapTex());
	shader->SetMaterial(material);
	shader->Commit();
*/
	DrawPrimitive();
}



