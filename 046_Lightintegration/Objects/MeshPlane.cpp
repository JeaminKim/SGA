#include "stdafx.h"
#include "MeshPlane.h"

MeshPlane::MeshPlane()
	: GameModel(Materials + L"Meshes/", L"Plane.material", Models + L"Meshes/", L"Plane.mesh")
{
	
}

MeshPlane::~MeshPlane()
{
	
}

void MeshPlane::Update()
{
	__super::Update();
}

void MeshPlane::Render()
{
	__super::Render();
}
