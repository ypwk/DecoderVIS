#pragma once

#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>
#include <vertex_buffer_layout.h>
#include <texture.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
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

	Shader m_Shader = Shader("res/shaders/Texture.shader");
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer = VertexBuffer(qubit_render_dimensions, 4 * 4 * sizeof(float));
	VertexBufferLayout m_VBLayout;
	IndexBuffer m_IB = IndexBuffer(qubit_render_indices, 6);
	Texture m_Texture = Texture("res/textures/iced_chris.png");
	Renderer m_Renderer;

public:
	Engine() {
		m_VBLayout.Push<float>(2);
		m_VBLayout.Push<float>(2);
		m_VertexArray.AddBuffer(m_VertexBuffer, m_VBLayout);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 mvp = qubit_Proj * m_View * model;

		m_Shader.Bind();
		m_Shader.SetUniformMat4f("u_MVP", mvp);

		m_Texture.Bind();
		m_Shader.SetUniform1i("u_Texture", 0);

		m_VertexArray.Unbind();
		m_VertexBuffer.Unbind();
		m_IB.Unbind();
		m_Shader.Unbind();
	}
	~Engine() {};
	void RenderQubit(glm::vec3 translation);
	void Clear();
};

