#include "engine.h"

void Engine::RenderQubit(glm::vec3 translation)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp = qubit_Proj * m_View * model;

    // shader bind
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("u_MVP", mvp);

    // render draw
    m_Renderer.Draw(m_VertexArray, m_IB, m_Shader);
}

void Engine::Clear() {
    m_Renderer.Clear();
}

