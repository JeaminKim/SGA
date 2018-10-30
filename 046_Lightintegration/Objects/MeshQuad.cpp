#include "stdafx.h"
#include "MeshQuad.h"

MeshQuad::MeshQuad()
	: GameModel(Materials + L"Meshes/", L"Quad.material", Models + L"Meshes/", L"Quad.mesh")
{
	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);
}

MeshQuad::~MeshQuad()
{
	SAFE_DELETE(shader);
}

void MeshQuad::Update()
{
	__super::Update();
}

void MeshQuad::Render()
{
	rasterizerState[1]->RSSetState();

	__super::Render();

	rasterizerState[0]->RSSetState();
}
