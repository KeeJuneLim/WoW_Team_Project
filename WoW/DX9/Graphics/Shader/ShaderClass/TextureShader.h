#pragma once

class TextureShader : public IShader
{	
public:
	TextureShader();
	~TextureShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

