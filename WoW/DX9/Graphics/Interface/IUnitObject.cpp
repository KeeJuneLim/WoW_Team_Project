#include "stdafx.h"
#include "IUnitObject.h"

IUnitObject::IUnitObject()
{
	forward = D3DXVECTOR3(0, 0, 1);
	float scale = 5.0f;
	scale = scale * scale;

	m_isMoving = false;
	m_moveSpeed = 0.3f * scale;
	m_currMoveSpeedRate = 1.0f;
	rotationationSpeed = 0.15f;

	m_isJumping = false;
	m_jumpPower = 0.8f * scale;
	m_gravity = 0.05f * scale;
	m_currGravity = 0.0f;
	m_jumpCnt = 0;
	m_maxStepHeight = 0.5f * scale;
}


IUnitObject::~IUnitObject()
{
}

void IUnitObject::SetDestination(const D3DXVECTOR3 & pos)
{
	m_destPos = pos; m_finalDestPos = m_destPos; return;

	m_vecAStarIndex.clear();
	m_destPos = position;
	m_finalDestPos = pos;
}


void IUnitObject::UpdateInput()
{
	if (Input::KeyPress('W')) m_deltaPos.z = 1;
	else if (Input::KeyPress('S')) m_deltaPos.z = -1;
	else m_deltaPos.z = 0;
	/*
	if (Input::Get()->ButtonPress(Input::LBUTTON) && Input::Get()->ButtonPress(Input::RBUTTON))
	{
		m_deltaPos.z = 1;
	}
	*/
	if (Input::Get()->ButtonPress(Input::RBUTTON))
	{
		forward = *Camera::Get()->GetForward();
		forward.y = 0;
		D3DXVec3Normalize(&forward, &forward);
	}
	else
	{
		if (Input::KeyPress('D')) m_deltaRot.y = 1;
		else if (Input::KeyPress('A')) m_deltaRot.y = -1;
		else m_deltaRot.y = 0;
	}

	if (Input::KeyDown(VK_SPACE))
	{
		if ( (m_isJumping == true) && (m_currGravity < m_jumpPower) )
		{
			
		}
		else
		{
			StartJump();
		}
	}

	if (m_deltaPos.z == 0 && m_deltaRot.y == 0 )
		return;
	
	D3DXMATRIXA16 matRY;
	D3DXMatrixRotationY(&matRY, m_deltaRot.y * rotationationSpeed);
	D3DXVec3TransformNormal(&forward, &forward, &matRY);
	
	m_vecAStarIndex.clear();
	m_destPos = (D3DXVECTOR3(position + forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate));
	m_finalDestPos = m_destPos;
}


void IUnitObject::UpdatePositionToDestination()
{
	D3DXVECTOR3 targetPos = position;
	
	UpdateTargetPosition(targetPos);
	ApplyTargetPosition(targetPos);
	
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMATRIXA16 matRY;
	D3DXMatrixLookAtLH(&matRY, &D3DXVECTOR3(0, 0, 0), &forward, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matRY, &matRY);
	D3DXMATRIXA16 matBaseRY;
	D3DXMatrixRotationY(&matBaseRY, m_baseRotation.y);
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	worldMatrix = matS * matRY * matBaseRY * matT;

	if (D3DXVec3LengthSq(&m_deltaPos) > 0 || D3DXVec3LengthSq(&m_deltaRot) > 0)
		m_isMoving = true;
	else
		m_isMoving = false;
}


void IUnitObject::UpdateTargetPosition(OUT D3DXVECTOR3 & targetPos)
{
	D3DXVECTOR3 forward = D3DXVECTOR3(m_destPos.x - position.x, 0, m_destPos.z - position.z);
	D3DXVECTOR3 forwardNormalized = forward;

	if (D3DXVec3LengthSq(&forward) > 0)
	{
		D3DXVec3Normalize(&forwardNormalized, &forwardNormalized);
		forward = forwardNormalized;

		if (m_deltaPos.z == 1)
		{
			m_currMoveSpeedRate = 1.0f;
		}
		else if (m_deltaPos.z == -1)
		{
			forward *= -1;
			m_currMoveSpeedRate = 0.5f;
		}
		else
		{
			m_deltaPos.z = 1;
			m_currMoveSpeedRate = 1.0f;
		}

		if (D3DXVec3Length(&(forward)) >= m_moveSpeed * m_currMoveSpeedRate)
		{
			targetPos = position + forwardNormalized * m_moveSpeed * m_currMoveSpeedRate;
		}
		else
		{
			targetPos.x = m_destPos.x;
			targetPos.z = m_destPos.z;
		}
	}
	else if (m_vecAStarIndex.empty() == false)
	{
		//������ ���� �� Astar ��尡 �������� �� ������ �缳��
		m_destPos = Objects::GetCurrentMap()->GetAStar()->GetNodes()[m_vecAStarIndex.back()]->GetLocation();
		m_vecAStarIndex.pop_back();
	}
	else
	{
		m_destPos = m_finalDestPos;
	}
}

void IUnitObject::ApplyTargetPosition(D3DXVECTOR3 &targetPos)
{
	float height = 0;
	bool isIntersected = true;
	IMap* pCurrMap = Objects::GetCurrentMap();

	//targetPos = position + forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

	if (pCurrMap != NULL)
		isIntersected = pCurrMap->GetHeight(height, targetPos);

	if (m_isJumping == true)
	{
		targetPos.y += m_jumpPower - m_currGravity;
		m_currGravity += m_gravity;

		if (isIntersected == false)
		{
			//targetPos�� �ٴ��� ������ position �� height ���
			pCurrMap->GetHeight(height, position);
			//���� ���� �� �̵� ����. y�� �ݿ�
			targetPos.x = position.x;
			targetPos.z = position.z;
		}

		//�ϰ��߿��� ���� üũ�ؼ� ���� ����
		if (m_jumpPower < m_currGravity && targetPos.y <= height)
		{
			targetPos.y = height;
			FinishJump();
		}
	}
	else //m_isJumping == false
	{
		if (pCurrMap != NULL)
		{
			isIntersected = pCurrMap->GetHeight(height, targetPos);
			//�̵��� ��ġ�� �ٴ��� ���ų� �������� ũ�� �̵� ���´�
			if (isIntersected == false)
			{
				targetPos = position; //���� ���� �� �̵� ����
			}
			else
			{
				//�̵��� ���� ������ �������� Ŭ ��
				//�ϰ��̸� �̵�����, ����̸� �̵� ����
				if (fabs(height - position.y) > m_maxStepHeight)
				{
					if (height < position.y) //�ϰ�
					{
						StartFall();
					}
					else //���
					{
						targetPos = position;
					}
				}
				else
				{
					targetPos.y = height; //�̵� ���Ѿ��� ���� �ݿ�
				}
			}
		}
	}

	position = targetPos;
}

void IUnitObject::UpdatePosition()
{
	if (Input::KeyPress('W')) m_deltaPos.z = 1;
	else if (Input::KeyPress('S')) m_deltaPos.z = -1;
	else m_deltaPos.z = 0;

	if (Input::KeyPress('D')) m_deltaRot.y = 1;
	else if (Input::KeyPress('A')) m_deltaRot.y = -1;
	else m_deltaRot.y = 0;

	if (Input::KeyDown(VK_SPACE) && m_isJumping == false)
		StartJump();

	rotation += m_deltaRot * rotationationSpeed;

	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, rotation.y);
	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &matRotY);

	D3DXVECTOR3 targetPos;
	float height = 0;
	bool isIntersected = true;
	IMap* pCurrMap = Objects::GetCurrentMap();

	targetPos = position + forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

	if (pCurrMap != NULL)
		isIntersected = pCurrMap->GetHeight(height, targetPos);

	if (m_isJumping == true)
	{
		targetPos.y += m_jumpPower - m_currGravity;
		m_currGravity += m_gravity;

		if (isIntersected == false)
		{
			//targetPos�� �ٴ��� ������ position �� height ���
			pCurrMap->GetHeight(height, position);
			//���� ���� �� �̵� ����. y�� �ݿ�
			targetPos.x = position.x;
			targetPos.z = position.z;
		}

		//�ϰ��߿��� ���� üũ�ؼ� ���� ����
		if (m_jumpPower < m_currGravity && targetPos.y <= height)
		{
			targetPos.y = height;
			FinishJump();
		}
	}
	else //m_isJumping == false
	{
		if (pCurrMap != NULL)
		{
			isIntersected = pCurrMap->GetHeight(height, targetPos);
			//�̵��� ��ġ�� �ٴ��� ���ų� �������� ũ�� �̵� ���´�
			if (isIntersected == false)
			{
				targetPos = position; //���� ���� �� �̵� ����
			}
			else
			{
				//�̵��� ���� ������ �������� Ŭ ��
				//�ϰ��̸� �̵�����, ����̸� �̵� ����
				if (fabs(height - position.y) > m_maxStepHeight)
				{
					if (height < position.y) //�ϰ�
					{
						StartFall();
					}
					else //���
					{
						targetPos = position;
					}
				}
				else
				{
					targetPos.y = height; //�̵� ���Ѿ��� ���� �ݿ�
				}
			}
		}
	}

	position = targetPos;

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMATRIXA16 matBaseRY;
	D3DXMatrixRotationY(&matBaseRY, m_baseRotation.y);
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	worldMatrix = matS * matRotY * matBaseRY * matT;
	m_combinedMatrix = worldMatrix;

	if (D3DXVec3LengthSq(&m_deltaRot) > 0 || D3DXVec3LengthSq(&m_deltaPos) > 0)
		m_isMoving = true;
	else
		m_isMoving = false;
}

void IUnitObject::StartFall()
{
	m_isJumping = true;
	m_currMoveSpeedRate = 0.8f;
	m_currGravity = m_jumpPower;
}

void IUnitObject::StartJump()
{
	if (m_jumpCnt > 0) return;

	m_jumpCnt++;
	m_isJumping = true;
	m_currMoveSpeedRate = 0.8f;
	m_currGravity = 0;
}

void IUnitObject::FinishJump()
{
	m_isJumping = false;
	m_currMoveSpeedRate = 1.0f;
	m_currGravity = 0;
	m_jumpCnt = 0;
}