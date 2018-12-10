#pragma once

class SkyGradientShader : public IShader
{	
public:
	SkyGradientShader();
	~SkyGradientShader();

	// IShader을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

