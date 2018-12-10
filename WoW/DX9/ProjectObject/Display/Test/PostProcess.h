#pragma once
#include "Graphics/Interface/UI/IUIObject.h"

class PostProcess : public IDisplayObject, public IUIButtonDelegate
{
	struct ScreenVtx
	{
		float x, y, z, rhw;
		float tu, tv;

		const static D3DVERTEXELEMENT9 Decl[3];
	};


	struct RTChain
	{
		int rtIndex;
		IDirect3DTexture9* pRenderTarget[2];
		bool bFirst;

		RTChain() :rtIndex(0) {};
		
		void Init(LPDIRECT3DTEXTURE9* pRT)
		{
			for (int i = 0; i < 2; ++i)
			{
				pRenderTarget[i] = pRT[i];
			}
		}

		void Cleanup()
		{
			SAFE_RELEASE(pRenderTarget[0]);
			SAFE_RELEASE(pRenderTarget[1]);
		}

		void Flip()
		{
			rtIndex = 1 - rtIndex;
		};

		LPDIRECT3DTEXTURE9  GetSource()
		{
			return pRenderTarget[1 - rtIndex];
		}
		LPDIRECT3DTEXTURE9  GetTarget()
		{
			return pRenderTarget[rtIndex];
		}
		
	};
	
private:
	LPD3DXEFFECT m_pEffect;
	vector<LPD3DXEFFECT> m_pEffectList;
	
	LPD3DXEFFECT m_pLightingEffect;
	
	list<LPD3DXEFFECT> m_pPPList;
	IDirect3DVertexBuffer9* m_pScreenVB;
	IDirect3DVertexDeclaration9* m_pScreenVtxDecl;
	
	RTChain m_rtChain;
	LPDIRECT3DTEXTURE9 m_pColorTex;
	LPDIRECT3DSURFACE9 m_pColorSurf;
	LPDIRECT3DTEXTURE9 m_pNormalTex;
	LPDIRECT3DSURFACE9 m_pNormalSurf;
	LPDIRECT3DTEXTURE9 m_pPositionTex;
	LPDIRECT3DSURFACE9 m_pPositionSurf;
	LPDIRECT3DTEXTURE9 m_pDepthTex;
	LPDIRECT3DSURFACE9 m_pDepthSurf;

public:
	PostProcess() {};
	~PostProcess();


	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void InitScene();
	void InitUI();

	void RenderSceneToQuad();
	void PerformPP();
	void PerformSinglePP(LPD3DXEFFECT pEffect);
	void RenderFinalQuad();
	void RenderUI();

	// IUIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
};