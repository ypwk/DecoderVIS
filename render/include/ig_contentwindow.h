#pragma once
#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>
#include <vertex_buffer_layout.h>
#include <imgui/imgui.h>

class ImGui_ContentWindowHandler
{
private:
	unsigned int m_Width = 3840;
	unsigned int m_Height = 2160;

	unsigned int m_FBO = 0;
	GLuint m_TXT = 0;

	unsigned int ms_FBO = 0;
	GLuint ms_TXT = 0;

	int samples = 4;

	GLuint padding = 10;

	const float quad_positions[8] = { 0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	const unsigned int quad_indices[6] = { 0, 1, 2, 2, 3, 0 };

	Shader m_Shader = Shader("res/shaders/Fbo.shader");
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer = VertexBuffer(quad_positions, 8 * sizeof(float));
	VertexBufferLayout m_VBLayout;
	IndexBuffer m_IB = IndexBuffer(quad_indices, 6);
	
public:
	ImGui_ContentWindowHandler(unsigned int w, unsigned int h) : m_Width(w), m_Height(h) { 
		// set up vertex array object for rendering fbo
		m_VBLayout.Push<float>(2);
		m_VertexArray.AddBuffer(m_VertexBuffer, m_VBLayout);
	};
	~ImGui_ContentWindowHandler() {
		GLCall(glDeleteFramebuffers(1, &m_FBO));
	}
	void RenderInit();
	void PreRender();
	void PostRender();
};

