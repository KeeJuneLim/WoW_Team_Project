#include "stdafx.h"
#include "AseModelParts.h"
#include "AseModel.h"

AseModelParts::AseModelParts()
{
	material = new Material();
}


AseModelParts::~AseModelParts()
{
	
}

void AseModelParts::Init()
{
	InitEachData();
	InitLocalMatrix();
}

void AseModelParts::Update()
{
	D3DXMATRIXA16 matLocalT, matLocalR;
	UINT tickCount = model->GetCurrentTick();

	UpdateLocalTranslationMatrix(tickCount, matLocalT);
	UpdateLocalRotationMatrix(tickCount, matLocalR);

	localMatrix = matLocalR * matLocalT;

	IDisplayObject::UpdateWorldMatrix();

	for (auto p : children)
	{
		p->Update();
	}

	if (boundary)
	{
		boundary->UpdateTransform(worldMatrix);

		Ray ray = Ray::RayFromScreenToWorld(Input::GetMousePosition().x, Input::GetMousePosition().y);
		boundary->isPicked = boundary->IntersectRay(ray);
	}
}

void AseModelParts::Render()
{
	
}

void AseModelParts::Render(IShader * pShader)
{
	IDirect3DTexture9* tex = NULL;
	tex = material->GetDiffuseMapTex();

	if (tex)
	{
		//D3DXMatrixIdentity(&worldMatrix);
		pShader->Render(this);

		if (boundary)
		{
			//boundary->Render();
		}
	}

	for (auto p : children)
	{
		p->Render(pShader);
	}
}

void AseModelParts::Draw()
{
	DrawPrimitive();
}



void AseModelParts::SetModel(AseModel * pModel)
{
	model = pModel;
	for (auto p : children)
	{
		((AseModelParts*)p)->SetModel(model);
	}
}

void AseModelParts::InitEachData()
{
	IDirect3DTexture9* tex = NULL;
	tex = material->GetDiffuseMapTex();

	if (tex)
	{
		vertexCount = vertices.size();
		vertexStride = sizeof(VertexType);

		DXUtil::CreateVertexBuffer(vb, &vertices[0],
			0, vertexCount, vertexStride);

		D3DVERTEXELEMENT9 vertexElem[MAX_FVF_DECL_SIZE] = { 0 };
		D3DXDeclaratorFromFVF(VertexType::FVF, vertexElem);

		DX::GetDevice()->CreateVertexDeclaration(vertexElem, &vertexDecl);
	}
	
	D3DXVECTOR3 minPoint, maxPoint;
	DXUtil::ComputeBoundingBox((BYTE*)&vertices[0], vertices.size(),
		sizeof(VertexType), minPoint, maxPoint);
	boundary = new BoundingBox(minPoint, maxPoint);
	boundary->InitGeometry();
	
	for (auto p : children)
	{
		((AseModelParts*)p)->InitEachData();
	}
}

void AseModelParts::InitLocalMatrix(AseModelParts * pParent)
{
	localMatrix = worldMatrix;

	if (pParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pParent->GetWorldMatrix());
		localMatrix *= matInvParent;
	}

	for (auto p : children)
	{
		((AseModelParts*)p)->InitLocalMatrix(this);
	}
}


void AseModelParts::UpdateLocalTranslationMatrix(int tickCount, OUT D3DXMATRIXA16 & mat)
{
	D3DXMatrixIdentity(&mat);
	if (m_vecPosTrack.empty())	//Track 정보 없을 때 Local 로 채워줌
	{
		mat._41 = localMatrix._41;
		mat._42 = localMatrix._42;
		mat._43 = localMatrix._43;
	}
  else if (tickCount <= m_vecPosTrack.front().tick)
  {
    mat._41 = m_vecPosTrack.front().position.x;
    mat._42 = m_vecPosTrack.front().position.y;
    mat._43 = m_vecPosTrack.front().position.z;
  }
  else if (tickCount >= m_vecPosTrack.back().tick)
  {
    mat._41 = m_vecPosTrack.back().position.x;
    mat._42 = m_vecPosTrack.back().position.y;
    mat._43 = m_vecPosTrack.back().position.z;
  }
  else
  {
    int nNext = -1;
    for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
    {
      if (m_vecPosTrack[i].tick > tickCount)
      {
        nNext = i;
        break;
      }
    }
    int nPrev = nNext - 1;

    float t = (tickCount - m_vecPosTrack[nPrev].tick) /
      (float)(m_vecPosTrack[nNext].tick - m_vecPosTrack[nPrev].tick);

    D3DXVECTOR3 v;
    D3DXVec3Lerp(&v, &m_vecPosTrack[nPrev].position, &m_vecPosTrack[nNext].position, t);
    mat._41 = v.x;
    mat._42 = v.y;
    mat._43 = v.z;
  }
}

void AseModelParts::UpdateLocalRotationMatrix(int tickCount, OUT D3DXMATRIXA16 & mat)
{
  D3DXMatrixIdentity(&mat);
  if (m_vecRotTrack.empty())	//Track 정보 없을 때 Local 로 채워줌
  {
    mat = localMatrix;
    mat._41 = 0.0f;
    mat._42 = 0.0f;
    mat._43 = 0.0f;
  }
  else if (tickCount <= m_vecRotTrack.front().tick)
  {
    D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().quaternion);
  }
  else if (tickCount >= m_vecRotTrack.back().tick)
  {
    D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().quaternion);
  }
  else
  {
    int nNext = -1;
    for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
    {
      if (m_vecRotTrack[i].tick > tickCount)
      {
        nNext = i;
        break;
      }
    }
    int nPrev = nNext - 1;

    float t = (tickCount - m_vecRotTrack[nPrev].tick) /
      (float)(m_vecRotTrack[nNext].tick - m_vecRotTrack[nPrev].tick);
	
    D3DXQUATERNION q;
    D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrev].quaternion, &m_vecRotTrack[nNext].quaternion, t);
    D3DXMatrixRotationQuaternion(&mat, &q);
  }
}





