#include "stdafx.h"
#include "TestShadow.h"
#include "../Objects/GameAnimModel.h"
#include "../Objects/MeshPlane.h"
#include "../Lights/Shadow.h"

TestShadow::TestShadow(ExecuteValues * values)
	: Execute(values)
{
	hanjo = new GameAnimModel
	(
		Models + L"Kachujin/", L"Kachujin.material",
		Models + L"Kachujin/", L"Kachujin.mesh"
	);
	hanjo->AddClip(Models + L"Kachujin/Samba_Dance.anim");
	hanjo->Play(0, true, 0.0f, 10.0f);

	hanjoShader = new Shader(Shaders + L"035_Animation.hlsl");
	hanjo->SetShader(hanjoShader);

	hanjo->Scale(0.025f, 0.025f, 0.025f);


	planeShader = new Shader(Shaders + L"042_Plane.hlsl");

	plane = new MeshPlane();
	plane->SetShader(planeShader);
	plane->Scale(10, 1, 10);
	plane->SetDiffuse(1, 1, 1, 1);

	shadow = new Shadow(values);
	shadow->Add(hanjo);
	shadow->Add(plane);
}

TestShadow::~TestShadow()
{
	SAFE_DELETE(shadow);

	SAFE_DELETE(planeShader);
	SAFE_DELETE(plane);

	SAFE_DELETE(hanjoShader);
	SAFE_DELETE(hanjo);
}

void TestShadow::Update()
{
	shadow->Update();

	hanjo->Update();
	//plane->Update();
}

void TestShadow::PreRender()
{
	shadow->PreRender();
}

void TestShadow::Render()
{
	/*hanjo->Render();*/
	//plane->Render();

	shadow->Render();
}

void TestShadow::PostRender()
{
	
}

