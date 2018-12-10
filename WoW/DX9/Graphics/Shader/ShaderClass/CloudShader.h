#pragma once

class CloudShader : public IShader
{	
public:
	CloudShader();
	~CloudShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

