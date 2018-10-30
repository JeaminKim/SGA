#pragma once

class SamplerState
{
public:
	SamplerState();
	~SamplerState();

	void PSSetSamplers(UINT slot);

	void Filter(D3D11_FILTER val);
	void AddressU(D3D11_TEXTURE_ADDRESS_MODE val);
	void AddressV(D3D11_TEXTURE_ADDRESS_MODE val);
	void AddressW(D3D11_TEXTURE_ADDRESS_MODE val);
	void ComparisonFunc(D3D11_COMPARISON_FUNC val);

private:
	void Changed();

private:
	D3D11_SAMPLER_DESC desc;

	ID3D11SamplerState* state;
};