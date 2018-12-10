#pragma once

enum PlayerStatus
{
	Player_Stand,
	Player_Run,
	Player_JumpStart,
	Player_Jumping,
	Player_JumpEnd,
	Status_End
};
class Weapon;

class Player : public IDisplayObject
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Render();

	void Delete();

private:
	IDisplayObject * face;
	IDisplayObject * body;
	IDisplayObject * tail;

	IDisplayObject * weaponL;
	IDisplayObject * weaponR;

	IDisplayObject * lance;
	Weapon* axe;
	IDisplayObject* fireCube;

	//IDisplayObject* cowJumpStart;
	//IDisplayObject* cowJumping;
	//IDisplayObject* cowJumpEnd;

	vector<IDisplayObject*> cowMotion;

	PlayerStatus status;
	D3DXVECTOR3 forward;
	D3DXMATRIX matR;
	bool bJumping;
	bool bMoving;
	// - start // 2-ing // 3-end

	//Boundary* boundary;


	// IDisplayObject을(를) 통해 상속됨
	virtual void Draw() override;

};