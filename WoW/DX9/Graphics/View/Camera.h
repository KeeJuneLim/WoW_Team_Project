#pragma once

enum CameraMode
{
	CM_ThirdPerson,
	CM_FirstPerson,
	CM_Free,
	CM_Deactivate
};

class Camera
{

public:
	static Camera*	Get();
	static void		Delete();
private:
	static Camera*	instance;
	Camera();
	~Camera();

	HWND			hWnd;
	VARIABLE(CameraMode, cameraMode, CameraMode);
	VARIABLE_P(D3DXVECTOR3, position, Position);
	D3DXVECTOR3	focus;
	D3DXVECTOR3	upDirection;
	D3DXVECTOR3 selfPosition;

	VARIABLE_P(D3DXMATRIX, viewMatrix, ViewMatrix);
	VARIABLE_P(D3DXMATRIX, projectionMatrix, ProjectionMatrix);
	VARIABLE_P(D3DXMATRIX, vpMatrix, VPMatrix);
	VARIABLE_P(D3DXVECTOR3, offset, Offset);
	VARIABLE_P(D3DXVECTOR3, rotation, Rotation);

	VARIABLE_P(D3DXVECTOR3, currentRotation, CurrentRotation);
	VARIABLE_P(D3DXVECTOR3, currentOffset, CurrentOffset);
	VARIABLE_P(D3DXVECTOR3, currentPosition, CurrentPosition);
	
	VARIABLE(D3DXVECTOR3*, targetPosition, TargetPosition);
	VARIABLE_P(D3DXVECTOR3, targetOffset, TargetOffset);
	VARIABLE(float, fovY, FovY);
	VARIABLE_P(D3DXVECTOR3, forward, Forward);
	VARIABLE_P(D3DXVECTOR3, currForward, CurrForward);
	VARIABLE_P(D3DXMATRIX, matR, MatR);
	VARIABLE_P(D3DXMATRIX, currMatR, CurrMatR);


public:

	void Init(HWND handle);
	void Reset();
	void Update();
	void UpdateRotation();
	void UpdateMatrix();

	void UpdateThirdPerson();
	void UpdateFirstPerson();
	void UpdateFree();
	
};

