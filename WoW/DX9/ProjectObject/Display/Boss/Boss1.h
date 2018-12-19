#pragma once

enum Boss1Status
{
	Boss1_Stand,
	Boss1_Run,
	Boss1_JumpStart,
	Boss1_Jumping,
	Boss1_JumpEnd,
	Boss1_Status_End
};


class Boss1 : public IDisplayObject
{
public:
	Boss1();
	~Boss1();

	void Init();
	void Update();
	void Render();

	void Delete();

	void UpdateInput();
	void ApplyMap();

private:
	IDisplayObject * face;
	IDisplayObject * body;
	IDisplayObject * tail;

	IDisplayObject * weaponL;
	IDisplayObject * weaponR;

	IDisplayObject * lance;
	IDisplayObject* fireCube;

	//IDisplayObject* cowJumpStart;
	//IDisplayObject* cowJumping;
	//IDisplayObject* cowJumpEnd;

	vector<IDisplayObject*> cowMotion;

	IDisplayObject* skull;

	Boss1Status status;
	D3DXVECTOR3 forward;
	D3DXMATRIX matR;
	bool bJumping;
	bool bMoving;
	// - start // 2-ing // 3-end

	//Boundary* boundary;

	// IDisplayObject을(를) 통해 상속됨
	virtual void Draw() override;

};