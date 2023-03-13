#pragma once
#include <GenericCode.h>
#include <Decoder.h>
#include <glm/vec2.hpp>

class RotatedPlanarCode : public GenericCode
{
public:
	RotatedPlanarCode(int dist);
	~RotatedPlanarCode() {};

	struct CodeState {};
	
	unsigned int render();
	void generateState(int duration) {};
	void decodeState(Decoder d) {};

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

private:
	int distance = 3;
	float errorRate = 0.1;
	int duration = 10;
};

