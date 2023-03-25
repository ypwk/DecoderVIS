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

protected:
	void AddStabilizerToRender(Stabilizer s, StabilizerState ss, Engine* e);
	void AddQubitToRender(Qubit q, QubitState qs, Engine* e);

private:
	std::vector<Qubit> dataQubits;
	std::vector<Stabilizer> measurementQubits;

	glm::vec3 GetQubitLocation(Qubit q);

	int distance = 3;
	float errorRate = 0.1f;
};

