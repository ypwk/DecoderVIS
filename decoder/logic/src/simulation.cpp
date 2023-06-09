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
			m_X_CostMap.set(X_graph_Edges[X_graph_Edges.size() - 1], -std::ceil(glm::length(\
				m_Code->GetStabilizerLocation(X_err_stabilizers[current_X_idx]) - \
				m_Code->GetStabilizerLocation(X_err_stabilizers[idx])) / GRAPH_LINE_WIDTH));
		}

		for (int idx = current_Z_idx + 1; idx < Z_err_stabilizers.size(); idx++) {
			Z_graph_Edges.push_back(m_Z_Graph.addEdge(Z_graph_Nodes[current_Z_idx], Z_graph_Nodes[idx]));
			m_Z_CostMap.set(Z_graph_Edges[Z_graph_Edges.size() - 1], -std::ceil(glm::length(\
				m_Code->GetStabilizerLocation(Z_err_stabilizers[current_Z_idx]) - \
				m_Code->GetStabilizerLocation(Z_err_stabilizers[idx])) / GRAPH_LINE_WIDTH));
			//m_Z_CostMap.set(Z_graph_Edges[Z_graph_Edges.size() - 1], -std::ceil(glm::length(\
			//	m_Code->GetStabilizerLocation(Z_err_stabilizers[current_Z_idx]) - \
			//	m_Code->GetStabilizerLocation(Z_err_stabilizers[idx])) / GRAPH_LINE_WIDTH));
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
				m_X_CostMap.set(X_graph_Edges[X_graph_Edges.size() - 1], -std::ceil(glm::length(\
					m_Code->GetStabilizerLocation(X_err_stabilizers[x]) - \
					m_Code->GetStabilizerLocation(X_err_stabilizers[y])) / GRAPH_LINE_WIDTH));
			}
		}

		for (int x = 0; x < Z_err_stabilizers.size(); x++) {
			for (int y = x + 1; y < Z_err_stabilizers.size(); y++) {
				Z_graph_Edges.push_back(m_Z_Graph.addEdge(Z_graph_Nodes[x], Z_graph_Nodes[y]));
				m_Z_CostMap.set(Z_graph_Edges[Z_graph_Edges.size() - 1], -std::ceil(glm::length(\
					m_Code->GetStabilizerLocation(Z_err_stabilizers[x]) - \
					m_Code->GetStabilizerLocation(Z_err_stabilizers[y])) / GRAPH_LINE_WIDTH));
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
		matchings_populated = false;
		return true;
	}
}

bool Simulation::fixErrors()
{
	if (matchings.size() > 0) {
		if (detailedExecution) {
			lemon::ListGraph::Node startingStab = m_qubitGraph.addNode();
			lemon::ListGraph::Node endingStab = m_qubitGraph.addNode();
			if (matchings[current_err_idx].type == 0) { // X
				for (auto& qubit_idx : Z_err_stabilizers[matchings[current_err_idx].a].qubits) {
					lemon::ListGraph::Edge e = m_qubitGraph.addEdge(startingStab, qubitGraph_Nodes[qubit_idx]);
					m_qubitCostMap.set(e, 1);
				}
				for (auto& qubit_idx : Z_err_stabilizers[matchings[current_err_idx].b].qubits) {
					lemon::ListGraph::Edge e = m_qubitGraph.addEdge(endingStab, qubitGraph_Nodes[qubit_idx]);
					m_qubitCostMap.set(e, 1);
				}
				lemon::Dijkstra<lemon::ListGraph, lemon::ListGraph::EdgeMap<int>> dijkstra(m_qubitGraph, m_qubitCostMap);
				dijkstra.run(startingStab, endingStab);
				lemon::ListGraph::Node* cur = &dijkstra.predNode(endingStab);
				std::vector <lemon::ListGraph::Node> toCorrect;
				while (*cur != startingStab) {
					toCorrect.push_back(*cur);
					cur = &dijkstra.predNode(*cur);
				}
				for (int idx = 0; idx < qubitGraph_Nodes.size(); idx++) {
					if (std::find(toCorrect.begin(), toCorrect.end(), qubitGraph_Nodes[idx]) != toCorrect.end()) {
						m_Code->applyOperator(&m_Code->dataQubits[idx], 'Z');
					}
				}
			}
			else {
				for (auto& qubit_idx : X_err_stabilizers[matchings[current_err_idx].a].qubits) {
					lemon::ListGraph::Edge e = m_qubitGraph.addEdge(startingStab, qubitGraph_Nodes[qubit_idx]);
					m_qubitCostMap.set(e, 1);
				}
				for (auto& qubit_idx : X_err_stabilizers[matchings[current_err_idx].b].qubits) {
					lemon::ListGraph::Edge e = m_qubitGraph.addEdge(endingStab, qubitGraph_Nodes[qubit_idx]);
					m_qubitCostMap.set(e, 1);
				}
				lemon::Dijkstra<lemon::ListGraph, lemon::ListGraph::EdgeMap<int>> dijkstra(m_qubitGraph, m_qubitCostMap);
				dijkstra.run(startingStab, endingStab);
				lemon::ListGraph::Node* cur = &dijkstra.predNode(endingStab);
				std::vector <lemon::ListGraph::Node> toCorrect;
				while (*cur != startingStab) {
					toCorrect.push_back(*cur);
					cur = &dijkstra.predNode(*cur);
				}
				for (int idx = 0; idx < qubitGraph_Nodes.size(); idx++) {
					if (std::find(toCorrect.begin(), toCorrect.end(), qubitGraph_Nodes[idx]) != toCorrect.end()) {
						m_Code->applyOperator(&m_Code->dataQubits[idx], 'X');
					}
				}
			}

			current_err_idx++;
			if (current_err_idx == matchings.size()) {
				current_err_idx = 0;
				matchings.clear();

				for (auto& vs : m_Code->virtualStabilizers) {
					vs.state = GenericCode::StabilizerState(0);
				}

				return true;
			}
			return false;
		}
		else {

		}
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
							-(m_X_CostMap[X_graph_Edges[coordToIdx(X_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[0]);
					}
				}
			}
			if (Z_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < current_Z_idx; idx++) {
					for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							-(m_Z_CostMap[Z_graph_Edges[coordToIdx(Z_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[2]);
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
							-(m_X_CostMap[X_graph_Edges[coordToIdx(X_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[0]);
					}
				}
			}
			if (Z_err_stabilizers.size() > 0) {
				for (int idx = 0; idx < Z_err_stabilizers.size(); idx++) {
					for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							-(m_Z_CostMap[Z_graph_Edges[coordToIdx(Z_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[2]);
					}
				}
			}
		}
		
	}
	else if (currentExecutionStep == 3 && current_X_idx > 0) { // decode graph
		if (X_err_stabilizers.size() > 0) {
			for (int idx = 0; idx < X_err_stabilizers.size(); idx++) {
				for (int n_idx = idx + 1; n_idx < X_err_stabilizers.size(); n_idx++) {
					if (mwpm_X.matching(X_graph_Edges[coordToIdx(X_err_stabilizers.size(), idx, n_idx)])) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(X_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(X_err_stabilizers[n_idx]), \
							-(m_X_CostMap[X_graph_Edges[coordToIdx(X_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[1]);
						if(!matchings_populated)
							matchings.push_back(Matching { idx, n_idx, GenericCode::StabilizerType(1)}); // Z type error for X stabilizer
					}
				}
			}
		}
		if (Z_err_stabilizers.size() > 0) {
			for (int idx = 0; idx < Z_err_stabilizers.size(); idx++) {
				for (int n_idx = idx + 1; n_idx < Z_err_stabilizers.size(); n_idx++) {
					if (mwpm_Z.matching(Z_graph_Edges[coordToIdx(Z_err_stabilizers.size(), idx, n_idx)])) {
						m_RenderEngine->AddLine(m_Code->GetStabilizerLocation(Z_err_stabilizers[idx]), \
							m_Code->GetStabilizerLocation(Z_err_stabilizers[n_idx]), \
							-(m_Z_CostMap[Z_graph_Edges[coordToIdx(Z_err_stabilizers.size(), idx, n_idx)]]), m_Code->STABILIZER_TYPE_AND_STATE_TO_COLOR[3]);
						if (!matchings_populated)
							matchings.push_back(Matching{ idx, n_idx, GenericCode::StabilizerType(0) }); // X type error for Z stabilizer
					}
				}
			}
		}
		if (!matchings_populated)
			matchings_populated = true;
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

	// utilize virtual stabilizers if there is an odd number of stabilizers that detected errors
	if (X_err_stabilizers.size() % 2 == 1) { 
		float max_dist = 0;
		GenericCode::VirtualStabilizer* max_stab = nullptr;
		for (auto& stabilizer : m_Code->virtualStabilizers) {
			if (stabilizer.type == X_err_stabilizers[X_err_stabilizers.size() - 1].type) {
				stabilizer.state = GenericCode::StabilizerState(0);
				float dist = glm::length(\
					m_Code->GetStabilizerLocation(stabilizer) - \
					m_Code->GetStabilizerLocation(X_err_stabilizers[X_err_stabilizers.size() - 1]));
				if (dist > max_dist) {
					max_dist = dist;
					max_stab = &stabilizer;
					max_stab->state = GenericCode::StabilizerState(1);
				}
			}
		}
		X_err_stabilizers.push_back(*max_stab);
		X_graph_Nodes.push_back(m_X_Graph.addNode());
	}

	for (auto& stabilizer : m_Code->stabilizers) {
		if (stabilizer.state && stabilizer.type) {
			Z_err_stabilizers.push_back(stabilizer);
			Z_graph_Nodes.push_back(m_Z_Graph.addNode());
		}
	}

	if (Z_err_stabilizers.size() % 2 == 1) {
		float max_dist = 10000000.0f;
		GenericCode::VirtualStabilizer* max_stab = nullptr;
		for (auto& stabilizer : m_Code->virtualStabilizers) {
			if (stabilizer.type == Z_err_stabilizers[Z_err_stabilizers.size() - 1].type) {
				float dist = glm::length(\
					m_Code->GetStabilizerLocation(stabilizer) - \
					m_Code->GetStabilizerLocation(Z_err_stabilizers[Z_err_stabilizers.size() - 1]));
				if (dist < max_dist && dist > 0) {
					max_dist = dist;
					max_stab = &stabilizer;
				}
			}
		}
		max_stab->state = GenericCode::StabilizerState(1);
		Z_err_stabilizers.push_back(*max_stab);
		Z_graph_Nodes.push_back(m_Z_Graph.addNode());
	}
}

void Simulation::updateEngineForCode(Input* i) {
	i->currentScale = -1.0f * m_Code->getDistance() * m_Code->getDistance();
	m_RenderEngine->UpdateView(pow(i->scaleQuant, -1.0f * m_Code->getDistance() * m_Code->getDistance()), m_Code->GetDataQubitLocation(m_Code->dataQubits[m_Code->dataQubits.size() - 1]) + glm::vec3(m_Code->RENDER_UNIT_LENGTH, m_Code->RENDER_UNIT_LENGTH, 0));
}

int Simulation::coordToIdx(int n, int x, int y) {
	return -0.5 * (x + 1) * (x + 1) + (n + 0.5) * (x + 1) - n + y - x - 1;
}