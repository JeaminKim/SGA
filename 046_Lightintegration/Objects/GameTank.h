#pragma once
#include "GameModel.h"

class GameTank : public GameModel
{
public:
	GameTank(wstring matFolder, wstring matFile);
	~GameTank();

	void Update() override;
	void Render() override;

private:
	ModelBone* leftWheelBone;
	D3DXMATRIX leftWheel;
};