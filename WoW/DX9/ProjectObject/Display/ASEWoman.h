#pragma once

class AseModel;

class ASEWoman : public IDisplayObject
{
private:
	AseModel* currentModel;
	AseModel* standModel;
	AseModel* runModel;

	bool isMoving;

public:
	ASEWoman();
	~ASEWoman();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

};