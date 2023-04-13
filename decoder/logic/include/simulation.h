#pragma once
#include "RotatedPlanarCode.h"
#include "decoder.h"
#include "glm/glm.hpp"
#include "input.h"

#include <chrono>
#include <lemon/list_graph.h>

class Simulation
{
private:
	bool needsVisualUpdate = true;
	bool populated = false;

	int current_X_idx = 0;
	int current_Z_idx = 0;

	std::vector<GenericCode::Stabilizer> X_err_stabilizers;
	std::vector<GenericCode::Stabilizer> Z_err_stabilizers;

	GenericCode* m_Code;
	Decoder decoder;
	Engine* m_RenderEngine;

	lemon::ListGraph m_X_Graph;
	lemon::ListGraph m_Z_Graph;
	lemon::ListGraph::EdgeMap<float> m_X_CostMap;
	lemon::ListGraph::EdgeMap<float> m_Z_CostMap;
	std::vector<lemon::ListGraph::Node> X_graph_Nodes;
	std::vector<lemon::ListGraph::Edge> X_graph_Edges;
	std::vector<lemon::ListGraph::Node> Z_graph_Nodes;
	std::vector<lemon::ListGraph::Edge> Z_graph_Edges;

public:
	Simulation(Engine* e) : m_X_CostMap(m_X_Graph), m_Z_CostMap(m_Z_Graph) {
		m_Code = new RotatedPlanarCode(codeDistance);
		m_RenderEngine = e;

		srand((int)time(NULL));

		Decoder decoder;
	};
	~Simulation() {};

	int codeDistance = 5;
	float errorRate = 0.01f;
	int delay = 5; // number of frames between each successive step
	int currentExecutionStep = 0;
	bool detailedExecution = false;

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
	
};

