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

	glm::mat4 m_Proj = glm::ortho(0.0f, 3840.0f, 0.0f, 2160.0f, -1.0f, 1.0f);
	glm::vec3 m_Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_Scale = 1.0f;

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	float initialScale = 4;

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
	glm::vec3 GetTranslation() {
		return m_Translation;
	};
	void AddQuad(glm::vec3 translation, float w, float h, float angle, glm::vec4 color);
	void AddLine(glm::vec3 start, glm::vec3 end, float thickness, glm::vec4 color);

	void AddCircle(glm::vec3 translation, float radius, glm::vec4 color);
	void AddSemiCircle(glm::vec3 translation, float radius, float angle, glm::vec4 color);
	void AddSemiCircleArc(glm::vec3 translation, float radius, float thickness, float angle, glm::vec4 color);
};

