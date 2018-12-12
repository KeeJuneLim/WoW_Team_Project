#include "stdafx.h"
#include "Player.h"
#include "Graphics\Model\X\AllocateHierarchyHLSL.h"
#include "ProjectObject\Display\Weapon.h"
#include "ProjectObject\Display\ShaderCube.h"
#include "Graphics\Model\OBJ\ObjMap.h"


Player::Player()
{
	bJumping = false;
	bMoving = false;
	scale = D3DXVECTOR3(.1, .1, .1);
	//
	//face = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Face_Level3I.X");
	//tail = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Tail_Level3I.X");
	//body = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Body_Level3I.X");

	//cowJumpStart = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpStart.X");
	//cowJumping = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumping.X");
	//cowJumpEnd = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpStart.X");

	status = Player_Stand;

	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowStand.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowRun.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpStart.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumping.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpEnd.X"));

	for (int i = 0; i < Status_End; ++i)
		cowMotion[i]->Init();

	Rendering::Get()->AddRenderingObject(cowMotion[status]);

	//face = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Face_Bash.X");
	//tail = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Tail_Level3I.X");
	//body = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Player/").c_str(), L"Body_Bash.X");

	//Camera::Get()->SetTargetPosition(&body->position);
	//axe = new Weapon();
	//axe->Init();

	//Rendering::Get()->AddRenderingObject(body);
	//Rendering::Get()->AddRenderingObject(tail);
	//Rendering::Get()->AddRenderingObject(face);
	//Rendering::Get()->AddRenderingObject(cowJumpStart);

	boundary = new BoundingSphere(D3DXVECTOR3(0, 0, 0), 18.f);
	boundary->InitGeometry();

	//Rendering::Get()->AddRenderingObject(this);

}

Player::~Player()
{



}

void Player::Init()
{


}

void Player::Update()
{
	Camera::Get()->UpdateRotation();
	rotation.y = Camera::Get()->GetCurrentRotation()->y;


	if (status != Player_Stand) //forward 설정
	{

		D3DXMatrixRotationY(&matR, rotation.y);
		D3DXVec3TransformNormal(&forward,
			&D3DXVECTOR3(deltaTransform.x, 0, deltaTransform.z),
			&matR);
		D3DXVec3Normalize(&forward, &forward);
	}

	Rendering::Get()->RemoveRenderingObject(cowMotion[status]);

	if (!bJumping) //점프모션일때 다른모션 막기
	{

		if (Input::KeyPress('W'))
		{
			status = Player_Run;

			if (Input::ButtonPress(Input::RBUTTON))
			{
				rotation.y = Camera::Get()->GetCurrentRotation()->y;
			}
		}
		if (Input::KeyUp('W'))
		{
			status = Player_Stand;
		}

		if (Input::KeyPress('A'))
		{
			if (Input::ButtonPress(Input::RBUTTON))
			{
				status = Player_Run;
				rotation.y = Camera::Get()->GetCurrentRotation()->y;
			}
		}
		if (Input::KeyUp('A'))
		{
			status = Player_Stand;
		}
		if (Input::KeyPress('D'))
		{
			if (Input::ButtonPress(Input::RBUTTON))
			{
				status = Player_Run;
				rotation.y = Camera::Get()->GetCurrentRotation()->y;
			}
		}
		if (Input::KeyUp('D'))
		{
			status = Player_Stand;
		}


		if (Input::KeyDown(VK_SPACE))
		{
			bJumping = true;
			status = Player_JumpStart;
		}


		if (Input::Get()->ButtonPress(Input::RBUTTON))
		{
			Camera::Get()->UpdateRotation();
		}


		//if (Input::Get()->ButtonPress(Input::LBUTTON))
		//{
		//		D3DXVECTOR3 pos = Input::GetMousePosition();
		//		ObjMap* tmp = new ObjMap(); tmp->Init();
		//		if (tmp->ComputeClickPosition(pos, pos.x, pos.y) == true)
		//		{
		//			Objects::GetPlayer()->SetPosition(&pos);
		//		}
		//		SAFE_DELETE(tmp);
		//}


		if (Input::KeyDown('1'))
		{
			status = Player_Jumping;
			fireCube = new ShaderCube(position, forward);
			fireCube->Init();
			Scenes::Get()->GetCurrentScene()->AddDisplayObject(fireCube);
		}
	}


	//if (D3DXVec3Length(&(movePosition - position)) >= FLT_EPSILON)
	//{
	//	bMoving = true;
	//	status = Player_Run;
	//	targetPosition = position + moveDirection;
	//}

	//if (D3DXVec3Length(&(movePosition - position)) < FLT_EPSILON)
	//{

	//}

	//if (targetPosition)
	//{
	//	D3DXVECTOR3 s = targetPosition - position;
	//	D3DXVec3Normalize(&s, &s);

	//	position += s * Time::Get()->GetDeltaTime();
	//}



	IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	IDisplayObject::ApplyVelocity();





	cowMotion[status]->SetPosition(&position);
	cowMotion[status]->SetRotation(&rotation);
	Rendering::Get()->AddRenderingObject(cowMotion[status]);



	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	D3DXTRACK_DESC track;
	dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->GetTrackDesc(0, &track);
	dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->GetAnimationSet(0, &pCurrAnimSet);

	if (pCurrAnimSet->GetPeriodicPosition(track.Position)
		>= pCurrAnimSet->GetPeriod() - 0.02f)
	{
		Rendering::Get()->RemoveRenderingObject(cowMotion[status]);

		dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->SetTrackPosition(0, 0);

		if (status == Player_JumpStart)
			status = Player_JumpEnd;


		else if (status == Player_JumpEnd)
		{
			status = Player_Stand;
			bJumping = false;
		}

		if ((status == Player_Jumping))
			status = Player_JumpEnd;
	}

	cowMotion[status]->Update();

////////////////////

	list<IDisplayObject*> m_mapList;
	m_mapList = Objects::FindObjectsByTag(TAG_MAP);
	list<IMap*> mapList;
	for (auto p : m_mapList)
	{
		mapList.push_back((IMap*)p);
	}

	for (auto p : mapList)
	{
		if (mapList.empty() == false)
		{
			if (velocity.y <= 0)
			{
				float height;

				if (p->ComputeHeight(height, targetPosition))
				{
					if (targetPosition.y <= height)
					{
						targetPosition.y = height;
						velocity.y = 0;
					}
					position = targetPosition;
				}
			}
			else
			{
				position = targetPosition;
			}
		}
		else
		{
			position = targetPosition;
		}
	}
/////////////////////////////////////////


	ImGui::Begin("Player Status");
	//ImGui::SliderInt("stat", &event, 0, 99);
	ImGui::SliderFloat3("forward", (float*)forward, -10, 10);
	ImGui::End();


}

void Player::Render()
{
	
}

void Player::Delete()
{
}

void Player::Draw()
{
}

