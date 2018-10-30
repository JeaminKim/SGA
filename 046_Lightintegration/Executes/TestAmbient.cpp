#include "stdafx.h"
#include "TestAmbient.h"
#include "../Landscape/Sky.h"
#include "../Landscape/TerrainRender.h"
#include "../Objects/MeshPlane.h"
#include "../Objects/MeshCube.h"
#include "../Objects/MeshSphere.h"
#include "../Objects/MeshBunny.h"

TestAmbient::TestAmbient(ExecuteValues * values)
	: Execute(values)
{
	shader = new Shader(Shaders + L"045_Ambient.hlsl");
	
	plane = new MeshPlane();
	plane->Scale(10, 1, 10);
	plane->SetShader(shader);
	plane->SetDiffuse(1, 1, 1);

	cube = new MeshCube();
	cube->Position(-10, 1.5, 0);
	cube->SetShader(shader);
	cube->SetDiffuse(0, 1, 0);
	cube->Scale(3, 3, 3);


	sphere = new MeshSphere();
	sphere->Position(10, 1.5f, 0);
	sphere->SetShader(shader);
	sphere->SetDiffuse(0, 0, 1);
	sphere->Scale(3, 3, 3);

	sphere2 = new MeshSphere();
	sphere2->Position(10, 1.5f, 10);
	sphere2->SetShader(shader);
	sphere2->SetDiffuse(1, 1, 1);
	sphere2->Scale(3, 3, 3);

	bunny = new MeshBunny();
	bunny->SetShader(shader);
	bunny->SetDiffuse(1, 1, 1);
	bunny->Scale(0.02f, 0.02f, 0.02f);
	bunny->Position(0, 5, 0);
	bunny->SetDiffuseMap(Textures + L"White.png");

	//Create LightBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(LightData);

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &lightBuffer);
		assert(SUCCEEDED(hr));
	}
	

}

TestAmbient::~TestAmbient()
{
	SAFE_DELETE(lightBuffer);

	SAFE_DELETE(shader);

	SAFE_DELETE(bunny);
	SAFE_DELETE(sphere2);
	SAFE_DELETE(sphere);
	SAFE_DELETE(cube);
	SAFE_DELETE(plane);
}

void TestAmbient::Update()
{
	bunny->Update();
	plane->Update();
	sphere2->Update();
	sphere->Update();
	cube->Update();
}

void TestAmbient::PreRender()
{
}

void TestAmbient::Render()
{
	ImGui::Separator();
	ImGui::SliderFloat3("AmbientFloor", (float*)&ambientBuffer->Data.Floor, 0, 1);
	ImGui::SliderFloat3("AmbientCeil", (float*)&ambientBuffer->Data.Ceil, 0, 1);

	ImGui::ColorEdit3("DirectionColor", (float*)&ambientBuffer->Data.Color);

	ImGui::Separator();
	ImGui::SliderFloat("Specular Exp", (float*)&specularBuffer->Data.Exp, 1, 100);
	ImGui::SliderFloat("Specular Intensity", (float*)&specularBuffer->Data.Intensity, 0, 10);
	
	ImGui::SliderFloat3("PointLight Position", (float*)&pointLightBuffer->Data.Position, -100, 100);
	ImGui::SliderFloat("PointLight Range", (float*)&pointLightBuffer->Data.Range, 1, 100);
	ImGui::ColorEdit3("PointLight Color", (float*)&pointLightBuffer->Data.Color);

	ImGui::Separator();
	ImGui::SliderFloat3("SpotLight Position", (float*)&spotLightBuffer->Data.Position, -100, 100);
	ImGui::SliderFloat("SpotLight Range", (float*)&spotLightBuffer->Data.Range, 1, 180);
	ImGui::ColorEdit3("SpotLight Color", (float*)&spotLightBuffer->Data.Color);
	ImGui::SliderFloat3("SpotLight Diretion", (float*)&spotLightBuffer->Data.Direction, -1, 1);
	ImGui::SliderFloat("SpotLight Outer", (float*)&spotLightBuffer->Data.Outer, 0, 180);
	ImGui::SliderFloat("SpotLight Innter", (float*)&spotLightBuffer->Data.Inner, 0, 180);



	lightBuffer->SetPSBuffer(10);
	

	bunny->Render();
	plane->Render();
	sphere2->Render();
	sphere->Render();
	cube->Render();
}

void TestAmbient::PostRender()
{
	
}



