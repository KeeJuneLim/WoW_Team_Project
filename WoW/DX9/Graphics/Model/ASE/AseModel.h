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

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	void Load(LPCTSTR filepath, LPCTSTR filename);

};

