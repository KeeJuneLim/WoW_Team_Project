#pragma once

class Boundary;

class BoundaryShader : public IShader
{	
public:
	BoundaryShader();
	~BoundaryShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj);
	virtual void PostRender() override;
	virtual void Render(Boundary* pObj) override;
};

