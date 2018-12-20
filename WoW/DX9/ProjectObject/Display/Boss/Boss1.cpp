#include "stdafx.h"
#include "Boss1.h"
#include "Graphics\Model\X\AllocateHierarchyHLSL.h"
#include "ProjectObject\Display\Weapon.h"
#include "ProjectObject\Display\ShaderCube.h"
#include "Graphics\Model\OBJ\ObjMap.h"
#include "ProjectObject\Display\Skill\SkullMissile.h"


Boss1::Boss1()
{
	bJumping = false;
	bMoving = false;
	position = D3DXVECTOR3(16.5f, 2, 16.5f);
	scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	status = Boss1_Stand;

	SkinnedMeshHLSL *smh = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/VoidWalker/").c_str(), L"VoidWalker_Stand0.X");
	smh->scale = scale;
	cowMotion.push_back(smh);
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowRun.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpStart.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumping.X"));
	cowMotion.push_back(new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Cow/").c_str(), L"CowJumpEnd.X"));

	for (int i = 0; i < Boss1_Status_End; ++i)
		cowMotion[i]->Init();

	Rendering::Get()->AddRenderingObject(cowMotion[status]);

	//face = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Boss1/").c_str(), L"Face_Bash.X");
	//tail = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Boss1/").c_str(), L"Tail_Level3I.X");
	//body = new SkinnedMeshHLSL((ASSET_PATH + L"Models/X/Boss1/").c_str(), L"Body_Bash.X");

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

Boss1::~Boss1()
{



}

void Boss1::Init()
{


}

void Boss1::Update()
{
	D3DXMatrixRotationY(&matR, rotation.y); // forward 설정
	D3DXVec3TransformNormal(&forward,
		&D3DXVECTOR3(0, 0, 1),
		&matR);
	D3DXVec3Normalize(&forward, &forward);


	Rendering::Get()->RemoveRenderingObject(cowMotion[status]);

	//IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	IDisplayObject::ApplyVelocity();


	/*skull->SetPosition(&position);
	skull->SetRotation(&rotation);*/
	cowMotion[status]->SetPosition(&position);
	cowMotion[status]->SetRotation(&rotation);

	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	D3DXTRACK_DESC track;
	dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->GetTrackDesc(0, &track);
	dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->GetAnimationSet(0, &pCurrAnimSet);
	if (pCurrAnimSet->GetPeriodicPosition(track.Position)
		>= pCurrAnimSet->GetPeriod() - 0.02f)
	{
		Rendering::Get()->RemoveRenderingObject(cowMotion[status]);

		dynamic_cast<SkinnedMeshHLSL*>(cowMotion[status])->m_pAC->SetTrackPosition(0, 0);

		if (status == Boss1_JumpStart)
			status = Boss1_JumpEnd;


		else if (status == Boss1_JumpEnd)
		{
			status = Boss1_Stand;
			bJumping = false;

		}

		if ((status == Boss1_Jumping))
			status = Boss1_JumpEnd;
	}
	Rendering::Get()->AddRenderingObject(cowMotion[status]);



	// sadsa

	cowMotion[status]->Update();

////////////////////

	//ApplyMap();
/////////////////////////////////////////


	ImGui::Begin("Boss1 Status");
	//ImGui::SliderInt("stat", &event, 0, 99);
	ImGui::SliderFloat3("forward", (float*)forward, -10, 10);
	ImGui::End();


}

void Boss1::Render()
{
	
}

void Boss1::Delete()
{
}

void Boss1::UpdateInput()
{

	if (!bJumping) //점프모션일때 다른모션 막기
	{

		if (Input::KeyPress('W'))
		{
			status = Boss1_Run;

		}
		if (Input::KeyUp('W'))
		{
			status = Boss1_Stand;

		}

		if (Input::KeyPress('A'))
		{
			status = Boss1_Run;
		}
		if (Input::KeyUp('A'))
		{
			status = Boss1_Stand;
		}
		if (Input::KeyPress('D'))
		{
			status = Boss1_Run;
		}
		if (Input::KeyUp('D'))
		{
			status = Boss1_Stand;
		}


		if (Input::KeyDown(VK_SPACE))
		{
			bJumping = true;
			status = Boss1_JumpStart;
		}



		if (Input::KeyDown('1'))
		{
			status = Boss1_Jumping;
			fireCube = new ShaderCube(position, forward);
			fireCube->Init();
			Scenes::Get()->GetCurrentScene()->AddDisplayObject(fireCube);
		}

		if (Input::KeyDown('2'))
		{
			IDisplayObject *skull = new SkullMissile(position, forward);
			skull->Init();
			Scenes::Get()->GetCurrentScene()->AddDisplayObject(skull);
		}
	}

}

void Boss1::ApplyMap()
{

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
}

void Boss1::Draw()
{
}

