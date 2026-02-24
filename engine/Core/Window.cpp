#include "pch.h"

#include "Window.h"
#include "Log.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include <GLFW/glfw3.h>
#include <spdlog/fmt/fmt.h>

namespace Circe {

    Window::Window(int width, int height, const char* title)
        : m_Width(width), m_Height(height) {
        
        if (!glfwInit()) {
            Log::Critical("Failed to initialize GLFW");
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            Log::Critical("Failed to create GLFW window");
            throw std::runtime_error("Failed to create GLFW window");
        }
        
        Log::Info(fmt::runtime("Window created: {}x{} '{}'"), width, height, title);

        glfwMakeContextCurrent(m_Window);
        SetVSync(true);
        glfwSetWindowUserPointer(m_Window, this);

        // Framebuffer size callback
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            Log::Info(fmt::runtime("Window resized: {}x{}"), width, height);
            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner) {
                return;
            }

            owner->m_Width = width;
            owner->m_Height = height;
            glViewport(0, 0, width, height);

            WindowResizeEvent event(width, height);
            if (owner->m_EventCallback) {
                owner->m_EventCallback(event);
            }
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner) {
                return;
            }

            WindowCloseEvent event;
            if (owner->m_EventCallback) {
                owner->m_EventCallback(event);
            }
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            (void)scancode;
            (void)mods;

            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner || !owner->m_EventCallback) {
                return;
            }

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    owner->m_EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    owner->m_EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    owner->m_EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner || !owner->m_EventCallback) {
                return;
            }

            KeyTypedEvent event(static_cast<int>(codepoint));
            owner->m_EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner || !owner->m_EventCallback) {
                return;
            }

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            owner->m_EventCallback(event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner || !owner->m_EventCallback) {
                return;
            }

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            owner->m_EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            (void)mods;

            auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!owner || !owner->m_EventCallback) {
                return;
            }

            if (action == GLFW_PRESS) {
                MouseButtonPressedEvent event(button);
                owner->m_EventCallback(event);
            } else if (action == GLFW_RELEASE) {
                MouseButtonReleasedEvent event(button);
                owner->m_EventCallback(event);
            }
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