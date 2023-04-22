#pragma once
#include "engine.h"

class GenericCode
{
public:

	struct Qubit {
		int index;
		std::string state = "+";
	};

	enum StabilizerType {
		X_STABILIZER = 0,
		Z_STABILIZER = 1,
	};

	enum StabilizerState {
		NORMAL = 0,
		DETECTED_ERROR = 1,
	};

	struct AbstractStabilizer {
		AbstractStabilizer::AbstractStabilizer(int i, StabilizerType t, StabilizerState s) {
			index = i;
			type = t;
			state = s;
		}
		virtual ~AbstractStabilizer() { }
		int index;
		StabilizerType type;
		StabilizerState state;
		std::vector<int> qubits;
		int stab_class = -1;
	};

	struct Stabilizer: public AbstractStabilizer {
		Stabilizer(int index, StabilizerType type, StabilizerState state) : AbstractStabilizer::AbstractStabilizer{ index, type, state } {
			stab_class = 0;
		}
	};

	struct VirtualStabilizer: public AbstractStabilizer {
		VirtualStabilizer(int index, StabilizerType type, StabilizerState state) : AbstractStabilizer::AbstractStabilizer{ index, type, state } {
			stab_class = 1;
		}
	};

	glm::vec4 Q_STATE_TO_COLOR[4] = {
		glm::vec4(1, 1, 1, 1),			//NORMAL
		glm::vec4(1, 0, 0, 0.7f),		//X ERROR
		glm::vec4(0, 0, 1, 0.7f),		//Z ERROR
		glm::vec4(1, 0, 1, 0.7f),		//XZ ERROR
	};

	glm::vec4 STABILIZER_TYPE_AND_STATE_TO_COLOR[4] = {
		glm::vec4(1, 0, 0, 0.4f),
		glm::vec4(1, 1, 0, 0.4f),
		glm::vec4(0, 0, 1, 0.4f),
		glm::vec4(0, 1, 1, 0.4f),
	};

	glm::vec4 LINE_COLOR = glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f);
	glm::vec4 BACK_STAB = glm::vec4(1, 1, 1, 0.1f);
	float RENDER_UNIT_LENGTH = 1000.0f;

	std::vector<Qubit> dataQubits;
	std::vector<Stabilizer> stabilizers;
	std::vector<VirtualStabilizer> virtualStabilizers;

	virtual void render(Engine* e) = 0;
	virtual void generateNextStep() {};

	void setErrorRate(float er) { errorRate = er; };
	float getErrorRate() { return errorRate; };

	void applyOperator(Qubit* q, char e) {
		bool neg = q->state.at(0) == '-';
		// check for combinations
		q->state.push_back(e);
		if (q->state.length() >= 3) {
			if (q->state.at(q->state.length() - 1) == q->state.at(q->state.length() - 2)) {
				q->state.replace(q->state.length() - 2, 2, "");
			}
			else if (q->state.length() > 3) {
				if (q->state.substr(q->state.length() - 3, 3) == "XZX") {
					neg = !neg;
					q->state.replace(q->state.length() - 3, 3, "Z");
				}
				else if (q->state.substr(q->state.length() - 3, 3) == "ZXZ") {
					neg = !neg;
					q->state.replace(q->state.length() - 3, 3, "X");
				}
			}
		}
		q->state.replace(0, 1, neg ? "-" : "+");
	};

	virtual glm::vec3 GetDataQubitLocation(Qubit q) = 0;
	virtual glm::vec3 GetStabilizerLocation(AbstractStabilizer s) = 0;

	int getDistance() {
		return distance;
	}

protected:
	float QUBIT_SIZE_INNER = 40.0f;
	float QUBIT_SIZE_OUTER = 50.0f;
	virtual void AddQubitToRender(Qubit q, Engine* e) = 0;
	virtual void AddStabilizerToRender(Stabilizer s, Engine* e) = 0;

private:
	float errorRate = 0.1f;
	int distance = 3;
};

