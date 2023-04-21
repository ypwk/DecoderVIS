#include "ig_contentwindow.h"

void ImGui_ContentWindowHandler::RenderInit()
{
    // create multisample FBOs with textures
    GLCall(glGenFramebuffers(1, &ms_back_FBO));
    glBindFramebuffer(GL_FRAMEBUFFER, ms_back_FBO);

    GLCall(glGenTextures(1, &ms_back_TXT));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_back_TXT));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, m_Width, m_Height, GL_TRUE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

    // attach texture
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ms_back_TXT, 0));

    GLCall(glGenFramebuffers(1, &ms_front_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ms_front_FBO));

    GLCall(glGenTextures(1, &ms_front_TXT));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_front_TXT));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, m_Width, m_Height, GL_TRUE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    
    // attach texture to ms_fbo
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ms_front_TXT, 0));

    // Unbind the FBO
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // make FBO to write to texture
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    // make texture for fbo
    GLCall(glGenTextures(1, &m_TXT));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TXT));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // attach texture to fbo 
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TXT, 0));
}

void ImGui_ContentWindowHandler::PreRender()
{
    ImGui::Begin("Visualization");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ms_back_FBO));
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void ImGui_ContentWindowHandler::PostRender()
{
    unsigned int temp = ms_front_FBO;
    ms_front_FBO = ms_back_FBO;
    ms_back_FBO = temp;

    // blit multisampled frame to writing framebuffer obj
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_front_FBO));
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO));
    GLCall(glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

    // output to imgui
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        reinterpret_cast<void*>(m_TXT), ImVec2(ImGui::GetWindowPos().x + padding,
            ImGui::GetWindowPos().y + padding * 3),
        ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - padding, 
            ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - padding), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    // back to default FrameBuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

}
