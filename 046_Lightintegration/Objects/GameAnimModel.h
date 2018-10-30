#include "GameModel.h"

class GameAnimModel : public GameModel
{
public:
	GameAnimModel
	(
		wstring matFolder, wstring matFile
		, wstring meshFolder, wstring meshFile
	);
	virtual ~GameAnimModel();

	virtual void Update();

	UINT AddClip(wstring file, float startTime = 0.0f);

	void LockRoot(UINT index, bool val);
	void Repeat(UINT index, bool val);
	void Speed(UINT index, float val);

	void Play
	(
		UINT index
		, bool bRepeat = false
		, float blendTime = 0.0f
		, float speed = 1.0f
		, float startTime = 0.0f
	);

private:
	vector<class ModelClip *> clips;
	
	class ModelTweener* tweener;
};