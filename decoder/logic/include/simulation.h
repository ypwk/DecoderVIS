#pragma once
#include <lemon/list_graph.h>
#include <lemon/matching.h>
#include <lemon/dijkstra.h>

#include "RotatedPlanarCode.h"
#include "decoder.h"
#include "glm/glm.hpp"
#include "input.h"

#include <chrono>

class Simulation
{
private:
	bool needsVisualUpdate = true;
	bool populated = false;

	int current_X_idx = 0;
	int current_Z_idx = 0;

	std::vector<GenericCode::AbstractStabilizer> X_err_stabilizers;
	std::vector<GenericCode::AbstractStabilizer> Z_err_stabilizers;

	GenericCode* m_Code;
	Decoder decoder;
	Engine* m_RenderEngine;

	lemon::ListGraph m_qubitGraph;
	lemon::ListGraph m_X_Graph;
	lemon::ListGraph m_Z_Graph;

	lemon::ListGraph::EdgeMap<int> m_qubitCostMap;
	lemon::ListGraph::EdgeMap<int> m_X_CostMap;
	lemon::ListGraph::EdgeMap<int> m_Z_CostMap;

	std::vector<lemon::ListGraph::Node> qubitGraph_Nodes;
	std::vector<lemon::ListGraph::Edge> qubitGraph_Edges;
	std::vector<lemon::ListGraph::Node> X_graph_Nodes;
	std::vector<lemon::ListGraph::Edge> X_graph_Edges;
	std::vector<lemon::ListGraph::Node> Z_graph_Nodes;
	std::vector<lemon::ListGraph::Edge> Z_graph_Edges;

	lemon::MaxWeightedPerfectMatching<lemon::ListGraph, lemon::ListGraph::EdgeMap<int>> mwpm_X;
	lemon::MaxWeightedPerfectMatching<lemon::ListGraph, lemon::ListGraph::EdgeMap<int>> mwpm_Z;

public:
	Simulation(Engine* e) : m_qubitCostMap(m_qubitGraph), m_X_CostMap(m_X_Graph), m_Z_CostMap(m_Z_Graph), mwpm_X(m_X_Graph, m_X_CostMap), mwpm_Z(m_Z_Graph, m_Z_CostMap) {
		m_Code = new RotatedPlanarCode(codeDistance);
		m_RenderEngine = e;

		for (int qubit_idx = 0; qubit_idx < m_Code->dataQubits.size(); qubit_idx++) {
			qubitGraph_Nodes.push_back(m_qubitGraph.addNode());
			for (int node_idx = 0; node_idx < qubit_idx; node_idx++) {
				qubitGraph_Edges.push_back(m_qubitGraph.addEdge(qubitGraph_Nodes[qubit_idx], qubitGraph_Nodes[node_idx]));
				m_qubitCostMap.set(qubitGraph_Edges[qubitGraph_Edges.size() - 1], -std::ceil(glm::length(\
					m_Code->GetDataQubitLocation(m_Code->dataQubits[qubit_idx]) - \
					m_Code->GetDataQubitLocation(m_Code->dataQubits[qubit_idx])) / GRAPH_LINE_WIDTH));
			}
		}

		srand((int)time(NULL));

		Decoder decoder;
	};
	~Simulation() {};

	int codeDistance = 5;
	float errorRate = 0.01f;
	int delay = 5; // number of frames between each successive step
	int currentExecutionStep = 0;
	bool detailedExecution = true;

	void render();
	void doTimeStep();

	bool propagateErrors();
	bool stabilizerMeasurements();
	bool assembleErrorGraph();
	bool decodeErrorGraph();
	bool fixErrors();

	void updateEngineForCode(Input* i);

private:
	void renderGraph();
	void populateStabilizers();

	int coordToIdx(int n, int x, int y);

	const float GRAPH_LINE_WIDTH = 100.0f;
};