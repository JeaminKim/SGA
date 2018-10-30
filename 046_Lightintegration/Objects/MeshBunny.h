#pragma once
#include "GameModel.h"

class MeshBunny : public GameModel
{
public:
	MeshBunny();
	~MeshBunny();

	void Update() override;
	void Render() override;

private:
	
};