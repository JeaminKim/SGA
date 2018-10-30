#include "stdafx.h"
#include "GameAnimModel.h"
#include "../Model/ModelClip.h"
#include "../Model/ModelTweener.h"

GameAnimModel::GameAnimModel(wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile)
	: GameModel(matFolder, matFile, meshFolder, meshFile)
{
	model->Buffer()->UseBlend(true);

	tweener = new ModelTweener();
}

GameAnimModel::~GameAnimModel()
{
	for (ModelClip* clip : clips)
		SAFE_DELETE(clip);

	SAFE_DELETE(tweener);
}

void GameAnimModel::Update()
{
	if (clips.size() < 1) return;

	CalcPosition();
	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);
		
		tweener->UpdateBlending(bone, Time::Delta());
	}

	__super::Update();
}

UINT GameAnimModel::AddClip(wstring file, float startTime)
{
	ModelClip* clip = new ModelClip(file);
	clips.push_back(clip);

	return clips.size() - 1;
}

void GameAnimModel::LockRoot(UINT index, bool val) 
{ 
	clips[index]->LockRoot(val); 
}

void GameAnimModel::Repeat(UINT index, bool val) 
{ 
	clips[index]->Repeat(val); 
}

void GameAnimModel::Speed(UINT index, float val) 
{
	clips[index]->Speed(val); 
}

void GameAnimModel::Play(UINT index, bool bRepeat, float blendTime, float speed, float startTime)
{
	tweener->Play(clips[index], bRepeat, blendTime, speed, startTime);
}
