#include "vertex_array.h"

#include "vertex_buffer_layout.h"
#include "renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	//std::cout << "created a vertex array!" << m_RendererID << std::endl;
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// bind vertex array
	Bind();
	// bind vertex buffer
	vb.Bind();

	// set up layout array
	const auto& elems = layout.GetElements();
	int offset = 0;
	for (unsigned int i = 0; i < elems.size(); i++) {
		const auto& element = elems[i];
		// std::cout << "add buffer loop " << element.count << " " << element.type << " " << element.normalized << std::endl;
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
