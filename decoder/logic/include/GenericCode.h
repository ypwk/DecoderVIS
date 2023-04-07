#pragma once
#include "engine.h"

class GenericCode
{
public:
	
	enum QubitState {
		NORMAL = 0,
		PARITY_ERROR_X = 1,
		PARITY_ERROR_Z = 2,
		DATA_ERROR_X = 3,
		DATA_ERROR_Y = 4,
		DATA_ERROR_Z = 5,
		TIME_ERROR = 6
	};

	struct Qubit {
		int index;
		QubitState state;
	};

	enum StabilizerState {
		X_STABILIZER = 0,
		Z_STABILIZER = 1,
	};

	struct Stabilizer {
		int index;
		StabilizerState type;
		std::vector<int> qubits;
	};	

	glm::vec4 Q_STATE_TO_COLOR[7] = {
		glm::vec4(1, 1, 1, 1),			//NORMAL
		glm::vec4(1, 0, 0, 0.7f),		//PARITY_ERROR_X,
		glm::vec4(1, 0, 0, 0.7f),		//PARITY_ERROR_Z,
		glm::vec4(1, 0, 0, 0.7f),		//DATA_ERROR_X,
		glm::vec4(0, 1, 0, 0.7f),		//DATA_ERROR_Y,
		glm::vec4(0, 0, 1, 0.7f),		//DATA_ERROR_Z,
		glm::vec4(1, 0, 1, 0.7f),		//TIME_ERROR
	};

	glm::vec4 STABILIZER_STATE_TO_COLOR[2] = {
		glm::vec4(1, 0, 0, 0.4f),
		glm::vec4(0, 0, 1, 0.4f)
	};

	glm::vec4 LINE_COLOR = glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f);
	glm::vec4 BACK_STAB = glm::vec4(1, 1, 1, 0.1f);

	std::vector<Qubit> dataQubits;
	std::vector<Stabilizer> measurementQubits;

	virtual void render(Engine* e) = 0;
	virtual void generateNextStep() {};

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

protected:
	float RENDER_UNIT_LENGTH = 1000.0f;
	float QUBIT_SIZE_INNER = 40.0f;
	float QUBIT_SIZE_OUTER = 50.0f;
	virtual void AddQubitToRender(Qubit q, Engine* e) = 0;
	virtual void AddStabilizerToRender(Stabilizer s, StabilizerState ss, Engine* e) = 0;

private:
	float errorRate = 0.1f;
};

