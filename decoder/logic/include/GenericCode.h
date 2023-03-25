#pragma once
#include "engine.h"

class GenericCode
{
public:
	struct Qubit {
		int index;
	};

	struct Stabilizer {
		int index;
		std::vector<int> qubits;
	};

	enum QubitState {
		NORMAL = 0,
		PARITY_ERROR_X = 1,
		PARITY_ERROR_Z = 2,
		DATA_ERROR_X = 3,
		DATA_ERROR_Z = 4,
		TIME_ERROR = 5
	};


	enum StabilizerState {
		X_STABILIZER = 0,
		Z_STABILIZER = 1,
	};

	glm::vec4 Q_STATE_TO_COLOR[6] = {
	glm::vec4(1, 1, 1, 1),			//NORMAL
	glm::vec4(1, 0, 0, 0.7f),		//PARITY_ERROR_X,
	glm::vec4(1, 0, 0, 0.7f),		//PARITY_ERROR_Z,
	glm::vec4(1, 0, 0, 0.7f),		//DATA_ERROR_X,
	glm::vec4(1, 0, 0, 0.7f),		//DATA_ERROR_Z,
	glm::vec4(1, 0, 1, 0.7f),		//TIME_ERROR

	};

	glm::vec4 STABILIZER_STATE_TO_COLOR[2] = {
		glm::vec4(1, 0, 0, 0.4f),
		glm::vec4(0, 0, 1, 0.4f)
	};

	glm::vec4 LINE_COLOR = glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f);
	glm::vec4 BACK_STAB = glm::vec4(1, 1, 1, 0.1f);

	virtual void render(Engine* e) = 0;
	virtual void generateNextStep() {};

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

protected:
	float RENDER_UNIT_LENGTH = 1000.0f;
	float QUBIT_SIZE_INNER = 40.0f;
	float QUBIT_SIZE_OUTER = 50.0f;
	virtual void AddQubitToRender(Qubit q, QubitState qs, Engine* e) = 0;
	virtual void AddStabilizerToRender(Stabilizer s, StabilizerState ss, Engine* e) = 0;

private:
	float errorRate = 0.1f;
};

