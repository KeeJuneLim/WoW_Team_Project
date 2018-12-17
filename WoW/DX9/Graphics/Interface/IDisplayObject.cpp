#include "stdafx.h"
#include "IDisplayObject.h"

IDisplayObject::IDisplayObject()
{
	activated = true;

	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);

	D3DXMatrixIdentity(&localMatrix);
	D3DXMatrixIdentity(&worldMatrix);
	
	moveSpeed = 3.0f;
	rotationSpeed = 5.0f;
	jumpPower = 18.0f;

	tag = TAG_DEFAULT;
	shaderType = ST_None;

	isCastShadow = false;
	isReceiveShadow = false;
}


IDisplayObject::~IDisplayObject()
{
	SAFE_RELEASE(vb);
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vertexDecl);

	SAFE_DELETE(boundary);
	SAFE_DELETE(material);

	for (auto child : children)
		SAFE_DELETE(child);
}

void IDisplayObject::DrawPrimitive()
{
	DX::GetDevice()->SetVertexDeclaration(vertexDecl);
	DX::GetDevice()->SetStreamSource(0, vb, 0, vertexStride);
	DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexCount / 3);
}

void IDisplayObject::DrawIndexedPrimitive()
{
	DX::GetDevice()->SetVertexDeclaration(vertexDecl);
	DX::GetDevice()->SetStreamSource(0, vb, 0, vertexStride);
	DX::GetDevice()->SetIndices(ib);
	DX::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0, vertexCount, 0, indexCount / 3);
}

void IDisplayObject::AddChild(IDisplayObject * pChild)
{
	if (pChild == NULL) return;

	children.push_back(pChild);
	pChild->parent = this;
}

IDisplayObject* IDisplayObject::FindChildByName(wstring _name)
{
	if (name == _name)
		return this;

	for (auto p : children)
	{
		IDisplayObject* pChild = p->FindChildByName(_name);
		if (pChild)	return pChild;
	}
	return NULL;
}

void IDisplayObject::SetRootWithChildren(IDisplayObject * pObj)
{
	root = pObj;

	for (auto p : children)
		p->SetRootWithChildren(pObj);
}

void IDisplayObject::SetShaderTypeWithChildren(ShaderType type)
{
	shaderType = type;

	for (auto p : children)
		p->SetShaderTypeWithChildren(type);
}

void IDisplayObject::UpdateUI()
{
	ImGui::Begin(String::ToString(name).c_str());
	ImGui::SliderFloat3("Position", (float*)&position, -10, 10);
	ImGui::SliderFloat3("Rotation", (float*)&rotation, -D3DX_PI, D3DX_PI);
	ImGui::SliderFloat3("Scale", (float*)&scale, 0.1f, 10);
	ImGui::Text("Forward %.2f %.2f %.2f", forward.x, forward.y, forward.z);
	ImGui::SliderFloat3("Velocity", (float*)&velocity, -100.0f, 100.0f);
	ImGui::End();
	
}

void IDisplayObject::UpdateLocalMatrix()
{
	D3DXMATRIX matS, matR, matT;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&matR, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	localMatrix = matS * matR * matT;
}

void IDisplayObject::UpdateWorldMatrix()
{
	if (parent)
	{
		worldMatrix = localMatrix * *parent->GetWorldMatrix();
	}
	else
	{
		worldMatrix = localMatrix;
	}
}

void IDisplayObject::UpdateInput()
{
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
/*
	if (Input::KeyPress('D'))
	{
		if(Input::ButtonPress(Input::RBUTTON))
			deltaTransform.x = 1;
		else
			deltaTransform.y = 1;
	}
	else if (Input::KeyPress('A'))
	{
		if (Input::ButtonPress(Input::RBUTTON))
			deltaTransform.x = -1;
		else
			deltaTransform.y = -1;
	}
	else
		deltaTransform.y = 0;
*/
	if (Input::KeyDown(VK_SPACE))
	{
		velocity.y = jumpPower;
	}

	
	//if (boundary)
	//{
	//	Ray ray = Ray::RayFromScreenToWorld(Input::GetMousePosition().x, Input::GetMousePosition().y);
	//	boundary->isPicked = boundary->IntersectRay(ray);
	//}
}

void IDisplayObject::ApplyInput()
{
	rotation.y += deltaTransform.y * rotationSpeed * Time::Get()->GetDeltaTime();

	D3DXVECTOR3 _forward;
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, rotation.y);
	D3DXVec3TransformNormal(&_forward,
		&D3DXVECTOR3(deltaTransform.x, 0, deltaTransform.z),
		&matR);
	D3DXVec3Normalize(&_forward, &_forward);

	targetPosition = position + _forward * moveSpeed * Time::Get()->GetDeltaTime();
}

void IDisplayObject::ApplyVelocity()
{
	targetPosition += velocity * Time::Get()->GetDeltaTime();

	velocity.y -= 9.8f * 5.0f * Time::Get()->GetDeltaTime();
}
