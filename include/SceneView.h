#pragma once

#define GLEW_STATIC // This must be defined when statically linking to GLEW
#include <GL/glew.h>

#pragma comment (lib, "glew32s.lib") // Link to glew32s.lib

#include <glm/vec2.hpp>
#include <GenericCode.h>

class SceneView
{
public:
	SceneView() {};
	~SceneView() {};
	void bind();
	void unbind();
	void deleteBuffers();
	void recreateBuffers(int width, int height);
	void render(GenericCode c);

private:
	unsigned int buffer = 0;
	unsigned int texture = 0;
	int mheight = 0;
	int mwidth = 0;
};

