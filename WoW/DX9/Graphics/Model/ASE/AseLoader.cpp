#include "stdafx.h"
#include "AseLoader.h"
#include "Asciitok.h"
#include "AseModelParts.h"


AseLoader::AseLoader()
{
}


AseLoader::~AseLoader()
{
}

AseModelParts * AseLoader::Load(LPCTSTR filepath, LPCTSTR filename, OUT ASE_SCENE * pScene)
{
	m_filepath = filepath;

	CString fullpath = filepath;
	fullpath.Append(filename);

	m_fin.open(fullpath);
	
	assert(m_fin.is_open() && "m_fin.is_open() == false");

	AseModelParts* m_pRootObj = NULL;

	while (!m_fin.eof() )
	{
		m_fin >> m_szToken;
		
		if (CompareStr(m_szToken, ID_SCENE))
		{
			if (pScene) ProcessSCENE(pScene);
		}
		else if (CompareStr(m_szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (CompareStr(m_szToken, ID_GEOMETRY))
		{
			AseModelParts* pObj = new AseModelParts();
			ProcessGEOMOBJECT(pObj);
			if (m_pRootObj == NULL)
			{
				m_pRootObj = pObj;
			}
		}
	}
  
	m_fin.close();

	return m_pRootObj;
}


int AseLoader::GetTokenAsInt()
{
	m_fin >> m_szToken;
	return atoi(m_szToken);
}

float AseLoader::GetTokenAsFloat()
{
	m_fin >> m_szToken;
	return atof(m_szToken);
}

char * AseLoader::GetTokenNoQuotation()
{
	m_fin.getline(m_szToken, TOKEN_SIZE, '\"');
	m_fin.getline(m_szToken, TOKEN_SIZE, '\"');

	return m_szToken;
}

void AseLoader::ProcessSCENE(OUT ASE_SCENE * pScene)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_FIRSTFRAME))
		{
			pScene->numFirstFrame = GetTokenAsInt();
		}
		else if (CompareStr(m_szToken, ID_LASTFRAME))
		{
			pScene->numLastFrame = GetTokenAsInt();
		}
		else if (CompareStr(m_szToken, ID_FRAMESPEED))
		{
			pScene->FrameSpeed = GetTokenAsInt();
		}
		else if (CompareStr(m_szToken, ID_TICKSPERFRAME))
		{
			pScene->TicksPerFrame = GetTokenAsInt();
		}
	} while (nLevel > 0);

}

void AseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MATERIAL_COUNT))
		{
			int size = GetTokenAsInt();

			m_mtlList.resize(size);
			m_diffuseMapNameList.resize(size);
		}
		else if (CompareStr(m_szToken, ID_MATERIAL))
		{
			ProcessMATERIAL(GetTokenAsInt());
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMATERIAL(int index)
{
	D3DMATERIAL9* mtl = &m_mtlList[index];
	ZeroMemory(mtl, sizeof(D3DMATERIAL9));
	mtl->Power = 20.0f;

	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_AMBIENT))
		{
			mtl->Ambient.r = GetTokenAsFloat();
			mtl->Ambient.g = GetTokenAsFloat();
			mtl->Ambient.b = GetTokenAsFloat();
			mtl->Ambient.a = 1.0f;
		}
		else if (CompareStr(m_szToken, ID_DIFFUSE))
		{
			mtl->Diffuse.r = GetTokenAsFloat();
			mtl->Diffuse.g = GetTokenAsFloat();
			mtl->Diffuse.b = GetTokenAsFloat();
			mtl->Diffuse.a = 1.0f;
		}
		else if (CompareStr(m_szToken, ID_SPECULAR))
		{
			mtl->Specular.r = GetTokenAsFloat();
			mtl->Specular.g = GetTokenAsFloat();
			mtl->Specular.b = GetTokenAsFloat();
			mtl->Specular.a = 1.0f;
		}
		else if (CompareStr(m_szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(index);
		}
    
	} while (nLevel > 0);
}

void AseLoader::ProcessMAP_DIFFUSE(int index)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_BITMAP))
		{
			CString fullpath = m_filepath + CString(GetTokenNoQuotation());
			m_diffuseMapNameList[index] = fullpath;
		}
	} while (nLevel > 0);
}

AseModelParts * AseLoader::ProcessGEOMOBJECT(OUT AseModelParts* &pObj)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_NODE_NAME))
		{
			m_AseModelPartsList[GetTokenNoQuotation()] = pObj;
		}
		else if (CompareStr(m_szToken, ID_NODE_PARENT))
		{
			m_AseModelPartsList[GetTokenNoQuotation()]->AddChild(pObj);
		}
		else if (CompareStr(m_szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(pObj);
		}
		else if (CompareStr(m_szToken, ID_MESH))
		{
			ProcessMESH(pObj);
		}
		else if (CompareStr(m_szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pObj);
		}
		else if (CompareStr(m_szToken, ID_MATERIAL_REF))
		{
			int index = GetTokenAsInt();
			pObj->GetMaterial()->SetMaterial(m_mtlList[index]);
			pObj->GetMaterial()->SetDiffuseMapTex(Assets::GetTexture(m_diffuseMapNameList[index]));
		}
	} while (nLevel > 0);

	return pObj;
}

void AseLoader::ProcessNODE_TM(OUT AseModelParts * pObj)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_TM_ROW0))
		{
			matWorld._11 = GetTokenAsFloat();
			matWorld._13 = GetTokenAsFloat();
			matWorld._12 = GetTokenAsFloat();
			matWorld._14 = 0.0f;
		}
		else if (CompareStr(m_szToken, ID_TM_ROW1))
		{
			matWorld._31 = GetTokenAsFloat();
			matWorld._33 = GetTokenAsFloat();
			matWorld._32 = GetTokenAsFloat();
			matWorld._34 = 0.0f;
		}
		else if (CompareStr(m_szToken, ID_TM_ROW2))
		{
			matWorld._21 = GetTokenAsFloat();
			matWorld._23 = GetTokenAsFloat();
			matWorld._22 = GetTokenAsFloat();
			matWorld._24 = 0.0f;
		}
		else if (CompareStr(m_szToken, ID_TM_ROW3))
		{
			matWorld._41 = GetTokenAsFloat();
			matWorld._43 = GetTokenAsFloat();
			matWorld._42 = GetTokenAsFloat();
			matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);

	pObj->SetWorldMatrix(&matWorld);
}

void AseLoader::ProcessMESH(OUT AseModelParts * pObj)
{
	vector<D3DXVECTOR3>	vecP;
	vector<D3DXVECTOR2>	vecT;
	vector<VERTEX_PNT>	vecVertex;
	
	
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_NUMVERTEX))
		{
			vecP.resize(GetTokenAsInt());
		}
		else if (CompareStr(m_szToken, ID_MESH_NUMFACES))
		{
			vecVertex.resize(3 * GetTokenAsInt());
		}
		else if (CompareStr(m_szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(vecP);
		}
		else if (CompareStr(m_szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(vecP, vecVertex);
		}
		else if (CompareStr(m_szToken, ID_MESH_NUMTVERTEX))
		{
			vecT.resize(GetTokenAsInt());
		}
		else if (CompareStr(m_szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTLIST(vecT);
		}
		else if (CompareStr(m_szToken, ID_MESH_NUMTVFACES))
		{
			
		}
		else if (CompareStr(m_szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACELIST(vecT, vecVertex);
		}
		else if (CompareStr(m_szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(vecVertex);
		}

	} while (nLevel > 0);

	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, pObj->GetWorldMatrix());
	/**/
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}
	
	pObj->SetVertices(&vecVertex);
}

void AseLoader::ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecV)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_VERTEX))
		{
			int index = GetTokenAsInt();
			vecV[index].x = GetTokenAsFloat();
			vecV[index].z = GetTokenAsFloat();
			vecV[index].y = GetTokenAsFloat();
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_FACE_LIST(IN vector<D3DXVECTOR3>& vecV, OUT vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_FACE))
		{
			int faceIndex = GetTokenAsInt();
			m_fin >> m_szToken; // A:
			int nA = GetTokenAsInt();
			m_fin >> m_szToken; // B:
			int nB = GetTokenAsInt();
			m_fin >> m_szToken; // C:
			int nC = GetTokenAsInt();

			vecVertex[faceIndex * 3 + 0].p = vecV[nA];
			vecVertex[faceIndex * 3 + 1].p = vecV[nC];
			vecVertex[faceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>& vecVT)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_TVERT))
		{
			int index = GetTokenAsInt();
			vecVT[index].x = GetTokenAsFloat();
			vecVT[index].y = 1.0f - GetTokenAsFloat();
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_TFACELIST(IN vector<D3DXVECTOR2>& vecVT, OUT vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_TFACE))
		{
			int faceIndex = GetTokenAsInt();
			int nA = GetTokenAsInt();
			int nB = GetTokenAsInt();
			int nC = GetTokenAsInt();

			vecVertex[faceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[faceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[faceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_NORMALS(OUT vector<VERTEX_PNT>& vecVertex)
{
	int faceIndex = 0;
	int vertexCount = 0;
	int aVertexIndex[] = { 0, 2, 1 };

	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_MESH_FACENORMAL))
		{
			faceIndex = GetTokenAsInt();
			vertexCount = 0;
		}
		else if (CompareStr(m_szToken, ID_MESH_VERTEXNORMAL))
		{
			m_fin >> m_szToken;
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.x = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.z = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.y = GetTokenAsFloat();
			vertexCount++;
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessTM_ANIMATION(OUT AseModelParts * pObj)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_POS_TRACK))
		{
			ProcessPOS_TRACK(pObj);
		}
		else if (CompareStr(m_szToken, ID_ROT_TRACK))
		{
			ProcessROT_TRACK(pObj);
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessPOS_TRACK(OUT AseModelParts * pObj)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_POS_SAMPLE))
		{
			ASE_POS_SAMPLE posSample;
			posSample.tick = GetTokenAsInt();
			posSample.position.x = GetTokenAsFloat();
			posSample.position.z = GetTokenAsFloat();
			posSample.position.y = GetTokenAsFloat();

			pObj->GetPosTrack()->push_back(posSample);
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessROT_TRACK(OUT AseModelParts * pObj)
{
	int nLevel = 0;
	do
	{
		m_fin >> m_szToken;
		if (CompareStr(m_szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(m_szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(m_szToken, ID_ROT_SAMPLE))
		{
			ASE_ROT_SAMPLE rotSample;

			rotSample.tick = GetTokenAsInt();

			rotSample.quaternion.x = GetTokenAsFloat();
			rotSample.quaternion.z = GetTokenAsFloat();
			rotSample.quaternion.y = GetTokenAsFloat();
			rotSample.quaternion.w = GetTokenAsFloat();
			
			rotSample.quaternion.x *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.y *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.z *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.w = cosf(rotSample.quaternion.w / 2.0f);

			if (pObj->GetRotTrack()->empty() == false)
			{
				rotSample.quaternion = pObj->GetRotTrack()->back().quaternion * rotSample.quaternion;
			}

			pObj->GetRotTrack()->push_back(rotSample);
		}
	} while (nLevel > 0);
}
