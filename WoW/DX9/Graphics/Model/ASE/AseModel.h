#pragma once
#include "AseModelParts.h"

class AseModel : public IDisplayObject
{
private:
	ASE_SCENE		aseScene;
	AseModelParts*	rootParts;

	UINT firstTick;
	UINT lastTick;
	UINT tickSpeed;
	VARIABLE(UINT, currentTick, CurrentTick);

public:
	AseModel();
	virtual ~AseModel();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	void Load(LPCTSTR filepath, LPCTSTR filename);

};

