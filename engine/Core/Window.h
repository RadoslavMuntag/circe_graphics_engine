#pragma once

struct GLFWwindow;

namespace Circe {

    class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        void PollEvents();
        void SwapBuffers();
        bool ShouldClose() const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        GLFWwindow* GetNativeWindow() const { return m_Window; }

        void SetVSync(bool enabled);

    private:
        GLFWwindow* m_Window;
        int m_Width;
        int m_Height;
    };

}