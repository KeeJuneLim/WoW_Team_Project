#pragma once

class ColorShader : public IShader
{	
public:
	ColorShader();
	~ColorShader();

	// IShader을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

