#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const { return m_Count; }
};

