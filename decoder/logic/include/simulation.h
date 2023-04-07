#pragma once
#include <RotatedPlanarCode.h>
#include <decoder.h>

class Simulation
{
public:
	Simulation(Engine* e) {
		currentCode = new RotatedPlanarCode(codeDistance);
		m_RenderEngine = e;

		Decoder decoder;
	};
	~Simulation() {};

	int codeDistance = 5;
	float errorRate = 0.05f;
	int delay = 5; // number of frames between each successive step
	int currentExecutionStep = 0;
	bool detailedExecution = false;

	void render();
	void doTimeStep();

	bool propagateErrors();
	bool assembleErrorGraph();
	bool decodeErrorGraph();
	bool fixErrors();

private:
	bool needsVisualUpdate = true;
	GenericCode* currentCode;
	Decoder decoder;
	Engine* m_RenderEngine;
};

