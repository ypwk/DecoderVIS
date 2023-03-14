#pragma once
#include <GenericCode.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class RotatedPlanarCode : virtual public GenericCode
{
public:
	RotatedPlanarCode(int dist);
	~RotatedPlanarCode() {};

	struct CodeState {};
	
	void render(Engine& e);
	void generateState(int duration) {};

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

private:
	int distance = 3;
	float errorRate = 0.1;
	int duration = 10;
};

