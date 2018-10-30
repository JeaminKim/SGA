#pragma once

class Water
{
public:
	Water(ExecuteValues* values, UINT width, UINT height, float thick, Texture* heightMap);
	~Water();

	void Update();
	void Render();

private:
	ExecuteValues* values;

	Material* material;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	UINT width, height;
	UINT vertexCount, indexCount;

	float thick;
	bool bWireframe;

	RasterizerState* rasterizerState[2];
	BlendState* blendState[2];
	
	Texture* heightMap;


	class VsBuffer : public ShaderBuffer
	{
	public:
		VsBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.WaveFrequency = 0.05f;
			Data.WaveAmplitude = 1.0f;

			Data.TextureScale = D3DXVECTOR2(14, 14);
			Data.BumpSpeed = D3DXVECTOR2(0.0f, 0.05f);
			Data.BumpHeight = 0.6f;
			Data.RunningTime = 0.0f;
		}

		struct Struct
		{
			float WaveFrequency;
			float WaveAmplitude;
			D3DXVECTOR2 TextureScale;

			D3DXVECTOR2 BumpSpeed;
			float BumpHeight;
			float RunningTime;
		} Data;
	};
	VsBuffer* vsBuffer;


	class PsBuffer : public ShaderBuffer
	{
	public:
		PsBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.ShallowColor = D3DXCOLOR(0.7f, 0.85f, 0.8f, 1.0f);
			Data.DeepColor = D3DXCOLOR(0.2f, 0.5f, 0.95f, 1.0f);

			Data.Bias = 0.8f;
			Data.Power = 0.5;
			Data.Amount = 0.5;

			Data.HeightRatio = 7.5f;
			Data.ShoreBlend = 35.0f;
		}

		struct Struct
		{
			D3DXCOLOR ShallowColor;
			D3DXCOLOR DeepColor;

			float Bias;
			float Power;
			float Amount;
			float ShoreBlend;

			D3DXVECTOR2 OceanSize;
			float HeightRatio;

			float Padding;
		} Data;
	};
	PsBuffer* psBuffer;
};