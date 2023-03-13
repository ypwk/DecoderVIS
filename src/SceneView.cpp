#include "SceneView.h"


void SceneView::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glViewport(0, 0, mwidth, mheight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneView::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneView::deleteBuffers()
{
    if (buffer) {
        glDeleteFramebuffers(GL_FRAMEBUFFER, &buffer);
        glDeleteTextures(1, &texture);
        texture = 0;
    }
}

void SceneView::recreateBuffers(int width, int height)
{
    mwidth = width;
    mheight = height;

    // free prior used buffers
    if (buffer) {
        deleteBuffers();
    }

    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mwidth, mheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(texture, buffers);

    // unbind
    unbind();
}

void SceneView::render(GenericCode c)
{
    bind();

    c.render();

    unbind();

    ImGui::Begin("CodeScene");
    {
        ImGui::BeginChild("CodeRender");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        glm::vec2 panelSize = { viewportPanelSize.x, viewportPanelSize.y };
        ImGui::Image(reinterpret_cast<void*>(texture), ImVec2{ panelSize.x, panelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::EndChild();
    }


}
