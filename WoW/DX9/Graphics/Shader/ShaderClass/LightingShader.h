#pragma once

class LightingShader : public IShader
{	
public:
	LightingShader();
	~LightingShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

