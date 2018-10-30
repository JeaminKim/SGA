#pragma once

class Frustum
{
public:
	Frustum(ExecuteValues* values, float zFar, Camera* camera = NULL, Perspective* perspective = NULL);
	~Frustum();

	void Update();

	bool ContainPoint(D3DXVECTOR3& position);
	bool ContainCube(D3DXVECTOR3& center, float radius);

private:
	ExecuteValues* values;

	D3DXPLANE planes[6];

	float zFar;

	Camera* camera;
	Perspective* perspective;
};