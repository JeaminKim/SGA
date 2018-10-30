#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SpotLight()
{
	buffer = new Buffer();
}

SpotLight::~SpotLight()
{
	SAFE_DELETE(buffer);
}

void SpotLight::Update()
{
	buffer->SetPSBuffer(3);
}
