#include "stdafx.h"
#include "Sky.h"

Sky::Sky(ExecuteValues * values)
	: values(values)
	, realTime(false), theta(0), phi(0)
	, radius(10), slices(32), stacks(16)
{
	mieTarget = new RenderTarget(values, 128, 64);
	rayLeighTarget = new RenderTarget(values, 128, 64);

	scatterShader = new Shader(Shaders + L"044_Scattering1.hlsl", "VS_Scattering", "PS_Scattering");
	targetShader = new Shader(Shaders + L"044_Scattering1.hlsl", "VS_Target", "PS_Target");

	worldBuffer = new WorldBuffer();
	targetBuffer = new TargetBuffer();
	scatterBuffer = new ScatterBuffer();

	GenerateSphere();
	GenerateQuad();


	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);
	depthState[1]->DepthWriteEnable(D3D11_DEPTH_WRITE_MASK_ZERO);

	rayleigh2D = new Render2D(values);
	rayleigh2D->Position(0, 100);
	rayleigh2D->Scale(200, 100);

	mie2D = new Render2D(values);
	mie2D->Position(0, 0);
	mie2D->Scale(200, 100);

	starField = new Texture(Textures + L"Starfield.png");
		
	rayleighSampler = new SamplerState();
	rayleighSampler->AddressU(D3D11_TEXTURE_ADDRESS_CLAMP);
	rayleighSampler->AddressV(D3D11_TEXTURE_ADDRESS_CLAMP);
	rayleighSampler->AddressW(D3D11_TEXTURE_ADDRESS_CLAMP);
	rayleighSampler->Filter(D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	mieSampler = new SamplerState();
	mieSampler->AddressU(D3D11_TEXTURE_ADDRESS_CLAMP);
	mieSampler->AddressV(D3D11_TEXTURE_ADDRESS_CLAMP);
	mieSampler->AddressW(D3D11_TEXTURE_ADDRESS_CLAMP);
	mieSampler->Filter(D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	starSampler = new SamplerState();
	starSampler->AddressU(D3D11_TEXTURE_ADDRESS_CLAMP);
	starSampler->AddressV(D3D11_TEXTURE_ADDRESS_CLAMP);
	starSampler->AddressW(D3D11_TEXTURE_ADDRESS_CLAMP);
	starSampler->Filter(D3D11_FILTER_MIN_MAG_MIP_LINEAR);
}

Sky::~Sky()
{
	SAFE_DELETE(depthState[0]);
	SAFE_DELETE(depthState[1]);

	SAFE_DELETE_ARRAY(quadVertices);
	SAFE_RELEASE(quadBuffer);

	SAFE_DELETE(scatterBuffer);
	SAFE_DELETE(targetBuffer);
	SAFE_DELETE(worldBuffer);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_DELETE(rayleighSampler);
	SAFE_DELETE(mieSampler);
	SAFE_DELETE(starSampler);

	SAFE_DELETE(scatterShader);
	SAFE_DELETE(targetShader);

	SAFE_DELETE(mieTarget);
	SAFE_DELETE(rayLeighTarget);
}

void Sky::Update()
{
	D3DXMATRIX V;
	D3DXVECTOR3 pos;
	values->MainCamera->Position(&pos);
	D3DXMatrixTranslation(&V, pos.x, pos.y, pos.z);

	worldBuffer->SetMatrix(V);
}

void Sky::PreRender()
{
	if (prevTheta == theta && prevPhi == phi) return;

	mieTarget->Set();
	rayLeighTarget->Set();

	ID3D11RenderTargetView * rtvs[2];
	rtvs[0] = rayLeighTarget->GetRTV();
	rtvs[1] = mieTarget->GetRTV();

	//   멀티 안됨, 통일 시켜야함
	ID3D11DepthStencilView * dsv;
	dsv = rayLeighTarget->GetDSV();

	D3D::Get()->SetRenderTargets(2, rtvs, dsv);

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &quadBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	targetBuffer->SetPSBuffer(11);
	targetShader->Render();

	D3D::GetDC()->Draw(6, 0);
}

void Sky::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	scatterShader->Render();
	worldBuffer->SetVSBuffer(1);

	ID3D11ShaderResourceView * rSRV = rayLeighTarget->GetSRV();
	D3D::GetDC()->PSSetShaderResources(10, 1, &rSRV);

	ID3D11ShaderResourceView * mSRV = mieTarget->GetSRV();
	D3D::GetDC()->PSSetShaderResources(11, 1, &mSRV);

	rayleighSampler->PSSetSamplers(10);
	mieSampler->PSSetSamplers(11);
	starSampler->PSSetSamplers(11);

	starField->SetShaderResource(12);

	scatterBuffer->Data.StarIntensity = values->GlobalLight->Data.Direction.y;
	scatterBuffer->SetPSBuffer(10);

	depthState[1]->OMSetDepthStencilState();
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	depthState[0]->OMSetDepthStencilState();

	rayleigh2D->SRV(rayLeighTarget->GetSRV());
	rayleigh2D->Update();
	rayleigh2D->Render();

	mie2D->SRV(mieTarget->GetSRV());
	mie2D->Update();
	mie2D->Render();
}

void Sky::GenerateSphere()
{
	UINT domeCount = 32;
	UINT latitude = domeCount / 2; // 위도
	UINT longitude = domeCount; // 경도

	vertexCount = longitude * latitude * 2;
	indexCount = (longitude - 1) * (latitude - 1) * 2 * 6;

	VertexTexture* vertices = new VertexTexture[vertexCount];

	UINT index = 0;
	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * Math::PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = Math::PI * j / (latitude - 1);

			vertices[index].Position.x = sinf(xz) * cosf(y);
			vertices[index].Position.y = cosf(xz);
			vertices[index].Position.z = sinf(xz) * sinf(y);
			vertices[index].Position *= 10.0f; // 크기를 키우려고 임의의 값 곱한거

			vertices[index].Uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].Uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		} // for(j)
	}  // for(i)

	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * Math::PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = (Math::PI * 2.0f) - (Math::PI * j / (latitude - 1));

			vertices[index].Position.x = sinf(xz) * cosf(y);
			vertices[index].Position.y = cosf(xz);
			vertices[index].Position.z = sinf(xz) * sinf(y);
			vertices[index].Position *= 10.0f; // 크기를 키우려고 임의의 값 곱한거

			vertices[index].Uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].Uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		} // for(j)
	}  // for(i)


	index = 0;
	UINT* indices = new UINT[indexCount * 3];

	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = i * latitude + j;
			indices[index++] = (i + 1) * latitude + j;
			indices[index++] = (i + 1) * latitude + (j + 1);

			indices[index++] = (i + 1) * latitude + (j + 1);
			indices[index++] = i * latitude + (j + 1);
			indices[index++] = i * latitude + j;
		}
	}

	UINT offset = latitude * longitude;
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = offset + i * latitude + j;
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + j;

			indices[index++] = offset + i * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + i * latitude + j;
		}
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

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Sky::GenerateQuad()
{
	quadVertices = new VertexTexture[6];

	quadVertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);   //   0
	quadVertices[1].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);   //   1
	quadVertices[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);   //   2
	quadVertices[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);   //   2
	quadVertices[4].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);   //   1
	quadVertices[5].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);   //   3

	quadVertices[0].Uv = D3DXVECTOR2(0, 1);   //   0
	quadVertices[1].Uv = D3DXVECTOR2(0, 0);   //   1
	quadVertices[2].Uv = D3DXVECTOR2(1, 1);   //   2
	quadVertices[3].Uv = D3DXVECTOR2(1, 1);   //   2
	quadVertices[4].Uv = D3DXVECTOR2(0, 0);   //   1
	quadVertices[5].Uv = D3DXVECTOR2(1, 0);   //   3

	//   CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = quadVertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &quadBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Sky::GetStarIntensity()
{
}
