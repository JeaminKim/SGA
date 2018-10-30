#include "stdafx.h"
#include "Blur.h"

Blur::Blur(ExecuteValues * values)
	: Render2D(values, Shaders + L"032_Blur.hlsl")
{
	buffer = new Buffer();
}

Blur::~Blur()
{
	SAFE_DELETE(buffer);
}

void Blur::Render()
{
	ImGui::Separator();
	ImGui::SliderInt("Blur Select", &buffer->Data.Select, 0, 2);
	ImGui::SliderInt("Blur Count", &buffer->Data.Count, 0, 20);
	ImGui::Separator();

	
	D3DXVECTOR2 scale = Scale();
	buffer->Data.Width = (int)scale.x;
	buffer->Data.Height = (int)scale.y;

	buffer->SetPSBuffer(10);

	__super::Render();
}
