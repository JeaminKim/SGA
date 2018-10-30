#include "stdafx.h"
#include "SamplerState.h"

SamplerState::SamplerState()
	: state(NULL)
{
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	Changed();
}

SamplerState::~SamplerState()
{
	SAFE_RELEASE(state);
}

void SamplerState::PSSetSamplers(UINT slot)
{
	D3D::GetDC()->PSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER val)
{
	desc.Filter = val;

	Changed();
}

void SamplerState::AddressU(D3D11_TEXTURE_ADDRESS_MODE val)
{
	desc.AddressU = val;

	Changed();
}

void SamplerState::AddressV(D3D11_TEXTURE_ADDRESS_MODE val)
{
	desc.AddressV = val;

	Changed();
}

void SamplerState::AddressW(D3D11_TEXTURE_ADDRESS_MODE val)
{
	desc.AddressW = val;

	Changed();
}

void SamplerState::ComparisonFunc(D3D11_COMPARISON_FUNC val)
{
	desc.ComparisonFunc = val;

	Changed();
}

void SamplerState::Changed()
{
	SAFE_RELEASE(state);

	HRESULT hr;

	hr = D3D::GetDevice()->CreateSamplerState(&desc, &state);
	assert(SUCCEEDED(hr));
}
