#include "stdafx.h"
#include "Grayscale.h"

Grayscale::Grayscale(ExecuteValues * values)
	: Render2D(values, Shaders + L"032_Grayscale.hlsl")
{
	buffer = new Buffer();
}

Grayscale::~Grayscale()
{
	SAFE_DELETE(buffer);
}

void Grayscale::Render()
{
	ImGui::Separator();
	ImGui::SliderInt("Grayscale Select", &buffer->Data.Select, 0, 2);
	ImGui::Separator();

	buffer->SetPSBuffer(10);


	__super::Render();
}
