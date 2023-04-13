#include "engine.h"
#include "theme.h"

Engine::Engine() {
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    GLCall(glCreateVertexArrays(1, &m_VertexArray));
    GLCall(glBindVertexArray(m_VertexArray));

    GLCall(glGenBuffers(1, &m_VertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 100000, nullptr, GL_DYNAMIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

    GLCall(glGenBuffers(1, &m_IndexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 300000, nullptr, GL_DYNAMIC_DRAW));
}


void Engine::UpdateView(float scale, glm::vec3 translation)
{
    m_Translation = translation;
    m_Scale = scale;
}

void Engine::UpdateAspectRatio() {
    ImVec2 view = ImGui::GetContentRegionAvail();
    m_Proj = glm::ortho(0.0f, view.x * initialScale , 0.0f, view.y * initialScale, -1.0f, 1.0f);
}

void Engine::Render() {
    if (vertices.size() > 0) {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_IndexBuffer));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, indices.size() * sizeof(indices[0]), &indices[0]));

        // bind to shader, apply projection, view, model transformations
        m_Shader.Bind();
        glm::mat4 mvp = glm::translate(glm::scale(m_Proj, glm::vec3(m_Scale, m_Scale, 0)), m_Translation);
        m_Shader.SetUniformMat4f("u_MVP", mvp);

        GLCall(glBindVertexArray(m_VertexArray));
        GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

        vertices.clear();
        indices.clear();
    }
}

void Engine::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Engine::AddCircle(glm::vec3 translation, float radius, glm::vec4 color)
{
    int curr_first_idx = (int) vertices.size();

    for (int i = 0; i < circle_vertex_num; i++) {
        Vertex tmp;
        tmp.Position = vec2{ translation.x + radius * cosf(i * glm::two_pi<float>() / circle_vertex_num)\
            , translation.y + radius * sinf(i * glm::two_pi<float>() / circle_vertex_num) };
        tmp.Color = vec4{ color.x, color.y, color.z, color.w };

        vertices.push_back(tmp);
    }
    Vertex v;
    v.Position = vec2{ translation.x, translation.y};
    v.Color = vec4{ color.x, color.y, color.z, color.w };
    vertices.push_back(v);

    for (int i = 0; i < circle_vertex_num; i++) {
        indices.push_back(curr_first_idx + circle_vertex_num);
        indices.push_back(curr_first_idx + i);
        indices.push_back(curr_first_idx + (i + 1) % circle_vertex_num);
    }
}

void Engine::AddSemiCircle(glm::vec3 translation, float radius, float angle, glm::vec4 color) 
{
    int curr_first_idx = (int) vertices.size();
    float rad_ang = angle * (glm::pi<float>() / 180);

    for (int i = 0; i < (circle_vertex_num * 2); i++) {
        Vertex tmp;
        tmp.Position = vec2{ translation.x + radius * cosf(rad_ang + i * glm::two_pi<float>() / (circle_vertex_num * 2))\
            , translation.y + radius * sinf(rad_ang + i * glm::two_pi<float>() / (circle_vertex_num * 2)) };
        tmp.Color = vec4{ color.x, color.y, color.z, color.w };

        vertices.push_back(tmp);
    }
    Vertex v;
    v.Position = vec2{ translation.x, translation.y };
    v.Color = vec4{ color.x, color.y, color.z, color.w };
    vertices.push_back(v);

    for (int i = 0; i < circle_vertex_num; i++) {
        indices.push_back(curr_first_idx + (circle_vertex_num * 2));
        indices.push_back(curr_first_idx + i);
        indices.push_back(curr_first_idx + (i + 1) % (circle_vertex_num * 2));
    }
}

void Engine::AddQuad(glm::vec3 translation, float w, float h, float angle, glm::vec4 color) {
    int curr_first_idx = (int)vertices.size();
    float rad_ang = angle * (glm::pi<float>() / 180);

    Vertex v0;
    v0.Position = vec2{ w / 2 * cosf(rad_ang) - h / 2 * sinf(rad_ang) + translation.x, \
                        w / 2 * sinf(rad_ang) + h / 2 * cosf(rad_ang) + translation.y };
    v0.Color = vec4{ color.x, color.y, color.z, color.w };

    Vertex v1;
    v1.Position = vec2{ - w / 2 * cosf(rad_ang) - h / 2 * sinf(rad_ang) + translation.x, \
                        - w / 2 * sinf(rad_ang) + h / 2 * cosf(rad_ang) + translation.y };
    v1.Color = vec4{ color.x, color.y, color.z, color.w };

    Vertex v2;
    v2.Position = vec2{ - w / 2 * cosf(rad_ang) + h / 2 * sinf(rad_ang) + translation.x, \
                        - w / 2 * sinf(rad_ang) - h / 2 * cosf(rad_ang) + translation.y };
    v2.Color = vec4{ color.x, color.y, color.z, color.w };

    Vertex v3;
    v3.Position = vec2{ w / 2 * cosf(rad_ang) + h / 2 * sinf(rad_ang) + translation.x, \
                        w / 2 * sinf(rad_ang) - h / 2 * cosf(rad_ang) + translation.y };
    v3.Color = vec4{ color.x, color.y, color.z, color.w };

    vertices.insert(vertices.end(), { v3, v0, v1, v2 });

    indices.insert(indices.end(), { curr_first_idx, curr_first_idx + 1, curr_first_idx + 2 \
                                    , curr_first_idx + 2, curr_first_idx + 3, curr_first_idx });
}

void Engine::AddLine(glm::vec3 start, glm::vec3 end, float thickness, glm::vec4 color) {
    if (end.y - start.y < 0) { // this is so dirty and bad
        glm::vec3 tmp(start.x, start.y, start.z);
        start = end;
        end = tmp;
    }
    glm::vec3 midpoint((start.x + end.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2);
    float dist = glm::length(end - start);  
    this->AddQuad(midpoint, thickness, dist, acos((end.x - start.x) / (dist)) * 180.0f / glm::pi<float>() + 90, color);
}

void Engine::AddSemiCircleArc(glm::vec3 translation, float radius, float thickness, float angle, glm::vec4 color) {
    int curr_first_idx = (int)vertices.size();
    float rad_ang = angle * (glm::pi<float>() / 180);

    for (int i = 0; i < circle_vertex_num; i++) {
        this->AddLine(glm::vec3(translation.x + radius * cosf(rad_ang + i * glm::two_pi<float>() / (2 * circle_vertex_num))\
            , translation.y + radius * sinf(rad_ang + i * glm::two_pi<float>() / (2 * circle_vertex_num)), 0), \
            glm::vec3(translation.x + radius * cosf(rad_ang + (i + 1) * glm::two_pi<float>() / (2 * circle_vertex_num))\
                , translation.y + radius * sinf(rad_ang + (i + 1) * glm::two_pi<float>() / (2 * circle_vertex_num)), 0), \
            thickness, color);
    }
}