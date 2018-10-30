#pragma once
#include "GameModel.h"

class MeshCube : public GameModel
{
public:
	MeshCube();
	~MeshCube();

	void Update() override;
	void Render() override;

private:
	
};