#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID = 0;
public:
	VertexBuffer() {};
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Init(const void* data, unsigned int size);
};

