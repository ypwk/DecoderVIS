#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

