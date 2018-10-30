#pragma once

class QuadTree
{
public:
	static const UINT DivideCount;

private:
	struct NodeType;

public:
	QuadTree(ExecuteValues* values, class Terrain* terrain);
	~QuadTree();

	void Update();
	void Render();

private:
	void RenderNode(NodeType* node);
	void DeleteNode(NodeType* node);

	void CalcMeshDimensions(UINT vertexCount, float& centerX, float& centerZ, float& meshWidth);
	void CreateTreeNode(NodeType* node, float positionX, float positionZ, float width);

	UINT ContainTriangleCount(float positionX, float positionZ, float width);
	bool IsTriangleContained(UINT index, float positionX, float positionZ, float width);

private:
	ExecuteValues* values;

	UINT triangleCount, drawCount;
	VertexTextureNormal* vertices;

	NodeType* parent;
	class Frustum* frustum;

	WorldBuffer* worldBuffer;
	Material* material;

	RasterizerState* rasterizer[2];
	//Perspective* perspective;

private:
	struct NodeType
	{
		float X, Z, Width;
		UINT TriangleCount;
		ID3D11Buffer* VertexBuffer, *IndexBuffer;

		NodeType* Childs[4];
	};

private:
	class LineBuffer : public ShaderBuffer
	{
	public:
		LineBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Type = 2;
			Data.Color = D3DXVECTOR3(1, 1, 1);

			Data.Spacing = 5;
			Data.Thickness = 0.1f;
		}

		struct Struct
		{
			int Type;
			D3DXVECTOR3 Color;

			int Spacing;
			float Thickness;

			float Padding[2];
		} Data;
	};
	LineBuffer* lineBuffer;
};