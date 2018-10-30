#pragma once
#include "GameModel.h"

class MeshSphere : public GameModel
{
public:
	MeshSphere();
	~MeshSphere();

	void Update() override;
	void Render() override;

private:
	
};