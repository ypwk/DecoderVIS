#include "simulation.h"

void Simulation::render()
{
	currentCode->render(m_RenderEngine);
}

void Simulation::doTimeStep()
{
	bool result = false;
	switch (currentExecutionStep) {
	case 0: 
		result = propagateErrors();
		break;
	case 1:
		result = assembleErrorGraph();
		break;
	case 2:
		result = decodeErrorGraph();
		break;
	case 3:
		result = fixErrors();
		break;
	}
	// update current phase of execution if done with current phase
	if (result) {
		currentExecutionStep = (currentExecutionStep + 1) % 4;
	}
}

bool Simulation::propagateErrors()
{
	if (detailedExecution) {

	}
	else {

	}
	return false;
}

bool Simulation::assembleErrorGraph()
{
	if (detailedExecution) {

	}
	else {

	}
	return false;
}

bool Simulation::decodeErrorGraph()
{
	if (detailedExecution) {

	}
	else {

	}
	return false;
}

bool Simulation::fixErrors()
{
	if (detailedExecution) {

	}
	else {

	}
	return false;
}
