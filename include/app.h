#pragma once

#include "imgui.h"
#include <RotatedPlanarCode.h>
#include <Controller.h>
#include <SceneView.h>

class App
{
public:
	App();
	~App() {};

	void run();
	void set_current_code(char* new_code);
	void handle_input();

private: 
	int qubits = 16;
	float currentErrorRate = 10;
	GenericCode current_code;
	Controller controller;
	Window window;
	SceneView scene;
};

