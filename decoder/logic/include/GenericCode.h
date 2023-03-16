#pragma once
#include "engine.h"

class GenericCode
{
public:
	struct CodeState {};
	virtual void render(Engine& e) = 0;
	virtual void generateState(int duration) {};
	virtual GenericCode::CodeState getState(int timestamp) { return CodeState(); };

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

private:
	float errorRate = 0.1f;
};

