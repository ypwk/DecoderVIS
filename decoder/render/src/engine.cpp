#include "engine.h"

void Engine::RenderQubit(glm::vec3 translation)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp = qubit_Proj * m_View * model;

    // shader bind
    quadShader.Bind();
    quadShader.SetUniformMat4f("u_MVP", mvp);

    // render draw
    m_Renderer.Draw(quadVertexArray, quadIndexBuffer, quadShader);
}

void Engine::Clear() {
    m_Renderer.Clear();
}

void Engine::InitQuad()
{
    quadVBLayout.Push<float>(2);
    quadVBLayout.Push<float>(2);
    quadVertexArray.AddBuffer(quadVertexBuffer, quadVBLayout);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = qubit_Proj * m_View * model;

    quadShader.Bind();
    quadShader.SetUniformMat4f("u_MVP", mvp);

    quadTexture.Bind();
    quadShader.SetUniform1i("u_Texture", 0);

    quadVertexArray.Unbind();
    quadVertexBuffer.Unbind();
    quadIndexBuffer.Unbind();
    quadShader.Unbind();
}

void Engine::InitCircle()
{
    // gen circle array data
    float c_theta = 0;

    for (int i = 0; i < num_vertices; i++) {
        circle_vertex_array[i * 4] = cosf(i * glm::two_pi<float>() / num_vertices);
        circle_vertex_array[i * 4 + 1] = sinf(i * glm::two_pi<float>() / num_vertices);
        circle_vertex_array[i * 4 + 2] = cosf(i * glm::two_pi<float>() / num_vertices) * 0.5 + 0.5f;
        circle_vertex_array[i * 4 + 3] = sinf(i * glm::two_pi<float>() / num_vertices) * 0.5 + 0.5f;

        c_theta += 360 / num_vertices;
    }
    circle_vertex_array[num_vertices * 4] = 0.0f;
    circle_vertex_array[num_vertices * 4 + 1] = 0.0f;
    circle_vertex_array[num_vertices * 4 + 2] = 0.5f;
    circle_vertex_array[num_vertices * 4 + 3] = 0.5f;

    for (int i = 0; i < num_vertices; i++) {
        circle_index_array[i * 3] = num_vertices;
        circle_index_array[i * 3 + 1] = i;
        circle_index_array[i * 3 + 2] = (i + 1) % num_vertices;
    }

    circleVertexBuffer.Init(circle_vertex_array, 4 * num_vertices * sizeof(float));
    circleIndexBuffer.Init(circle_index_array, 3 * num_vertices);
    circleVBLayout.Push<float>(2);
    circleVBLayout.Push<float>(2);
    circleVertexArray.AddBuffer(circleVertexBuffer, circleVBLayout);
}

void Engine::RenderCircle(glm::vec3 translation, float radius, float ratio)
{

    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius * ratio, radius));
    glm::mat4 mvp = qubit_Proj * m_View * model * scale;

    // shader bind
    quadShader.Bind();
    quadShader.SetUniformMat4f("u_MVP", mvp);

    // render draw
    quadShader.Bind();
    circleVertexArray.Bind();
    circleIndexBuffer.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, circleIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}
