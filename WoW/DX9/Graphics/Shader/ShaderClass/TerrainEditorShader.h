#pragma once

class TerrainEditorShader : public IShader
{
public:
	TerrainEditorShader();
	~TerrainEditorShader();

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;

};

