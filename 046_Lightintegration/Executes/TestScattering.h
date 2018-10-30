#pragma once
#include "Execute.h"

class TestAmbient : public Execute
{
public:
	TestAmbient(ExecuteValues* values);
	~TestAmbient();

	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen(){}

private:

};