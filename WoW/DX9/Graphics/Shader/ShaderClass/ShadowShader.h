#pragma once

class ShadowShader : public IShader
{	
public:
	ShadowShader();
	~ShadowShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

