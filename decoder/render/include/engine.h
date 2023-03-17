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

enum QubitState {
	NORMAL = 0,
	PARITY_ERROR_X = 1,
	PARITY_ERROR_Z = 2,
	DATA_ERROR_X = 3,
	DATA_ERROR_Z = 4,
	TIME_ERROR = 5
};


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

	glm::mat4 m_Proj = glm::ortho(0.0f, 3840.0f, 0.0f, 2160.0f, -1.0f, 1.0f);
	glm::vec3 m_View = glm::vec3(0.0f, 0.0f, 0.0f);
	float my_scale = 1.0f;

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	const int circle_vertex_num = 32;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Shader m_Shader = Shader("res/shaders/Color.shader");

public:
	Engine();
	~Engine() {}

	void UpdateView(float scale, glm::vec3 translation);
	void UpdateAspectRatio();
	void Render();
	void Clear();
	glm::vec3 GetView() {
		return m_View;
	};

	void AddQubit(glm::vec3 translation, float ratio, QubitState qs);

	void AddCircle(glm::vec3 translation, float radius, float ratio, glm::vec4 color);
	void AddSemiCircle(glm::vec3 translation, float radius, float ratio, float angle, glm::vec4 color);
	void AddQuad(glm::vec3 translation, float w, float h, float ratio, float angle, glm::vec4 color);
	void AddLine(glm::vec3 start, glm::vec3 end, float thickness, float ratio, glm::vec4 color);
};

