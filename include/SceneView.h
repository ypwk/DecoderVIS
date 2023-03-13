#pragma once

#include <GL/glew.h>

#define GLEW_STATIC

#pragma comment (lib, "glew32s.lib") // Link to glew32s.lib

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <GenericCode.h>
#include <iostream>
#include <opengl_shader.h>
#include <file_manager.h>

class SceneView
{
public:
	SceneView();
	~SceneView();
	void bind();
	void unbind();
	void deleteBuffers();
	void setup();
	void make_triangle(unsigned int& vbo, unsigned int& vao, unsigned int& ebo);
	void recreateBuffers(int width, int height);
	void render(GenericCode c);

private:
	unsigned int rbuffer = 0;
	unsigned int buffer = 0;
	unsigned int texture = 0;
	int mheight = 0;
	int mwidth = 0;
	GLuint rectVAO = 0;
	GLuint rectVBO = 0;
	unsigned int vbo = 0;
	unsigned int vao = 0;
	unsigned int ebo = 0;
	Shader triangle_shader;
};

