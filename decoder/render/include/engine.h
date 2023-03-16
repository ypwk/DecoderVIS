#pragma once

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"
#include "vertex_buffer_layout.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <imgui/imgui.h>


class Engine {
private:
	struct vec2 {
		float x;
		float y;
	};

	struct vec4 {
		float x;
		float y;
		float z;
		float w;
	};

	struct Vertex {
		vec2 Position;
		vec4 Color;
	};

	glm::mat4 qubit_Proj = glm::ortho(0.0f, 3840.0f, 0.0f, 2160.0f, -1.0f, 1.0f); // 4k resolution
	glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	const unsigned int num_circle_size = 100;

	int num_circles = 0;
	const int num_vertices = 32;
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Shader m_Shader = Shader("res/shaders/Color.shader");

public:
	Engine() {
		GLCall(glCreateVertexArrays(1, &m_VertexArray));
		GLCall(glBindVertexArray(m_VertexArray));

		GLCall(glGenBuffers(1, &m_VertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 33 * num_circle_size, nullptr, GL_DYNAMIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, Position)));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, Color)));

		GLCall(glGenBuffers(1, &m_IndexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_vertices * 3 * num_circle_size, nullptr, GL_DYNAMIC_DRAW));
	}
	~Engine() {};

	void AddQubit(glm::vec3 translation, float ratio, glm::vec4 color);
	void AddCircle(glm::vec3 translation, float radius, float ratio, glm::vec4 color);

	void Render();
	void Clear();
};

