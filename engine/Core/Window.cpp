#include "Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Circe {

    Window::Window(int width, int height, const char* title)
        : m_Width(width), m_Height(height) {
        
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_Window);
        SetVSync(true);

        // Framebuffer size callback
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    }

    Window::~Window() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(m_Window);
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SetVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
    }

}