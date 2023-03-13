#pragma once
#include <Window.h>
#include "imgui.h"

class GenericCode
{
public:
	struct CodeState {};
	virtual unsigned int render();
	virtual void generateState(int duration) {};
	virtual GenericCode::CodeState getState(int timestamp) { return CodeState(); };

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

private:
	float errorRate = 0.1;
};

