#include "ig_contentwindow.h"

void ImGui_ContentWindowHandler::RenderInit()
{
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    // make texture for fbo
    GLCall(glGenTextures(1, &m_TXT));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TXT));

    // TODO use window size here
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // attach texture to fbo 
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TXT, 0));

    // set up renderbuffer
    GLCall(glGenRenderbuffers(1, &m_RBO));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ImGui_ContentWindowHandler::PreRender()
{
    ImGui::Begin("Visualization");

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);
}

void ImGui_ContentWindowHandler::PostRender()
{
    // second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        reinterpret_cast<void*>(m_TXT), ImVec2(ImGui::GetWindowPos().x + padding,
            ImGui::GetWindowPos().y + padding),
        ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - padding, 
            ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - padding), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
