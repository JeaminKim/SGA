#include "stdafx.h"
#include "Export.h"
#include "../Fbx/Exporter.h"

#include "../Objects/GameModel.h"

Export::Export(ExecuteValues * values)
	: Execute(values)
{
	Fbx::Exporter* exporter = NULL;

	////Capsule
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Capsule.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Capsule.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Capsule.mesh");
	//SAFE_DELETE(exporter);

	////Cube
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cube.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Cube.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Cube.mesh");
	//SAFE_DELETE(exporter);

	////Cylinder
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cylinder.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Cylinder.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Cylinder.mesh");
	//SAFE_DELETE(exporter);
	//
	////Plane
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Plane.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Plane.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Plane.mesh");
	//SAFE_DELETE(exporter);
	//
	////Quad
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Quad.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Quad.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Quad.mesh");
	//SAFE_DELETE(exporter);

	////Sphere
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Sphere.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Sphere.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Sphere.mesh");
	//SAFE_DELETE(exporter);

	////Teapot
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Teapot.fbx");
	//exporter->ExportMaterial(Materials + L"/Meshes/", L"Teapot.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Teapot.mesh");
	//SAFE_DELETE(exporter);


	////Kachujin Mesh
	//exporter = new Fbx::Exporter(Assets + L"Kachujin/Mesh.fbx");
	//exporter->ExportMaterial(Models + L"Kachujin/", L"Kachujin.material");
	//exporter->ExportMesh(Models + L"Kachujin/", L"Kachujin.mesh");
	//SAFE_DELETE(exporter);

	////Samba_Dancing
	//exporter = new Fbx::Exporter(Assets + L"Kachujin/Samba_Dancing.fbx");
	//exporter->ExportAnimation(Models + L"Kachujin/", L"Samba_Dance.anim", 0);
	//SAFE_DELETE(exporter);

	////Taunt
	//exporter = new Fbx::Exporter(Assets + L"Kachujin/Taunt.fbx");
	//exporter->ExportAnimation(Models + L"Kachujin/", L"Taunt.anim", 0);
	//SAFE_DELETE(exporter);

	////Running
	//exporter = new Fbx::Exporter(Assets + L"Kachujin/Running.fbx");
	//exporter->ExportAnimation(Models + L"Kachujin/", L"Running.anim", 0);
	//SAFE_DELETE(exporter);

	//Kachujin Mesh
	//exporter = new Fbx::Exporter(Assets + L"Bunny/stanford-bunny.fbx");
	//exporter->ExportMaterial(Materials + L"Meshes/", L"Bunny.material");
	//exporter->ExportMesh(Models + L"Meshes/", L"Bunny.mesh");
	//SAFE_DELETE(exporter);
}

Export::~Export()
{
	
}

void Export::Update()
{
	
}

void Export::PreRender()
{
	
}

void Export::Render()
{
	
}

void Export::PostRender()
{
	
}

void Export::ResizeScreen()
{
}

