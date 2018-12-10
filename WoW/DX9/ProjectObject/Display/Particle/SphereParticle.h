#pragma once
#include "stdafx.h"


class SphereParticle : public IDisplayObject
{

	struct PARTICLE_VERTEX
	{
		D3DXVECTOR3 position;
		D3DCOLOR _color;
	};

	LPDIRECT3DVERTEXDECLARATION9 m_pParticlesVertexDeclaration = NULL;

	struct Attribute
	{
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _velocity;
		D3DXCOLOR   _color;
	};

	size_t  _numParticle;
	LPDIRECT3DTEXTURE9      _pTex;
	LPDIRECT3DVERTEXBUFFER9 _pVB;
	vector<Attribute*>		_particles;

	ID3DXEffect* m_pEffect;

public:
	
	SphereParticle()
	{
		shaderType = ST_Particle;
		Rendering::Get()->AddRenderingObject(this);
	}

	~SphereParticle()
	{
		Rendering::Get()->RemoveRenderingObject(this);
		SAFE_RELEASE(_pVB);
		SAFE_RELEASE(m_pParticlesVertexDeclaration);
		for (auto p : _particles)
			SAFE_DELETE(p);
	}

	void Init()
	{
		_numParticle = 500;
		_particles.resize(_numParticle);
		for (int i = 0; i < _numParticle; i++)
		{
			Attribute* att = new Attribute();
			att->_position = D3DXVECTOR3(0, 0, 0);
			att->_velocity.x = GetRandomFloat(-0.04f, 0.04f);
			att->_velocity.y = GetRandomFloat(-0.04f, 0.04f);
			att->_velocity.z = GetRandomFloat(-0.04f, 0.04f);
			D3DXVec3Normalize(&att->_velocity, &att->_velocity);
			att->_color = D3DXCOLOR(1, 0.3, 0.3, 1);
			
			_particles[i] = att;
		}

		DX::GetDevice()->CreateVertexBuffer(_numParticle * sizeof(PARTICLE_VERTEX),
			D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			0, D3DPOOL_DEFAULT, &_pVB, 0);

		_pTex = Assets::GetTexture(_T("../../_assets/textures/particle/snow.png"));
		//_pTex = Assets::GetTexture(_T("../../_assets/textures/ham.png"));

		m_pEffect = Shaders::Get()->GetEffect(SHADER_PATH + _T("Particle.fx"));

		D3DVERTEXELEMENT9 particle_decl[] =
		{
			{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0,  12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			D3DDECL_END()
		};

		// the type of data passed to the vertex shader is created
		DX::GetDevice()->CreateVertexDeclaration(particle_decl, &m_pParticlesVertexDeclaration);

	}

	void Update()
	{
		static float rad = 1;
		rad += 0.008f;

		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, rad * 3);
		for (size_t i = 0; i < _particles.size(); i++)
		{
			Attribute* att = _particles[i];
			att->_position = att->_velocity * cosf(rad) * 10.0f;
			D3DXVec3TransformCoord(&att->_position, &att->_position, &matR);
			att->_color = D3DXCOLOR(cosf(rad) + 1, att->_color.g, att->_color.b, cosf(rad) * 0.5f + 0.5f);
		}
		

		PARTICLE_VERTEX* v;
		_pVB->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD);
		for (auto p : _particles)
		{
			v->position = p->_position;
			v->_color = p->_color;

			v++;
		}
		_pVB->Unlock();

	}

	void Render()
	{
		//pre
		DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
		DX::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
		
		/*
		DX::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		DX::GetDevice()->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.6f));
		DX::GetDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));
		
		DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
		DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
		DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));
		*/
		//DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		//DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		//DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//render
		DX::GetDevice()->SetVertexDeclaration(m_pParticlesVertexDeclaration);
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);

		D3DXMATRIXA16 matVP;
		matVP = *Camera::Get()->GetViewMatrix() * *Camera::Get()->GetProjectionMatrix();
		m_pEffect->SetMatrix("gVPMatrix", &matVP);
		m_pEffect->SetTexture("DiffuseMap_Tex", _pTex);
		
		D3DXVECTOR4 camPos(*Camera::Get()->GetPosition(), 1);
		m_pEffect->SetVector("gWorldCamPos", &camPos);
		
		static float angle = 0;
		angle += 0.06f;
		m_pEffect->SetFloat("gAngle", angle);

		m_pEffect->CommitChanges();
		
		DX::GetDevice()->SetStreamSource(0, _pVB, 0, sizeof(PARTICLE_VERTEX));
		DX::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, _particles.size());

		m_pEffect->EndPass();
		m_pEffect->End();


		//post
		DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
		DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		DX::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
		//DX::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	}

	void Draw() {}
	
};
