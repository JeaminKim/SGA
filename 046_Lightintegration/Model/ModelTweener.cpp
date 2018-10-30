#include "stdafx.h"
#include "ModelTweener.h"
#include "ModelClip.h"
#include "ModelBone.h"

ModelTweener::ModelTweener()
	: current(NULL), next(NULL), blendTime(0.0f), elapsedTime(0.0f)
{

}

ModelTweener::~ModelTweener()
{

}

void ModelTweener::Play(ModelClip * clip, bool bRepeat, float blendTime, float speed, float startTime)
{
	this->blendTime = blendTime;
	this->elapsedTime = startTime;


	if (next == NULL)
	{
		next = clip;
		next->Repeat(bRepeat);
		next->StartTime(startTime);
		next->Speed(speed);
	}
	else
	{
		current = clip;
		current->Repeat(bRepeat);
		current->StartTime(startTime);
		current->Speed(speed);
	}
}

void ModelTweener::UpdateBlending(ModelBone * bone, float time)
{
	D3DXMATRIX matrix;
	if (next != NULL)
	{
		elapsedTime += time;
		float t = elapsedTime / blendTime;

		if (t > 1.0f)
		{
			matrix = next->GetKeyframeMatrix(bone, time);

			current = next;
			next = NULL;
		}
		else
		{
			D3DXMATRIX start = current->GetKeyframeMatrix(bone, time);
			D3DXMATRIX end = next->GetKeyframeMatrix(bone, time);

			Math::LerpMatrix(matrix, start, end, t);
		}
	}
	else
	{
		matrix = current->GetKeyframeMatrix(bone, time);
	}

	bone->Local(matrix);
}
