#pragma once

#include <imgui\imgui.h>
#include <iostream>

class CentralNodeHandler
{
private:
	bool open = false;

public:
	CentralNodeHandler() {};
	~CentralNodeHandler() {};
	void Render();
};

