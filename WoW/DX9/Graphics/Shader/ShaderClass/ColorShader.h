#pragma once

class ColorShader : public IShader
{	
public:
	ColorShader();
	~ColorShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

