#include "simulation.h"

void Simulation::render()
{
	m_Code->render(m_RenderEngine);
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
	// propagate errors
	for (auto& qubit : m_Code->dataQubits){
		if (((float) rand()) / (float)RAND_MAX < errorRate) {
			int err = rand() % 3;
			if (err == 0) {
				m_Code->applyOperator(&qubit, 'X');
			}
			else if (err == 1) {
				m_Code->applyOperator(&qubit, 'Z');
			}
			else {
				m_Code->applyOperator(&qubit, 'X');
				m_Code->applyOperator(&qubit, 'Z');
			}
		}
	}
	return true;
}

bool Simulation::assembleErrorGraph()
{
	if (detailedExecution) {
		for (auto& qubit : m_Code->dataQubits) {
			if (((float)rand()) / (float)RAND_MAX < errorRate) {
				int err = rand() % 3;
				if (err == 0) {
					m_Code->applyOperator(&qubit, 'X');
				}
				else if (err == 1) {
					m_Code->applyOperator(&qubit, 'Z');
				}
				else {
					m_Code->applyOperator(&qubit, 'X');
					m_Code->applyOperator(&qubit, 'Z');
				}
			}
		}
	}
	else {

	}
	return true;
}

bool Simulation::decodeErrorGraph()
{
	if (detailedExecution) {

	}
	else {

	}
	return true;
}

bool Simulation::fixErrors()
{
	if (detailedExecution) {

	}
	else {

	}
	return true;
}
