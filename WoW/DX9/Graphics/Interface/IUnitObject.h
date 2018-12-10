#pragma once

class IUnitObject : public IDisplayObject
{
protected:
	D3DXVECTOR3	m_deltaPos;
	D3DXVECTOR3	m_deltaRot;
	D3DXVECTOR3	m_forward;
	D3DXVECTOR3	m_baseRotation;

	bool		m_isMoving;
	float		m_moveSpeed;
	float		m_currMoveSpeedRate;
	float		m_rotationSpeed;

	bool		m_isJumping;
	float		m_jumpPower;
	float		m_gravity;
	float		m_currGravity;
	int			m_jumpCnt;

	float		m_maxStepHeight;

	D3DXVECTOR3	m_destPos;
	D3DXVECTOR3	m_finalDestPos;
	vector<int>	m_vecAStarIndex;

	IUnitObject();
	virtual ~IUnitObject();
public:
	
	void SetDestination(const D3DXVECTOR3 &pos);
	void UpdateInput();
	void UpdatePositionToDestination();
	void UpdateTargetPosition(OUT D3DXVECTOR3 &targetPos);
	void ApplyTargetPosition(D3DXVECTOR3 &targetPos);

	D3DXVECTOR3* GetForward() { return &m_forward; }
	void SetForward(D3DXVECTOR3 *forward) { m_forward = *forward; }
	
	void UpdatePosition();
	void StartFall();
	void StartJump();
	void FinishJump();
	
};

