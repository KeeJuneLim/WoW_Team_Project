#pragma once

class SkyGradientShader : public IShader
{	
public:
	SkyGradientShader();
	~SkyGradientShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

