#include "stdafx.h"
#include "Water.h"

Water::Water(ExecuteValues * values, UINT width, UINT height, float thick, Texture * heightMap)
	: values(values), width(width), height(height), thick(thick)
	, heightMap(heightMap), bWireframe(false)
{
	material = new Material(Shaders + L"040_Water.hlsl");
	material->SetAmbient(D3DXCOLOR(0.25f, 0.20f, 1.0f, 1.0f));
	material->SetDiffuse(D3DXCOLOR(0.600f, 0.721f, 0.937f, 0.627f));
	material->SetSpecular(D3DXCOLOR(1, 1, 1, 1));
	material->SetNormalMap(Textures + L"WaveNormal.png");
	material->SetShininess(500);


	VertexTexture* vertices = NULL;
	UINT* indices = NULL;

	//Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTexture[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x * thick;
				vertices[index].Position.y = (float)0.0f;
				vertices[index].Position.z = (float)z * thick;

				vertices[index].Uv.x = (float)x / (float)width;
				vertices[index].Uv.y = (float)z / (float)height;
			}
		}//for(z)
	}

	//CreateIndexData
	{
		indexCount = width * height * 6;
		indices = new UINT[indexCount];

		UINT index = 0;
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				indices[index + 0] = (width + 1) * z + x; //0
				indices[index + 1] = (width + 1) * (z + 1) + x; //1
				indices[index + 2] = (width + 1) * z + x + 1; //2

				indices[index + 3] = (width + 1) * z + x + 1; //2
				indices[index + 4] = (width + 1) * (z + 1) + x; //1
				indices[index + 5] = (width + 1) * (z + 1) + x + 1; //1

				index += 6;
			}
		}//for(z)
	}

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		rasterizerState[0] = new RasterizerState();
		rasterizerState[1] = new RasterizerState();
		rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

		blendState[0] = new BlendState();
		blendState[1] = new BlendState();
		blendState[1]->BlendEnable(true);
	}

	worldBuffer = new WorldBuffer();
	vsBuffer = new VsBuffer();

	psBuffer = new PsBuffer();
	psBuffer->Data.OceanSize = D3DXVECTOR2(width * thick, height * thick);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

Water::~Water()
{
	SAFE_DELETE(material);
	SAFE_DELETE(worldBuffer);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_DELETE(rasterizerState[0]);
	SAFE_DELETE(rasterizerState[1]);
}

void Water::Update()
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, 0, 3, 0);

	worldBuffer->SetMatrix(T);

	vsBuffer->Data.RunningTime = Time::Get()->Running();
}

void Water::Render()
{
	psBuffer->SetPSBuffer(10);
	vsBuffer->SetVSBuffer(10);

	heightMap->SetShaderResource(5);
	heightMap->SetShaderSampler(5);


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();


	if (bWireframe == true)
		rasterizerState[1]->RSSetState();

	blendState[1]->OMSetBlendState();

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);

	blendState[0]->OMSetBlendState();
	rasterizerState[0]->RSSetState();
}
