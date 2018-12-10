#pragma once

class IUIObject : public IDisplayObject
{
protected:
	LPD3DXSPRITE m_pSprite;
	VARIABLE_P(D3DXVECTOR3, m_combinedPos, CombinedPosition);
	VARIABLE_P(D3DXVECTOR3, m_pivot, Pivot);
	VARIABLE_P(D3DXVECTOR2, m_size, Size);
	VARIABLE_P(D3DCOLOR, m_color, Color);
	bool		m_bDrawBorder;
public:

	IUIObject(LPD3DXSPRITE pSprite);
	virtual ~IUIObject();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override {};

	void UpdateCombinedPosition();
	void DrawBorder();
	void GetBorderRect(RECT *rect);
};

#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"


