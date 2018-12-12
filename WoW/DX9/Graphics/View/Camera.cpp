#include "stdAfx.h"
#include "Camera.h"

Camera* Camera::instance = NULL;

Camera * Camera::Get()
{
	if (instance == NULL)
		instance = new Camera();

	return instance;
}

void Camera::Delete()
{
	SAFE_DELETE(instance);
}

Camera::Camera()
{
	
}

Camera::~Camera()
{
}

void Camera::Init(HWND handle)
{
	hWnd = handle;

	Reset();
}

void Camera::Reset()
{
	offset = D3DXVECTOR3(0, 1, -2);
	position = offset;
	rotation = D3DXVECTOR3(0, 0, 0);
	focus = D3DXVECTOR3(0, 0, 0);
	upDirection = D3DXVECTOR3(0, 1, 0);
	forward = D3DXVECTOR3(0, -1, 1);

	currentPosition = position;
	currentRotation = rotation;
	currForward = forward;
	fovY = D3DX_PI * 0.25f;

	cameraMode = CameraMode::CM_FirstPerson;
	targetPosition = NULL;
}

void Camera::Update()
{
	switch (cameraMode)
	{
	case(CameraMode::CM_ThirdPerson):
		UpdateThirdPerson();
		break;
	case(CameraMode::CM_FirstPerson):
		UpdateFirstPerson();
		break;
	case(CameraMode::CM_Free):
		UpdateFree();
		break;
	}
	UpdateMatrix();

	if (GUI::Get()->showCamera)
	{
		ImGui::Begin(GUI::CameraStr);
		ImGui::Text("Position %.f %.f %.f", position.x, position.y, position.z);
		ImGui::Text("Rotation %.f %.f %.f", rotation.x, rotation.y, rotation.z);
		ImGui::SliderInt("Mode", (int*)&cameraMode, 0, CM_Deactivate);
		ImGui::SliderFloat("FovY", (float*)&fovY, D3DX_PI * 0.15f, D3DX_PI * 0.35f);
		ImGui::End();
	}
	
}


void Camera::UpdateRotation()
{
	rotation.x += (Input::GetMouseDeltaPosition().y) / 100.f;
	rotation.y += (Input::GetMouseDeltaPosition().x) / 100.f;

	//X축 회전 제한
	if (rotation.x <= -D3DX_PI * 0.4f)
	{
		rotation.x = -D3DX_PI * 0.4f;
	}
	if (rotation.x >= D3DX_PI * 0.35f)
	{
		rotation.x = D3DX_PI * 0.35f;
	}
}

void Camera::UpdateMatrix()
{
	D3DXMatrixLookAtLH(&viewMatrix, &position, &focus, &upDirection);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);

	RECT rect;
	GetClientRect(hWnd, &rect);
	float fAspectRatio = rect.right / (float)rect.bottom;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fovY, fAspectRatio, 0.1f, 1000.0f);
	DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	vpMatrix = viewMatrix * projectionMatrix;
}


void Camera::UpdateThirdPerson()
{
	if (Input::Get()->ButtonPress(Input::LBUTTON) )//|| Input::Get()->ButtonPress(Input::RBUTTON))
	{
		UpdateRotation();
	}


	D3DXVec3Lerp(&currentRotation, &currentRotation, &rotation, 8.0f * Time::Get()->GetDeltaTime());

	offset.z += Input::GetMouseDeltaPosition().z / 25.0f;

	D3DXMATRIX matRotX, matRotY, matR;
	D3DXMatrixRotationX(&matRotX, rotation.x);
	D3DXMatrixRotationY(&matRotY, rotation.y);
	matR = matRotX * matRotY;

	D3DXMatrixRotationX(&matRotX, currentRotation.x);
	D3DXMatrixRotationY(&matRotY, currentRotation.y);
	currMatR = matRotX * matRotY;
	
	D3DXVECTOR3 _position = offset;
	D3DXVec3TransformCoord(&_position, &_position, &currMatR);
	
	focus = D3DXVECTOR3(0, 0, 0);

	if (targetPosition)
	{
		_position += *targetPosition;
		focus = *targetPosition + targetOffset;
	}

	position = _position;
}

void Camera::UpdateFirstPerson()
{
	//if (Input::Get()->ButtonPress(Input::RBUTTON) )
	//{
	//	UpdateRotation();
	//}
	//

	D3DXVec3Lerp(&currentRotation, &currentRotation, &rotation, 8.0f * Time::Get()->GetDeltaTime());

	offset.z += Input::GetMouseDeltaPosition().z / 25.0f;

	D3DXMATRIX matRotX, matRotY, matR;
	D3DXMatrixRotationX(&matRotX, rotation.x);
	D3DXMatrixRotationY(&matRotY, rotation.y);
	matR = matRotX * matRotY;

	D3DXMatrixRotationX(&matRotX, currentRotation.x);
	D3DXMatrixRotationY(&matRotY, currentRotation.y);
	currMatR = matRotX * matRotY;

	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &currMatR);
	
	D3DXVECTOR3 _right(forward.z, 0, -forward.x);
	D3DXVec3Normalize(&_right, &_right);

	D3DXVECTOR3 _position;
	_position = forward * offset.z;
	_position += _right * offset.x;
	_position.y += offset.y;

	if (targetPosition)
	{
		_position += *targetPosition;
	}

	focus = _position + forward;
	position = _position;
	
}


void Camera::UpdateFree()
{
	if (Input::Get()->ButtonPress(Input::LBUTTON))// ||
		//Input::Get()->ButtonPress(Input::RBUTTON))
	{
		UpdateRotation();
	}

	D3DXVec3Lerp(&currentRotation,
		&currentRotation, &rotation,
		8.0f * Time::Get()->GetDeltaTime());

	offset.z += Input::GetMouseDeltaPosition().z / 25.0f;

	D3DXVECTOR3 _position = offset;

	D3DXMATRIX matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, rotation.x);
	D3DXMatrixRotationY(&matRotY, rotation.y);
	matR = matRotX * matRotY;

	D3DXMatrixRotationX(&matRotX, currentRotation.x);
	D3DXMatrixRotationY(&matRotY, currentRotation.y);
	currMatR = matRotX * matRotY;

	D3DXVec3TransformCoord(&_position, &_position, &currMatR);

	focus = D3DXVECTOR3(0, 0, 0);

	D3DXVECTOR3 deltaTransform;
	if (Input::KeyPress('W'))
		deltaTransform.z = 1;
	else if (Input::KeyPress('S'))
		deltaTransform.z = -1;
	else
		deltaTransform.z = 0;

	if (Input::KeyPress('D'))
		deltaTransform.x = 1;
	else if (Input::KeyPress('A'))
		deltaTransform.x = -1;
	else
		deltaTransform.x = 0;

	if (Input::KeyPress('E'))
		deltaTransform.y = 1;
	else if (Input::KeyPress('Q'))
		deltaTransform.y = -1;
	else
		deltaTransform.y = 0;

	float moveSpeed = 10.0f;

	D3DXVec3TransformNormal(&forward,
		&D3DXVECTOR3(deltaTransform.x, 0, deltaTransform.z), &matR);
	forward.y = deltaTransform.y;
	D3DXVec3Normalize(&forward, &forward);

	selfPosition = selfPosition + forward * moveSpeed *
		Time::Get()->GetDeltaTime();

	_position += selfPosition;
	focus = selfPosition + targetOffset;

	position = _position;
	

}

