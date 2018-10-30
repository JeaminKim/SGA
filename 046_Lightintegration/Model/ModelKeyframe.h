#pragma once

class ModelKeyframe
{
public:
	struct Transform;
	friend class ModelClip;

	ModelKeyframe();
	~ModelKeyframe();

public:
	D3DXMATRIX GetInterpolatedMatrix(float time, bool bRepeat);

private:
	UINT GetKeyframeIndex(float time);
	void CalcKeyframeIndex
	(
		float time
		, bool bRepeat
		, OUT UINT& index1
		, OUT UINT& index2
		, OUT float& interpolatedTime
	);

	D3DXMATRIX GetInterpolatedMatrix(UINT index1, UINT index2, float t);

private:
	wstring boneName;

	float duration;
	float frameRate;
	UINT frameCount;

	vector<Transform> transforms;

public:
	struct Transform
	{
		float Time;

		D3DXVECTOR3 S;
		D3DXQUATERNION R;
		D3DXVECTOR3 T;
	};
};