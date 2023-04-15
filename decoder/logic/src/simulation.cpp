#include "simulation.h"

void Simulation::render()
{
	m_Code->render(m_RenderEngine);
	renderGraph();
	
}

void Simulation::doTimeStep()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	bool result = false;
	switch (currentExecutionStep) {
	case 0:
		result = propagateErrors();
		break;
	case 1:
		result = stabilizerMeasurements();
		break;
	case 2:
		result = assembleErrorGraph();
		break;
	case 3:
		result = decodeErrorGraph();
		break;
	case 4:
		result = fixErrors();
		break;
	}
	//auto stop = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	//std::cout << "step: " << currentExecutionStep << " duration: " << duration.count() << std::endl;

	// update current phase of execution if done with current phase
	if (result) {
		currentExecutionStep = (currentExecutionStep + 1) % 5;
	}

	
}

bool Simulation::propagateErrors()
{
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
	return true;
}

bool Simulation::stabilizerMeasurements() 
{
	for (int idx = 0; idx < m_Code->stabilizers.size(); idx++) {
		m_Code->stabilizers[idx].state = GenericCode::StabilizerState(0);
		for (int qubit_idx : m_Code->stabilizers[idx].qubits) {
			if (m_Code->dataQubits[qubit_idx].state.find(m_Code->stabilizers[idx].type ? 'Z' : 'X') != std::string::npos) {
				m_Code->stabilizers[idx].state = GenericCode::StabilizerState((m_Code->stabilizers[idx].state + 1) % 2);
			}
		}
	}
	return true;
}

bool Simulation::assembleErrorGraph()
{
	if (detailedExecution) {
		if (!populated) {
			populateStabilizers();
		}

		for (int idx = current_X_idx + 1; idx < X_err_stabilizers.size(); idx++) {
			X_graph_Edges.push_back(m_X_Graph.addEdge(X_graph_Nodes[current_X_idx], X_graph_Nodes[idx]));
			m_X_CostMap.set(X_graph_Edges[X_graph_Edges.size() - 1], std::ceil(GRAPH_LINE_WIDTH / glm::length(\
				m_Code->GetStabilizerLocation(X_err_stabilizers[current_X_idx]) - \
				m_Code->GetStabilizerLocation(X_err_stabilizers[idx]))));
		}

		for (int idx = current_Z_idx + 1; idx < Z_err_stabilizers.size(); idx++) {
			Z_graph_Edges.push_back(m_Z_Graph.addEdge(Z_graph_Nodes[current_Z_idx], Z_graph_Nodes[idx]));
			m_Z_CostMap.set(Z_graph_Edges[Z_graph_Edges.size() - 1], std::ceil(GRAPH_LINE_WIDTH / glm::length(\
				m_Code->GetStabilizerLocation(Z_err_stabilizers[current_Z_idx]) - \
				m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]))));
		}

		current_X_idx = std::min((int)X_err_stabilizers.size(), current_X_idx + 1);
		current_Z_idx = std::min((int)Z_err_stabilizers.size(), current_Z_idx + 1);
		if (current_X_idx == (int) X_err_stabilizers.size() && current_Z_idx == (int) Z_err_stabilizers.size()) {
			current_Z_idx = 0;
			current_X_idx = 0;
			return true;
		}
		
		return false;
	}
	else {
		if (!populated) {
			populateStabilizers();
		}

		// populate graphs
		for (int x = 0; x < X_err_stabilizers.size(); x++) {
			for (int y = x + 1; y < X_err_stabilizers.size(); y++) {
				X_graph_Edges.push_back(m_X_Graph.addEdge(X_graph_Nodes[x], X_graph_Nodes[y]));
				m_X_CostMap.set(X_graph_Edges[X_graph_Edges.size() - 1], std::ceil(GRAPH_LINE_WIDTH / glm::length(\
					m_Code->GetStabilizerLocation(X_err_stabilizers[x]) - \
					m_Code->GetStabilizerLocation(X_err_stabilizers[y]))));
			}
		}

		for (int x = 0; x < Z_err_stabilizers.size(); x++) {
			for (int y = x + 1; y < Z_err_stabilizers.size(); y++) {
				Z_graph_Edges.push_back(m_Z_Graph.addEdge(Z_graph_Nodes[x], Z_graph_Nodes[y]));
				m_Z_CostMap.set(Z_graph_Edges[Z_graph_Edges.size() - 1], std::ceil(GRAPH_LINE_WIDTH / glm::length(\
					m_Code->GetStabilizerLocation(Z_err_stabilizers[x]) - \
					m_Code->GetStabilizerLocation(Z_err_stabilizers[y]))));
			}
		}

		return true;
	}
}

bool Simulation::decodeErrorGraph()
{
	if (current_X_idx == 0) {
		if (X_err_stabilizers.size() > 0)
			mwpm_X.run();

		if (Z_err_stabilizers.size() > 0)
			mwpm_Z.run();
	}
	if (current_X_idx < 2) {
		current_X_idx++;
		return false;
	}
	else {
		current_X_idx = 0;
		populated = false;
		return true;
	}
}

bool Simulation::fixErrors()
{
	if (detailedExecution) {

	}
	else {

	}
	return true;
}

void Simulation::renderGraph() {
	// render graph if there are stabilizers with things

	if (currentExecutionStep == 2) { // assemble graph
		if (detailedExecution) {
			if (X_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < current_X_idx; idx++) {
					for (int n_idx = idx + 1; n_idx < X_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(X_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(X_err_stabilizers[n_idx]), \
							(m_X_CostMap[X_graph_Edges[(X_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[0]);
					}
				}
			}
			if (Z_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < current_Z_idx; idx++) {
					for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							(m_Z_CostMap[Z_graph_Edges[(Z_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[2]);
					}
				}
			}
		} 
		else {
			if (X_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < X_err_stabilizers.size(); idx++) {
					for (int n_idx = idx + 1; n_idx < X_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(X_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(X_err_stabilizers[n_idx]), \
							(m_X_CostMap[X_graph_Edges[(X_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[0]);
					}
				}
			}
			if (Z_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < Z_err_stabilizers.size(); idx++) {
					for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							(m_Z_CostMap[Z_graph_Edges[(Z_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[2]);
					}
				}
			}
		}
		
	}
	else if (currentExecutionStep == 3 && current_X_idx > 0) { // decode graph
		if (X_err_stabilizers.size() > 0) {
			for (int idx = 0; idx < X_err_stabilizers.size(); idx++) {
				for (int n_idx = idx + 1; n_idx < X_err_stabilizers.size(); n_idx++) {
					if (mwpm_X.matching(X_graph_Edges[(X_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1])) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(X_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(X_err_stabilizers[n_idx]), \
							(m_X_CostMap[X_graph_Edges[(X_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[1]);
					}
				}
			}
		}
		if (Z_err_stabilizers.size() > 0) {
			for (int idx = 0; idx < Z_err_stabilizers.size(); idx++) {
				for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
					if (mwpm_Z.matching(Z_graph_Edges[(Z_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1])) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							(m_Z_CostMap[Z_graph_Edges[(Z_err_stabilizers.size() - idx / 2) * std::floor(idx / 2) + n_idx - 1]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[3]);
					}
				}
			}
		}
	}
}

void Simulation::populateStabilizers() {
	populated = true;

	X_err_stabilizers.clear();
	Z_err_stabilizers.clear();

	X_graph_Edges.clear();
	X_graph_Nodes.clear();
	m_X_Graph.clear();

	Z_graph_Edges.clear();
	Z_graph_Nodes.clear();
	m_Z_Graph.clear();

	for (auto& stabilizer : m_Code->stabilizers) {
		if (stabilizer.state && !stabilizer.type) {
			X_err_stabilizers.push_back(stabilizer);
			X_graph_Nodes.push_back(m_X_Graph.addNode());
		}
	}

	for (auto& stabilizer : m_Code->stabilizers) {
		if (stabilizer.state && stabilizer.type) {
			Z_err_stabilizers.push_back(stabilizer);
			Z_graph_Nodes.push_back(m_Z_Graph.addNode());
		}
	}
}


void Simulation::updateEngineForCode(Input* i) {
	i->currentScale = -1.0f * m_Code->getDistance() * m_Code->getDistance();
	m_RenderEngine->UpdateView(pow(i->scaleQuant, -1.0f * m_Code->getDistance() * m_Code->getDistance()), m_Code->GetDataQubitLocation(m_Code->dataQubits[m_Code->dataQubits.size() - 1]) + glm::vec3(m_Code->RENDER_UNIT_LENGTH, m_Code->RENDER_UNIT_LENGTH, 0));
}