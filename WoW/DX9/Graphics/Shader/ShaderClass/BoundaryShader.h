#pragma once

class Boundary;

class BoundaryShader : public IShader
{	
public:
	BoundaryShader();
	~BoundaryShader();

	// IShader을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj);
	virtual void PostRender() override;
	virtual void Render(Boundary* pObj) override;
};

