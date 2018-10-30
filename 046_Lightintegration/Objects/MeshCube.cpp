#include "stdafx.h"
#include "MeshCube.h"

MeshCube::MeshCube()
	: GameModel(Materials + L"Meshes/", L"Cube.material", Models + L"Meshes/", L"Cube.mesh")
{
	
}

MeshCube::~MeshCube()
{
	SAFE_DELETE(shader);
}

void MeshCube::Update()
{
	__super::Update();
}

void MeshCube::Render()
{
	__super::Render();
}
