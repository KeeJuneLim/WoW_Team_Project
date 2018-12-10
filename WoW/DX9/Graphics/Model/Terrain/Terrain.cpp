#include "stdafx.h"
#include "Terrain.h"
#include "TerrainEditor.h"
#include "QuadTree.h"



Terrain::Terrain()
	:heightFactor(1.0f), tiling(1.0f), lodRatio(0.015f)
{
	material = new Material();
	//scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
}


Terrain::~Terrain()
{
	Rendering::Get()->RemoveRenderingObject(this);
	//Rendering::Get()->RemoveShadowCastObject(this);

	SAFE_DELETE(quadTree);
	SAFE_DELETE(editor);
}


void Terrain::Init()
{
	shaderType = ST_TerrainEditor;
	Rendering::Get()->AddRenderingObject(this);
	//Rendering::Get()->AddShadowCastObject(this);

	D3DVERTEXELEMENT9 vertexElem[] =
	{
		{ 0, 0,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 12,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,	0 },
		{ 0, 24,    D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,	0 },
		{ 0, 32,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	1 },
		D3DDECL_END()
	};
	DX::GetDevice()->CreateVertexDeclaration(vertexElem, &vertexDecl);

	editor = new TerrainEditor(this);
}

void Terrain::Update()
{
	if (editor)
	{
		editor->UpdateUI();

		D3DXVECTOR3 pos = Input::GetMousePosition();

		if (editor->type != TerrainEditor::BT_None)
		{
			
			if (ComputeClickPosition(editor->center, pos.x, pos.y) == true)
			{
				if (editor->type == TerrainEditor::BT_Blending)
				{
					if (Input::ButtonPress(Input::RBUTTON))
					{

						editor->EditBlendingTex(dimension);
					}
				}
				else
				{
					if (Input::ButtonPress(Input::RBUTTON))
					{

						editor->EditHeight((BYTE*)&vertices[0],
							sizeof(VertexType), editedIndices, dimension);
						UpdateEditedVertexNormal();
						UpdateVB();
					}

					if (Input::ButtonUp(Input::RBUTTON))
					{
						UpdateNormal();
						UpdateVB();
					}
				}
				
			}
		}
	}
	
}


void Terrain::Render(IShader * pShader)
{
	//DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*
	//DX::GetDevice()->SetRenderState(D3DRS_FOGENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_FOGCOLOR, 0xffdddddd);
	DX::GetDevice()->SetRenderState(D3DRS_FOGDENSITY, FtoDw(0.04f));
	DX::GetDevice()->SetRenderState(D3DRS_FOGSTART, FtoDw(100.0f));
	DX::GetDevice()->SetRenderState(D3DRS_FOGEND, FtoDw(250.0f));
	DX::GetDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	*/
	
	UpdateIB();

	pShader->SetMatrix(MT_World, &worldMatrix);
	pShader->SetTexture(TT_DiffuseMap, material->GetTexture(TT_DiffuseMap));
	pShader->SetTexture(TT_DiffuseMap1, material->GetTexture(TT_DiffuseMap1));
	pShader->SetTexture(TT_DiffuseMap2, material->GetTexture(TT_DiffuseMap2));
	pShader->SetTexture(TT_DiffuseMap3, material->GetTexture(TT_DiffuseMap3));
	pShader->SetTexture(TT_DiffuseMap4, material->GetTexture(TT_DiffuseMap4));
	pShader->SetTexture(TT_AlphaMap, material->GetTexture(TT_AlphaMap));
	pShader->SetMaterial(material);
	pShader->SetTerrainEditor(editor);
	pShader->Commit();

	pShader->Render(this);

	DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	DX::GetDevice()->SetRenderState(D3DRS_FOGENABLE, false);
}


void Terrain::Draw()
{
	IDisplayObject::DrawIndexedPrimitive();
}


bool Terrain::ComputeHeight(float & height, const D3DXVECTOR3 & pos)
{
	if (pos.x < 0 || pos.z < 0 ||
		pos.x > size.x || pos.z > size.z)
	{
		height = 0.0f;
		return false;
	}

	// 1-3
	// |\|
	// 0-2
	float ratioX = pos.x;
	float ratioZ = pos.z;
	int nX = (int)ratioX;
	int nZ = (int)ratioZ;
	float fDeltaX = ratioX - nX;
	float fDeltaZ = ratioZ - nZ;

	int _0 = (nZ + 0) * dimension + nX + 0;
	int _1 = (nZ + 1) * dimension + nX + 0;
	int _2 = (nZ + 0) * dimension + nX + 1;
	int _3 = (nZ + 1) * dimension + nX + 1;

	if (fDeltaX + fDeltaZ < 1.0f)
	{
		float zY = vertices[_1].p.y - vertices[_0].p.y;
		float xY = vertices[_2].p.y - vertices[_0].p.y;
		height = vertices[_0].p.y + xY * fDeltaX + zY * fDeltaZ;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;

		float xY = vertices[_1].p.y - vertices[_3].p.y;
		float zY = vertices[_2].p.y - vertices[_3].p.y;
		height = vertices[_3].p.y + xY * fDeltaX + zY * fDeltaZ;
	}
	return true;
}

bool Terrain::ComputeClickPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
{
	Ray		ray = Ray::RayFromScreenToWorld(screenX, screenY);
	float	minDist = FLT_MAX;
	float	currDist;
	bool	bIntersect = false;

	for (int i = 0; i < indexCount; i += 3)
	{
		if (ray.IntersectTri(
			&vertices[usedIndices[i]].p,
			&vertices[usedIndices[i + 1]].p,
			&vertices[usedIndices[i + 2]].p, &currDist))
		{
			if (currDist < minDist)
			{
				bIntersect = true;
				minDist = currDist;
				vOut = ray.position + ray.direction * currDist;
			}
		}
	}
	return bIntersect;
}

void Terrain::Load(wstring filepath)
{
	std::ifstream fin(filepath, std::ios::binary);
	assert(fin.is_open() && "fin.is_open() == false");
	
	fin.seekg(0, fin.end);
	int len = fin.tellg();
	unsigned char* heights = new unsigned char[len];
	fin.seekg(0, fin.beg);
	fin.read((char*)heights, len);
	fin.close();

	dimension = sqrt(len);
	InitVertices();
	
	for (int z = 0; z < dimension; ++z)
		for (int x = 0; x < dimension; ++x)
		{
			int index = z * dimension + x;

			vertices[index].p.y = heights[index] * heightFactor;
		}
	delete[] heights;
	

	InitIndices();
	UpdateNormal();
	InitBuffers();
	InitQuadTree();
}


void Terrain::UpdateDimension(UINT dim)
{
	dimension = dim;
	InitVertices();
	InitIndices();
	UpdateNormal();
	InitBuffers();
	InitQuadTree();
}

void Terrain::InitVertices()
{
	numTile = dimension - 1;
	vertexCount = dimension * dimension;
	vertexStride = sizeof(VertexType);
	size.x = numTile;
	size.z = numTile;

	vertices.resize(vertexCount);
	surfaceVertices.resize(vertexCount);

	for (int z = 0; z < dimension; ++z)
		for (int x = 0; x < dimension; ++x)
		{
			int index = z * dimension + x;
			vertices[index].p = D3DXVECTOR3(x, 0, z);
			vertices[index].n = D3DXVECTOR3(0, 1, 0);
			vertices[index].t = D3DXVECTOR2(x, z) * tiling;
			vertices[index].t1 = D3DXVECTOR2(x, z) * tiling / (float)(dimension - 1);

			surfaceVertices[index] = vertices[index].p;
		}
}

void Terrain::InitIndices()
{
	indexCount = numTile * numTile * 3 * 2;
	indices.reserve(indexCount);
	usedIndices.resize(indexCount);

	for (int z = 0; z < numTile; ++z)
		for (int x = 0; x < numTile; ++x)
		{
			// 1-3
			// |\|
			// 0-2
			int _0 = (z + 0) * dimension + x + 0;
			int _1 = (z + 1) * dimension + x + 0;
			int _2 = (z + 0) * dimension + x + 1;
			int _3 = (z + 1) * dimension + x + 1;

			indices.push_back(_0);
			indices.push_back(_1);
			indices.push_back(_2);
			indices.push_back(_2);
			indices.push_back(_1);
			indices.push_back(_3);
		}
}

void Terrain::InitBuffers()
{
	SAFE_RELEASE(vb);
	SAFE_RELEASE(ib);

	DX::GetDevice()->CreateVertexBuffer(vertexCount * vertexStride,
		D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vb, NULL);

	UpdateVB();

	DX::GetDevice()->CreateIndexBuffer(indexCount * sizeof(UINT),
		D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &ib, NULL);
	
	LPVOID pIndex;
	ib->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, (LPVOID*)&indices[0], indexCount * sizeof(UINT));
	ib->Unlock();
	
}

void Terrain::UpdateVB()
{
	LPVOID pV = NULL;
	vb->Lock(0, 0, &pV, D3DLOCK_DISCARD);
	memcpy(pV, &vertices[0], vertexCount * vertexStride);
	vb->Unlock();
}

void Terrain::UpdateIB()
{
	LPVOID pI;
	ib->Lock(0, 0, &pI, D3DLOCK_DISCARD);

	indexCount = quadTree->GenerateIndex(pI, &surfaceVertices[0], Frustum::Get(), lodRatio) * 3;
	CopyMemory(&usedIndices[0], pI, indexCount * sizeof(UINT));

	ib->Unlock();
}

void Terrain::UpdateNormal()
{
	for (int z = 1; z < numTile; ++z)
	{
		for (int x = 1; x < numTile; ++x)
		{
			// --u--
			// |\|\|
			// l-n-r
			// |\|\|
			// --d--
			int index = z * dimension + x;

			int l = index - 1;
			int r = index + 1;
			int u = index + dimension;
			int d = index - dimension;

			D3DXVECTOR3 du = vertices[u].p - vertices[d].p;
			D3DXVECTOR3 lr = vertices[r].p - vertices[l].p;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);

			vertices[index].n = n;
		}
	}
}

void Terrain::ApplyHeight(vector<float>& heights)
{
	for (int z = 0; z < dimension; ++z)
		for (int x = 0; x < dimension; ++x)
		{
			int index = z * dimension + x;
			
			vertices[index].p.y = heights[index] * heightFactor;
		}
}

void Terrain::InitQuadTree()
{
	SAFE_DELETE(quadTree);
	quadTree = new QuadTree(dimension);
	quadTree->Build(&surfaceVertices[0]);

}


void Terrain::UpdateEditedVertexNormal()
{
	for (int i = 0; i < editedIndices.size(); i++)
	{
		int index = editedIndices[i];
		
		int l = index - 1;
		if (l < 0 || l >= vertices.size()) continue;
		int r = index + 1;
		if (r < 0 || r >= vertices.size()) continue;
		int u = index + dimension;
		if (u < 0 || u >= vertices.size()) continue;
		int d = index - dimension;
		if (d < 0 || d >= vertices.size()) continue;
		
		D3DXVECTOR3 du = vertices[u].p - vertices[d].p;
		D3DXVECTOR3 lr = vertices[r].p - vertices[l].p;
		D3DXVECTOR3 n;
		D3DXVec3Cross(&n, &du, &lr);
		D3DXVec3Normalize(&n, &n);

		vertices[index].n = n;
	}
}

void Terrain::SaveData(wstring fullpath)
{
	Path::CreateFolder(Path::GetDirectoryName(fullpath));

	BinaryWriter* writer = new BinaryWriter();
	writer->Open(fullpath);
	
	vector<float> heights;
	heights.resize(vertices.size());

	for (int i = 0; i < vertices.size(); i++)
	{
		heights[i] = vertices[i].p.y;
	}
	
	writer->UInt(dimension);
	writer->Byte(&heights[0], heights.size() * sizeof(float));
	
	writer->Close();
	SAFE_DELETE(writer);
}

void Terrain::OpenData(wstring fullpath)
{
	BinaryReader* reader = new BinaryReader();
	reader->Open(fullpath);

	dimension = reader->UInt();
	UINT vertexCount = dimension * dimension;

	vector<float> heights;
	heights.resize(vertexCount);
	LPVOID ptr = (LPVOID)&heights[0];
	reader->Byte(&ptr, vertexCount * sizeof(float));

	reader->Close();
	SAFE_DELETE(reader);
	
	InitVertices();
	ApplyHeight(heights);
	InitIndices();
	UpdateNormal();
	InitBuffers();
	InitQuadTree();
	
}
