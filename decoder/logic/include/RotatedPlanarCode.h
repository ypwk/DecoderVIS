#pragma once
#include <GenericCode.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class RotatedPlanarCode : virtual public GenericCode
{
public:

	RotatedPlanarCode(int dist);
	~RotatedPlanarCode() {};
	
	void render(Engine* e);
	void generateNextStep() {};

	glm::vec3 GetDataQubitLocation(Qubit q);
	glm::vec3 GetStabilizerLocation(Stabilizer s);

protected:
	void AddStabilizerToRender(Stabilizer s, Engine* e);
	void AddQubitToRender(Qubit q, Engine* e);

private:
	int distance = 3;
	float errorRate = 0.1f;
};

