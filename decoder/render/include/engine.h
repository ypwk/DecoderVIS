#pragma once

#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>
#include <vertex_buffer_layout.h>
#include <texture.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include <imgui/imgui.h>


class Engine {
private:
	float qubit_render_dimensions[16] = {
             100.0f,  100.0f, 1.0f,  1.0f,
             -100.0f,  100.0f, 0.0f,  1.0f,
             -100.0f,  -100.0f, 0.0f,  0.0f,
             100.0f,  -100.0f, 1.0f,  0.0f,
    };

    const unsigned int qubit_render_indices[6] = {
            0, 1, 2,
            2, 3, 0
    };
	
	glm::mat4 qubit_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	Shader quadShader = Shader("res/shaders/Texture.shader");
	VertexArray quadVertexArray;
	VertexBuffer quadVertexBuffer = VertexBuffer(qubit_render_dimensions, 4 * 4 * sizeof(float));
	VertexBufferLayout quadVBLayout;
	IndexBuffer quadIndexBuffer = IndexBuffer(qubit_render_indices, 6);
	Texture quadTexture = Texture("res/textures/iced_chris.png");

	const int num_vertices = 32;
	float circle_vertex_array[32 * 4 + 4];
	unsigned int circle_index_array[32 * 3];

	Shader circleShader = Shader("res/shaders/Texture.shader");
	VertexArray circleVertexArray;
	VertexBuffer circleVertexBuffer;
	VertexBufferLayout circleVBLayout;
	IndexBuffer circleIndexBuffer;

	Renderer m_Renderer;

public:
	Engine() {
		InitQuad();
		InitCircle();
	}
	~Engine() {};

	void InitQuad();
	void InitCircle();

	void RenderQubit(glm::vec3 translation);
	void RenderCircle(glm::vec3 translation, float radius, float ratio);
	void Clear();
};

