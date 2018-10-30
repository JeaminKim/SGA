#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight()
{
	buffer = new Buffer();
}

PointLight::~PointLight()
{
	SAFE_DELETE(buffer);
}

void PointLight::Update()
{
	buffer->SetPSBuffer(2);
}
