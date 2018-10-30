#include "stdafx.h"
#include "MeshBunny.h"

MeshBunny::MeshBunny()
	: GameModel(Materials + L"Meshes/", L"Bunny.material", Models + L"Meshes/", L"Bunny.mesh")
{
	
}

MeshBunny::~MeshBunny()
{
	
}

void MeshBunny::Update()
{
	__super::Update();
}

void MeshBunny::Render()
{
	__super::Render();
}
