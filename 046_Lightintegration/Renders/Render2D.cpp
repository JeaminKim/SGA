#include "stdafx.h"
#include "Render2D.h"

Render2D::Render2D(ExecuteValues * values, wstring shaderFile, string vsName, string psName)
	: values(values)
	, position(0, 0), scale(1, 1)
{
	String::Replace(&shaderFile, Shaders, L"");

	wstring file = L"";
	if (shaderFile.length() > 1)
	{
		file = Shaders + shaderFile;
		shader = new Shader(file, vsName, psName);
	}
	else
	{
		file = Shaders + L"990_Render2D.hlsl";
		shader = new Shader(file);
	}


	worldBuffer = new WorldBuffer();

	//Create Ortho Matrix
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		D3DXMatrixOrthoOffCenterLH(&orthographic, 0, desc.Width, 0, desc.Height, -1, 1);
	}

	//Create DepthStencilState
	{
		depthState[0] = new DepthStencilState();
		depthState[1] = new DepthStencilState();
		depthState[1]->DepthEnable(false);
	}

	//Create Vertex Buffer
	{
		VertexTexture* vertices = new VertexTexture[6];
		vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

		vertices[0].Uv = D3DXVECTOR2(0, 1);
		vertices[1].Uv = D3DXVECTOR2(0, 0);
		vertices[2].Uv = D3DXVECTOR2(1, 1);
		vertices[3].Uv = D3DXVECTOR2(1, 1);
		vertices[4].Uv = D3DXVECTOR2(0, 0);
		vertices[5].Uv = D3DXVECTOR2(1, 0);

		
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));

		SAFE_DELETE_ARRAY(vertices);
	}

	D3DXMatrixLookAtLH
	(
		&view, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0)
	);
}

Render2D::~Render2D()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(depthState[0]);
	SAFE_DELETE(depthState[1]);

	SAFE_RELEASE(vertexBuffer);
}

void Render2D::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Render2D::Position(D3DXVECTOR2 vec)
{
	position = vec;
}

void Render2D::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Render2D::Scale(D3DXVECTOR2 vec)
{
	scale = vec;
}

void Render2D::Update()
{
	D3DXMATRIX S, T, W;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixTranslation
	(
		&T, position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, 0
	);


	W = S * T;
	worldBuffer->SetMatrix(W);
}

void Render2D::Render()
{
	//2D용 View, Projection 설정
	{
		values->ViewProjection->SetView(view);
		values->ViewProjection->SetProjection(orthographic);
		values->ViewProjection->SetVSBuffer(0);
	}
	
	//드로잉 구간
	{
		UINT stride = sizeof(VertexTexture);
		UINT offset = 0;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D::GetDC()->PSSetShaderResources(10, 1, &srv);

		worldBuffer->SetVSBuffer(1);
		shader->Render();

		depthState[1]->OMSetDepthStencilState();
		D3D::GetDC()->Draw(6, 0);
		depthState[0]->OMSetDepthStencilState();
	}

	//메인용 View, Projection으로 변경
	{
		D3DXMATRIX view, projection;
		values->MainCamera->Matrix(&view);
		values->Perspective->GetMatrix(&projection);

		values->ViewProjection->SetView(view);
		values->ViewProjection->SetProjection(projection);
		values->ViewProjection->SetVSBuffer(0);
	}
}