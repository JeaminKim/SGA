#pragma once
#include "GameModel.h"

class MeshPlane : public GameModel
{
public:
	MeshPlane();
	~MeshPlane();

	void Update() override;
	void Render() override;

private:
	
};