#pragma once

class ToonShader : public IShader
{	
public:
	ToonShader();
	~ToonShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

