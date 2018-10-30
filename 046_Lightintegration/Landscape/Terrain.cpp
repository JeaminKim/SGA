#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain(ExecuteValues * values)
	: values(values)
{
	ReadHeightMap(Contents + L"HeightMaps/HeightMap256.png");
	
	CalcNormalVector();
	AlignVertexData();
}

Terrain::~Terrain()
{
	SAFE_DELETE_ARRAY(vertices);
}

void Terrain::CopyVertices(void * vertices)
{
	memcpy(vertices, this->vertices, sizeof(VertexTextureNormal) * vertexCount);
}

void Terrain::ReadHeightMap(wstring file)
{
	vector<D3DXCOLOR> heights;
	Texture* heightTexture = new Texture(file);
	heightTexture->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightTexture->GetWidth() - 1;
	height = heightTexture->GetHeight() - 1;

	//Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTextureNormal[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 15.0f;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = (float)z / height;
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
}

void Terrain::CalcNormalVector()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTextureNormal v0 = vertices[index0];
		VertexTextureNormal v1 = vertices[index1];
		VertexTextureNormal v2 = vertices[index2];

		D3DXVECTOR3 d1 = v1.Position - v0.Position;
		D3DXVECTOR3 d2 = v2.Position - v0.Position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

void Terrain::AlignVertexData()
{
	VertexTextureNormal* vertexData = new VertexTextureNormal[indexCount];

	UINT index = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index1 = (width + 1) * z + x;
			UINT index2 = (width + 1) * z + (x + 1);
			UINT index3 = (width + 1) * (z + 1) + x;
			UINT index4 = (width + 1) * (z + 1) + (x + 1);

			vertexData[index] = vertices[index3]; index++;
			vertexData[index] = vertices[index4]; index++;
			vertexData[index] = vertices[index1]; index++;
			vertexData[index] = vertices[index1]; index++;
			vertexData[index] = vertices[index4]; index++;
			vertexData[index] = vertices[index2]; index++;
		}
	}
	SAFE_DELETE_ARRAY(vertices);

	vertexCount = indexCount;
	vertices = new VertexTextureNormal[vertexCount];
	memcpy(vertices, vertexData, sizeof(VertexTextureNormal) * vertexCount);
	
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indices);
}