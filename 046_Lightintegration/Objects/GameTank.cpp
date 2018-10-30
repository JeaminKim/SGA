#include "stdafx.h"
#include "GameTank.h"

GameTank::GameTank(wstring matFolder, wstring matFile)
	: GameModel(matFolder, matFile,	Models + L"Tank/", L"Tank.mesh")
{
	leftWheelBone = model->BoneByName(L"l_back_wheel_geo");
	leftWheel = leftWheelBone->Local();
}

GameTank::~GameTank()
{
}

void GameTank::Update()
{
	__super::Update();

	D3DXMATRIX R;
	//D3DXMatrixRotationX(&R, sinf(Time::Get()->Running() * 2.0f) * 0.25f);
	D3DXMatrixRotationX(&R, Math::ToRadian(Time::Get()->Running() * -100));

	leftWheelBone->Local(R * leftWheel);
}

void GameTank::Render()
{
	__super::Render();
}
