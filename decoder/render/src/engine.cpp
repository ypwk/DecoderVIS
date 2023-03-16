#include "engine.h"
#include "theme.h"

void Engine::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


void Engine::AddCircle(glm::vec3 translation, float radius, float ratio, glm::vec4 color)
{
    for (int i = 0; i < num_vertices; i++) {
        Vertex tmp;
        tmp.Position = vec2{ translation.x + radius * cosf(i * glm::two_pi<float>() / num_vertices)\
            , translation.y + ratio * radius * sinf(i * glm::two_pi<float>() / num_vertices) };
        tmp.Color = vec4{ color.x, color.y, color.z, color.w };

        vertices.push_back(tmp);
    }
    Vertex v;
    v.Position = vec2{ translation.x, translation.y};
    v.Color = vec4{ color.x, color.y, color.z, color.w };
    vertices.push_back(v);

    for (int i = 0; i < num_vertices; i++) {
        indices.push_back((num_vertices + 1) * num_circles + num_vertices);
        indices.push_back((num_vertices + 1) * num_circles + i);
        indices.push_back((num_vertices + 1) * num_circles + (i + 1) % num_vertices);
    }

    num_circles += 1;
}

void Engine::AddQubit(glm::vec3 translation, float ratio, glm::vec4 color) {
    this->AddCircle(translation, 25.0f, ratio, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    this->AddCircle(translation, 20.0f, ratio, color);
}

void Engine::Render() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_IndexBuffer));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, indices.size() * sizeof(indices[0]), &indices[0]));

    m_Shader.Bind();
    glm::mat4 mvp = qubit_Proj * m_View;
    m_Shader.SetUniformMat4f("u_MVP", mvp);

    GLCall(glBindVertexArray(m_VertexArray));
    GLCall(glDrawElements(GL_TRIANGLES, (int) indices.size() * sizeof(indices[0]), GL_UNSIGNED_INT, nullptr));

    num_circles = 0;
    vertices.clear();
    indices.clear();
}